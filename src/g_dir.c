/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2002  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "g_dir.h"
#include "g_malloc.h"
#include "g_secure.h"
#include "g_debug.h"
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if STDC_HEADERS
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef __CHECKER__
#include <string.h>
#endif
/*
 * From "autoconf" info :
 */
#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#ifndef HAVE_ALPHASORT
int alphasort() ;
#endif

/* This function can't work : not portable on HPUX */
/*
int myalphasort(const void *a, const void *b)
{
  return( strcmp( ((struct dirent *)a)->d_name
                , ((struct dirent *)b)->d_name ) ) ;
}
*/

/*
 *
 */

#if HAVE_DIRENT_CONST
static int g_Selected(const struct dirent *f)
#else
static int g_Selected(struct dirent *f)
#endif
{
  /* Filter ~ file */

  if (strcmp(f->d_name,"")  == 0) return(0) ;
  if (strcmp(f->d_name,".") == 0) return(0) ;
  if (strcmp(f->d_name,"..")== 0) return(0) ;
  return(1) ;
}

/*
 *
 */

void g_Free_dirent(struct dirent **f, struct stat *st, int nb)
{
  int i ;

  for(i=0;i<nb;i++)
    free(f[i]) ;
  free(f) ;
  g_Free(st) ;
}

void g_Output_Null(FILE *output)
{
  const int zero = 0 ;
  fwrite(&zero, 1, sizeof(zero), output) ;
}

void G_FUNCTION(g_Pipe_Directory,(FILE *output, char *name))

  struct dirent **f ;
  struct stat *st ;
  int i ;
  int nb ;
  char *new_name ;

  /*
   * Read content of the directory
   */
  nb = scandir(".", &f, g_Selected, alphasort) ;
  
  if ( nb < 0 )
    {
      /*
       * Directory deleted while NNFS run
       */
      g_Output_Null(output) ;
      G_RETURN(;) ;
    }

  G_MALLOC(st, nb) ;
  for(i=0;i<nb;i++)
    {
      if ( lstat(f[i]->d_name, &st[i]) )
	{
	  free(f[i]) ;
	  memcpy(&f[i], &f[i+1], sizeof(f[i]) * (nb-i-1) ) ;
	  i-- ;
	}
    }
  /*
   * Output directory to pipe
   */
  fwrite(&nb, 1, sizeof(int), output) ;
  for(i=0;i<nb;i++)
    {
      fwrite(f[i]->d_name, 1, strlen(f[i]->d_name)+1, output) ;
      fwrite(&st[i], 1, sizeof(st[i]), output) ;
    }
  /*
   * Same for the children
   */
  for(i=0;i<nb;i++)
    {
      if ( S_ISDIR(st[i].st_mode) )
	{
	  if ( chdir(f[i]->d_name) )
	    {
	      g_Output_Null(output) ;
	    }
	  else
	    {
	      new_name = g_Formatted_Strings("%" G_STRING_SEPARATOR "%",
					     name, f[i]->d_name);
	      g_Pipe_Directory(output, new_name) ;
	      g_Free(new_name) ;
	      if ( chdir("..") )
		{
		  (void)chdir(name) ;
		}
	    }
	}
    }

  g_Free_dirent(f, st, nb) ;
  G_RETURN(;) ;
}

/*
 *
 */

void G_FUNCTION(g_Read_Pipe_Directory,(g_Dir **fi, int *nb, FILE* input, const g_Config *config))

  struct stat st ;
  int i, k ;
  
  g_fread((void*)nb, 1, sizeof(nb), input) ;

  G_MALLOC(*fi, *nb) ;
  k = 0 ;
  for(i=0;i<*nb;i++)
    {
      (*fi)[i].name = g_Read_Name(input) ;
      g_fread((void*)&st, 1, sizeof(st), input) ;
      if ( config->cross_mount_point
           || config->device_number_to_copy == st.st_dev )
      g_Stat_To_File_On_Host(&st, &(*fi)[k++].info, config) ;
    }
  *nb = k ;
  G_RETURN(;) ;
}


/*
 * Convert stat to g_File_On_Host
 */

void g_Stat_To_File_On_Host(const struct stat *st, g_File_On_Host *foh, const g_Config *config)
{
  /* to shutup the memory checkers */

  memset(foh, '\0', sizeof(*foh)) ;


if ( S_ISLNK(st->st_mode) ) foh->type = g_Is_A_Link ;
   else
      if ( S_ISDIR(st->st_mode) ) foh->type = g_Is_A_Directory ;
         else
            if ( S_ISREG(st->st_mode) ) foh->type = g_Is_A_File ;
               else
		 if ( S_ISFIFO(st->st_mode) ) foh->type = g_Is_A_FIFO ;
		 else
		   foh->type = g_Is_Not_Supported ;

/*
 * The size of directories is not important.
 */
if ( foh->type == g_Is_A_Directory )
  foh->size = 0 ;
else
  foh->size         = st->st_size ;

/* 01/11/2002 date 0 and 1 are set to 2.
 * In order to avoid DELETED_TIME
 */
if ( st->st_mtime > 1 )
	foh->date         = st->st_mtime ;
else
	foh->date         = 2 ;
foh->mode         = st->st_mode & G_MODE_MASK ;
foh->up_to_date   = g_False ;
foh->to_historize = g_False ;
#ifdef ROOTNNFS
foh->uid          = st->st_uid ;
foh->gid          = st->st_gid ;
#endif
/*
 * Hard links not supported
 */
if ( !config->copy_hard_link )
     if ( st->st_nlink != 1 && foh->type != g_Is_A_Directory )
           foh->type = g_Is_Not_Supported ;
/*
 * Mode 000 are not supported
 */
if ( foh->mode == 0 )
         foh->type = g_Is_Not_Supported ;
}

/*
 *
 */

void G_FUNCTION(g_Read_Directory,(g_Dir **fi, int *nb, const g_Config *config))
 
struct dirent **f ;
struct stat st ;
int i,k ;

*nb = scandir(".", &f, g_Selected, alphasort) ;

if ( *nb < 0 )
	{
	/*
	 * Directory deleted while NNFS run
	 */
	*nb = 0 ;
        G_MALLOC(*fi, 0) ;	/* 31/07/2001 : Because it will freed */
	G_RETURN(;) ;
	}

G_MALLOC(*fi, *nb) ;
k = 0 ;
for(i=0;i<*nb;i++)
	{
	if ( f[i]==NULL )
	  G_EXIT(4) ;
	if ( lstat(f[i]->d_name, &st) == 0
	     && ( config->cross_mount_point
		  || config->device_number_to_copy == st.st_dev
		  )
	     )
	  {
	    (*fi)[k].name = g_Save_String(f[i]->d_name) ;
	    g_Stat_To_File_On_Host(&st, &(*fi)[k++].info, config) ;
	  }
	free(f[i]) ;
	}
free(f) ;
*nb = k ;
G_RETURN(;) ;
}




