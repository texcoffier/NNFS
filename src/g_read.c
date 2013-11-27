/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2005  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_nnfs.h"
#include "g_read.h"
#include "g_secure.h"
#include "g_debug.h"
#include "g_dspbar.h"
#include "g_malloc.h"
#include "g_tree.h"
#include "g_histo.h"
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include "g_copy.h"

/*
 *
 */

g_Nnfs_Error G_FUNCTION(g_Read_Integer,(FILE *f, int* value))

int c ;
if ( fscanf(f, "%d", value) != 1 )
  {
    if ( G_VERBOSE )
      {
	G_PRINTF("Error Read integer\n") ;
	g_Copy_Stream(f, stderr, 80) ;
      }
    G_RETURN(g_Error_Read) ;
  }
c = getc(f) ;
if ( c != '\n' )
  {
    if ( G_VERBOSE )
      {
	G_PRINTF("c=%c(%d) value=%d\n",c,c,*value) ;
	g_Copy_Stream(f, stderr, 80) ;
      }
    G_RETURN(g_Error_Read) ;
  }
G_RETURN(g_No_Error) ;
}

/*
 *
 */

g_Nnfs_Error G_FUNCTION(g_Jump_Over,(FILE *input, int size))

char buf[G_LINE_CHUNK] ;
int len, read_len ;

while( size )
	{
	len = MINIMUM(G_LINE_CHUNK,size) ;
	read_len = fread(buf, 1, len, input) ;
        IF ( G_VERBOSE )
             {
             G_PRINTF("Over = (") ;
             fwrite(buf,1,read_len,stderr) ;
             fprintf(stderr,")\n") ;
             }
        
	if ( read_len != len ) G_RETURN(g_Error_Read) ;
	size -= len ;
	}
if ( g_Read_Integer(input,&size) ) G_RETURN(g_Error_Read) ;
G_RETURN( g_No_Error ) ;
}

/*
 *
 */

void G_FUNCTION(g_Set_File_Info,(char *name, g_File_Info *a))


#ifdef ROOTNNFS
     if ( chown(name, a->hosts[a->up_to_date_on_medium].uid,
                          a->hosts[a->up_to_date_on_medium].gid) )
       {
       a->read_error = g_Error_UidGid ;
       a->error      = ERROR_NUMBER ;
       }
   else
#endif
  if ( chmod(name, a->hosts[a->up_to_date_on_medium].mode) )
      {
      a->read_error = g_Error_Chmod ;
      a->error      = ERROR_NUMBER ;
      }

G_RETURN(;) ;
  }

/*
 *
 */
FILE *G_FUNCTION(g_Open_Tmp_Name,
                (g_NNFS *nnfs, const char *tmp_name, int size, g_File_Info *a))

FILE *w ;

w = g_fopen(tmp_name,"w") ; /* 2005-10-29 was fopen */
G_RETURN(w) ;
}
/*
 *
 */
void G_FUNCTION(g_Ask_User_To_Free_Disk_Space,(g_NNFS *nnfs, const char *operation, const char *file_name,
  int error))

char *s ;
char temp ;

if ( nnfs->config.confirmation == 0 )
     G_RETURN(;) ;


s = g_Formatted_Strings("Operation: %\nFile name: %\nError message: %",
  operation, file_name, strerror(error)) ;
g_Output(G_CAN_NOT_WRITE, s) ;
g_Free(s) ;
g_read(0,&temp,1) ;

G_RETURN(;) ;
}


void G_FUNCTION(g_Copy_The_File,
                (g_NNFS *nnfs, FILE *w, int size, g_File_Info *a))

int n, l, wrote ;
char buf[G_LINE_CHUNK] ;

while( size )
   {
      l = MINIMUM(G_LINE_CHUNK, size) ;
      n = fread(buf, 1, l, nnfs->state.input) ;
      if ( n != l )
	      {
	      a->read_error = g_Error_Read ;
	      G_PF("Read_Error\n") ;
	      break ;
	      }
      if ( !a->read_error ) /* No Error Write */
          {
              wrote = fwrite(buf, 1, n, w) ;
	      if ( n != wrote )
		      {
                      g_Ask_User_To_Free_Disk_Space(nnfs,"fwrite", a->name, errno) ;
                      if ( wrote < 0 )
                            wrote = 0 ;
                      if ( fwrite(buf+wrote, 1, n-wrote, w) != n-wrote )
                            {
		            a->read_error = g_Error_Write ;
		            a->error = ERROR_NUMBER ;
                            }                      
		      }
         }
      size -= n ;
   }
if ( fflush(w) )
  {
     g_Ask_User_To_Free_Disk_Space(nnfs,"fflush", a->name, errno) ;
     fflush(w) ;
  }
if ( fclose(w) )
  {
     G_PF("G_CAN_NOT_WRITE on close\n") ;
     a->read_error = g_Error_Write ;
     a->error = ERROR_NUMBER ;
  }

G_RETURN(;) ;
}

/*
 *
 */

void G_FUNCTION(g_Read_Normal_File,(g_NNFS *nnfs, g_File_Info *a))

char *tmp_name ;
FILE *w ;
int size, err ;

size = a->hosts[a->up_to_date_on_medium].size ;
G_PF("Name=%s Size=%d\n", nnfs->state.name, size) ;

w = NULL ;
if ( a->nnfs_to_local )
	{
        g_Display_Bar() ;

	tmp_name = g_Next_File_Name(&nnfs->state) ;
        w = g_Open_Tmp_Name(nnfs, tmp_name, size, a) ;
        if ( w == NULL )
            {
             /*
              * Free space or deleted directory
              */
               g_Ask_User_To_Free_Disk_Space(nnfs,"fopen", nnfs->state.name, errno) ;
               w = g_Open_Tmp_Name(nnfs, tmp_name, size, a) ;
            }
        if ( w )
           {
             g_Copy_The_File(nnfs, w, size, a) ;
             G_PF("read_error=%d\n", a->read_error) ;
             if ( a->read_error != g_Error_Read )
               {
	       if ( g_Read_Integer(nnfs->state.input, &err) )
                  a->read_error = g_Error_Read ;
               else
                   if ( err )
		       a->read_error = g_Error_Is_On_Medium ;
               }
             if ( !a->read_error )
                {
        	   g_Set_File_Info(tmp_name, a) ;
		   if ( !a->to_delete )
                       {
                        if ( g_Historize(nnfs,a) )
			   a->read_error = g_Error_Historize ;
		       }
		   if ( g_rename(tmp_name, nnfs->state.name) )
		     {
		       a->read_error = g_Error_Rename_To_Real_Name ;
		     }
                }
             /*
              * Former "if" can raise a read error
              */
             if ( a->read_error )
                {
                G_PF("Read error, so destroy the tmp file\n") ;
                (void)g_remove(tmp_name) ;
                }
           }
        else
           {
             G_PF("Can't create file %s\n", nnfs->state.name) ;
             a->read_error = g_Error_Create_File ;
             g_Jump_Over(nnfs->state.input, size) ;
           }
        g_Free(tmp_name) ;
	}
else
     {
	if ( g_Jump_Over(nnfs->state.input, size) )
		{
		a->read_error = g_Error_Read ;
		G_PF("Read_Error\n") ;
		}
    }
G_PF("Read_error = %d\n", a->read_error) ;
		
G_RETURN(;) ;
}

/*
 *
 */


void g_Read_Will_Be_Impossible(g_NNFS *nnfs, g_File_Info *f)
{
   f->nnfs_to_local = g_False ;
   f->error = ERROR_NUMBER ;
   f->read_error = g_Error_Mkdir ;
}

g_Boolean g_File_Is_A_Directory(const char *pn)
{
  struct stat st ;
  
  if ( stat(pn, &st) )
    return(g_False) ;

  return( (g_Boolean)S_ISDIR(st.st_mode) ) ;
}


void G_FUNCTION(g_Read_Directory_File,(g_NNFS *nnfs, g_File_Info *a))

int error;
if ( a->nnfs_to_local )
        {
        g_Display_Bar() ;

	if ( a->hosts[nnfs->header.nb_fs].type != g_Is_A_Directory )
	    {
	    g_Always_Delete_A_File(nnfs,a) ;
	    if ( g_mkdir(nnfs->state.name, a->hosts[a->up_to_date_on_medium].mode) )
	        {
		  error = errno;
		    if ( !g_File_Is_A_Directory(nnfs->state.name) )
		    {

                      g_Ask_User_To_Free_Disk_Space(nnfs,"mkdir", nnfs->state.name, error) ;
	              if ( g_mkdir(nnfs->state.name, a->hosts[a->up_to_date_on_medium].mode) )
		       {
                       /*
		        * Can't make the local directory
		        * So : can't update under.
                        */
                      g_Tree_Travel1(nnfs, a, NULL
				     , g_Read_Will_Be_Impossible) ;
		       }
		    }
		}
	    }
	/*	else    */   /* commented the 24/03/2002 */
	  g_Set_File_Info(nnfs->state.name, a) ;
        }
G_RETURN(;) ;
}

/*
 *
 */

void G_FUNCTION(g_Read_FIFO,(g_NNFS *nnfs, g_File_Info *a))

int error;

if ( a->nnfs_to_local )
        {
        g_Display_Bar() ;

	if ( a->hosts[nnfs->header.nb_fs].type != g_Is_A_FIFO )
	    {
	    g_Always_Delete_A_File(nnfs,a) ;
            if ( g_mkfifo(nnfs->state.name, a->hosts[a->up_to_date_on_medium].mode) )
                {
                error = errno;
		if ( !g_File_Is_A_Directory(nnfs->state.name) )
		    {
                    g_Ask_User_To_Free_Disk_Space(nnfs,"mkfifo", nnfs->state.name, error) ;
	            if ( g_mkfifo(nnfs->state.name, a->hosts[a->up_to_date_on_medium].mode) )
		         {
		         a->read_error = g_Error_Create_FIFO ;
		         a->error = ERROR_NUMBER ;
		         }
                    }
                }
	    }
	  g_Set_File_Info(nnfs->state.name, a) ;
        }
G_RETURN(;) ;
}

/*
 *
 */

void G_FUNCTION(g_Read_Symbolic_File,(g_NNFS *nnfs, g_File_Info *a))

char buf[G_LINE_CHUNK] ;
int size, error ;
char *tmp_name ;

G_PF("Name=%s\n", nnfs->state.name) ;

if ( g_Read_Integer(nnfs->state.input, &size) )
		{
		a->read_error = g_Error_Read ;
		G_PF("Read_Error\n") ;
		G_RETURN(;) ;
		}
G_PF("Size = %d\n", size) ;

if ( a->nnfs_to_local )
	{
        g_Display_Bar() ;

	tmp_name = g_Next_File_Name(&nnfs->state) ;
	if ( size>=G_LINE_CHUNK )
		{
		G_PRINTF("Symbolic link too long\n") ;
		G_EXIT(1) ;
		}
	if ( fread(buf, 1, size, nnfs->state.input) != size )
		{
		a->read_error = g_Error_Read ;
		G_PF("Read_Error\n") ;
		g_Free(tmp_name) ;
		G_RETURN(;) ;
		}
	buf[size] = '\0' ;
	if ( g_Read_Integer(nnfs->state.input, &error) )
		{
		a->read_error = g_Error_Read ;
		G_PF("Read_Error\n") ;
		g_remove(tmp_name) ;
		g_Free(tmp_name) ;
		G_RETURN(;) ;
		}
	if ( g_symlink(buf, tmp_name) )
		{
                g_Ask_User_To_Free_Disk_Space(nnfs,"symlink", a->name, errno) ;
	        if ( g_symlink(buf, tmp_name) )
                     {
		     a->read_error = g_Error_Create_Symbolic_Link ;
		     a->error = ERROR_NUMBER ;
                     }
		}
#ifdef ROOTNNFS
#if HAVE_LCHOWN
        if ( !a->read_error )
               {
        /* 24/03/2002 Change UID/GID/Mode of a symbolic link
         * if it is possible.
         */
     		if ( lchown(tmp_name, a->hosts[a->up_to_date_on_medium].uid,
		  a->hosts[a->up_to_date_on_medium].gid) )
                           PERROR("lchown") ;
               }
#endif
#endif

	G_PF("Error = %d\n", error) ;
	if ( error ) a->read_error = g_Error_Is_On_Medium ;
	if ( a->read_error )
		g_remove(tmp_name) ;
	else
		if ( g_rename(tmp_name, nnfs->state.name) )
			{
			a->error = ERROR_NUMBER ;
			a->read_error = g_Error_Rename_To_Real_Name ;
			g_remove(tmp_name) ;
			}
	g_Free(tmp_name) ;
	}
else
	if ( g_Jump_Over(nnfs->state.input, size) )
		{
		a->read_error = g_Error_Read ;
		a->error = ERROR_NUMBER ;
		G_PF("Read_Error\n") ;
		}
G_RETURN(;) ;
}

/*
 * Read a file from medium
 */

void G_FUNCTION(g_Read_A_File,(g_NNFS *nnfs, g_File_Info *a))

G_PF("name = %s on-medium=%d state=%d\n", nnfs->state.name, a->on_medium,
		nnfs->state.read_error) ;
fprintf(stderr,"\n\nread:%s\n", nnfs->state.name);

if ( !a->on_medium )
  G_RETURN(;) ;

if (    nnfs->state.read_error
     && a->nnfs_to_local
     && a->hosts[a->up_to_date_on_medium].type != g_Is_A_Directory )
	{
	a->read_error = g_Error_Read ;
        g_Display_Bar_Error() ;
	G_RETURN(;) ;
	}

switch( a->hosts[a->up_to_date_on_medium].type )
	{
	case g_Is_A_Deleted :   break ; /* Deletion was yet done */
	case g_Is_A_File :      g_Read_Normal_File   (nnfs,a) ; break ;
	case g_Is_A_Directory : g_Read_Directory_File(nnfs,a) ; break ;
	case g_Is_A_Link :      g_Read_Symbolic_File (nnfs,a) ; break ;
	case g_Is_A_FIFO :      g_Read_FIFO          (nnfs,a) ; break ;
	case g_Is_Not_Supported:g_Printf("Conflicting version of NNFS") ;
	  break ;
	}
if ( a->read_error && a->nnfs_to_local )
{
fprintf(stderr, "NNFS: %s\nNNFS: Error ", nnfs->state.name) ;
switch( a->read_error )
{
 case g_Error_Chmod:
   fprintf(stderr, "changing the file mode\n") ;
   break ;
 case g_Error_UidGid:
   fprintf(stderr, "change the file owner/group\n") ;
   break ;
 case g_Error_Read:
   fprintf(stderr, "reading the medium\n") ;
   break ;
 case g_Error_Write:
   fprintf(stderr, "writing the file content\n") ;
   break ;
 case g_Error_Is_On_Medium:
   fprintf(stderr, "of the last invocation of nnfs failed to wrote the file on the medium\n") ;
   break ;
 case g_Error_Historize:
   fprintf(stderr, "moving the file to the history directory\n") ;
   break ;
 case g_Error_Rename_To_Real_Name:
   fprintf(stderr, "renaming the temporary file to the real name\n") ;
   break ;
 case g_Error_Create_File:
   fprintf(stderr, "creating the file\n") ;
   break ;
 case g_Error_Mkdir:
   fprintf(stderr, "creating the directory\n") ;
   break ;
 case g_Error_Create_Symbolic_Link:
   fprintf(stderr, "creating the symbolic link\n") ;
   break ;
 case g_Error_Create_FIFO:
   fprintf(stderr, "creating the FIFO file\n") ;
   break ;
 default:
   G_PRINTF("Unknown\n") ;
}
g_Display_Bar_Error() ;
}

if ( a->read_error == g_Error_Read )
	nnfs->state.read_error = g_True ;

G_RETURN(;) ;
}









