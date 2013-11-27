/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2006  Thierry EXCOFFIER (exco@liris.univ-lyon1.fr)
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

#include "g_debug.h"
#include "g_malloc.h"
#include "g_nnfs.h"
#include "g_file.h"
#include "g_read.h"
#include "g_tree.h"
#include "g_dir.h"
#include "g_secure.h"
#include "g_copy.h"
#include "g_dspbar.h"
#include "g_report.h"
#include "g_histo.h"
#include "g_update.h"
#include "g_filter.h"
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_UTIME_H
#include <utime.h>
#endif

/*
 *
 */

void g_Tree_Travel1( g_NNFS *nnfs, g_File_Info *f,
			void (*fct)(g_NNFS *, g_File_Info*),
			void (*post)(g_NNFS *, g_File_Info*))
{
int i ;
int cl, ml ;
g_File_Info *dir ;
int child, dir_len ;

if ( fct ) (*fct)(nnfs, f) ;

ml = 0 ;
for(i=0;i<f->nb_children;i++)
	{
	cl = strlen(f->children[i]->name) ;
	if ( cl > ml ) ml = cl ;
	}
cl = strlen(nnfs->state.name) ;
if ( cl + ml + 2 > nnfs->state.max_file_name )
   {
   nnfs->state.max_file_name = cl + ml + 2 ;
   nnfs->state.name = g_Realloc(nnfs->state.name, nnfs->state.max_file_name) ;
   }

nnfs->state.name[cl] = G_CHAR_SEPARATOR ;
dir     = nnfs->state.dir ;
child   = nnfs->state.child ;
dir_len = nnfs->state.dir_len ;
nnfs->state.dir_len = cl ;	/* Len without the last / */
nnfs->state.dir     = f ;
for(i=0;i<f->nb_children;i++)
	{
	strcpy(&nnfs->state.name[cl+1], f->children[i]->name) ;
	nnfs->state.child = i ;
	g_Tree_Travel1(nnfs, f->children[i], fct, post) ;
	}
nnfs->state.name[cl]         = '\0' ;
nnfs->state.dir     = dir ;
nnfs->state.child   = child ;
nnfs->state.dir_len = dir_len ;
if ( post ) (*post)(nnfs, f) ;
}

void g_Tree_Travel( g_NNFS *nnfs,
			void (*fct)(g_NNFS *, g_File_Info*),
			void (*post)(g_NNFS *, g_File_Info*))
{
  /* These 2 lines to make checkers happy */
  nnfs->state.child = 0 ;
  nnfs->state.dir_len = 0 ;

  nnfs->state.dir = nnfs->state.nnfs ;
  g_Tree_Travel1(nnfs, nnfs->state.nnfs, fct, post) ;
}

/*
 *
 */

static int g_nb_files ;

static void g_File_Count(g_NNFS *nnfs, g_File_Info *fi)
{
g_nb_files++ ;
}

int g_Nb_Files(g_NNFS *nnfs)
{
g_nb_files = 0 ;
g_Tree_Travel(nnfs, g_File_Count, NULL) ;
return(g_nb_files) ;
}

/*
 *
 */

void G_FUNCTION(g_Filter_A_File,(g_NNFS *nnfs, g_File_Info *f))

g_Display_Bar() ;
/* The 2003/09/01 the test was : ( nnfs->state.name[1] )
 * But it is not important because "." is really the only top directory
 */
if ( strcmp(".", nnfs->state.name) ) /* Not the "." file */
{
  if ( nnfs->state.dir->filtered ) /* Added the 2003/09/02 */
    f->filtered = g_True ;
  else
    f->filtered = 
	      g_Eval_Filter(nnfs->state.name+2,
	    &f->hosts[f->nnfs_to_local ? f->up_to_date : nnfs->header.nb_fs],
			 nnfs->config.output) ;
    
}

/*
 * Solve "tst/test29" problems
 */
if ( nnfs->state.dir->to_delete && !f->to_delete )
  f->to_delete = g_True ;
/*
 *
 * All is done now for the file
 * Send stats to user interface
 *
 */
if ( f->to_delete     ) nnfs->state.nb_to_delete++ ;
if ( f->local_to_nnfs
     && !f->filtered
     && !f->hosts[f->up_to_date].type != g_Is_A_Directory
     ) nnfs->state.nb_local_to_nnfs++ ;
if ( f->nnfs_to_local
     && !f->to_delete
     && !f->hosts[f->up_to_date].type != g_Is_A_Directory
     ) nnfs->state.nb_nnfs_to_local++ ;
if ( f->local_change  ) nnfs->state.nb_local_change++ ;
if ( f->nnfs_change   ) nnfs->state.nb_nnfs_change++ ;
if ( f->read_error    ) nnfs->state.nb_read_error++ ;
if ( f->conflict      ) nnfs->state.nb_conflict++ ;
if ( f->hosts[nnfs->header.nb_fs].type != g_Is_A_Deleted ) nnfs->state.nb_local_file++ ;

if ( f->local_to_nnfs
     && !f->filtered
     && f->hosts[f->up_to_date].type != g_Is_A_Deleted
     && !(!f->nnfs_to_local
	  && f->hosts[nnfs->header.nb_fs].type == g_Is_A_Deleted)
     )
  nnfs->state.nb_byte +=  f->hosts[f->up_to_date].size ;

if ( strcmp( nnfs->state.name, "." ) )
{
  if ( (f->conflict || f->to_delete || f->nnfs_to_local || f->local_to_nnfs)
       && !f->filtered )
    g_Output_Binary2(G_FILE, f,
		     sizeof(*f)+ (nnfs->header.nb_fs-0)*sizeof(g_File_On_Host),
		     nnfs->state.name + nnfs->state.dir->to_delete ) ;

  if ( f->filtered &&
       !f->conflict && !f->to_delete &&
       f->hosts[f->up_to_date].type!=g_Is_Not_Supported )
    {
      fprintf(nnfs->state.filteredlogfile, "%s\n", nnfs->state.name+2) ;
      G_RETURN(;) ;
    }

  if ( f->nnfs_to_local || f->local_to_nnfs || f->to_delete || f->conflict )
    {
      fprintf(nnfs->state.logfile, 
	      "%c%c%c%c%c%c%c %s\n",
	      f->nnfs_to_local ? 'R' : ' ',
	      f->local_to_nnfs ? 'W' : ' ',
	      f->to_delete     ? 'D' : ' ',
	      f->conflict      ? 'C' : ' ',
	      f->nnfs_filtered ? 'f' : ' ',
	      f->filtered      ? 'F' : ' ',
	      f->to_historize  ? 'H' : ' ',
	      nnfs->state.name+2) ;
      fprintf(nnfs->state.logfile, g_other_file_info(nnfs->header.nb_fs, f,1)) ;
  }
}

G_RETURN(;) ;
}


/*
 *
 */

void G_FUNCTION(g_Write_An_Nnfs_File, (g_NNFS *nnfs,
					g_File_Info *info))

int i ;

G_PF("File = %s\n", info->name) ;

g_Display_Bar() ;
g_fprintf(nnfs->state.output,
	"%s%c%d %d %x\n", info->name, '\0',
	  /* On medium: should be same expression as in  g_Write_A_File */
	  /*  (info->local_to_nnfs && !info->filtered && !info->read_error)*/
/* To fix test47 the 13/12/2002 */
	 (info->local_to_nnfs && !info->filtered
	  && ( !info->read_error || !info->nnfs_to_local) )
/* 22/2/98 */  || ( info->hosts[info->up_to_date].type == g_Is_A_Directory )
	  ,
          /* filtered and error */
	(info->filtered || (info->nnfs_filtered&&!info->local_to_nnfs))+
	  /*          (info->read_error?2:0) */
/* To fix test47 the 13/12/2002 */
          ((info->read_error && info->nnfs_to_local)?2:0)
	  ,
			 info->nb_children) ;
for(i=0;i<nnfs->header.nb_fs;i++)
	g_fprintf(nnfs->state.output,"%d %x %lx %lx %d %d"
#ifdef ROOTNNFS
                        " %x %x"
#endif
                        "\n"
			,info->hosts[i].type
			,info->hosts[i].mode
			,info->hosts[i].date
			,info->hosts[i].size
			,info->hosts[i].up_to_date
			,info->hosts[i].to_historize
#ifdef ROOTNNFS
                        ,info->hosts[i].uid
                        ,info->hosts[i].gid
#endif
			) ;
G_RETURN(;) ;
}

/*
 *
 */

void G_FUNCTION(g_File_Changement,(g_NNFS *nnfs, g_File_Info *b))

g_File_On_Host *a, after_info ;
struct stat after ;

a = &b->hosts[nnfs->header.fs_id] ;

if ( lstat(nnfs->state.name, &after) )
{
   b->write_error = g_Error_Deleted ;
}
else
   {
   g_Stat_To_File_On_Host(&after, &after_info, &nnfs->config) ;
   if ( a->type != after_info.type ) b->write_error = g_Error_Type_Changed ;
   else
#if HAVE_LINK_MTIME
   if ( a->date != after_info.date ) b->write_error = g_Error_Time_Changed ;
   else
#endif
     if ( nnfs->config.mode_change_sensitive /* 2006/02/02 for test54 */
	&& a->mode != after_info.mode ) b->write_error = g_Error_Mode_Changed ;
   else
   if ( a->size != after_info.size ) b->write_error = g_Error_Size_Changed ;
   }

g_fprintf(nnfs->state.output, "%d\n", b->write_error != g_No_Error) ;
G_RETURN(;) ;
}
	
/*
 * fs_id
 */

void G_FUNCTION(g_Write_A_File,(g_NNFS *nnfs, g_File_Info *a))

int size ;
FILE *r ;
int i ;
char buf[G_LINE_CHUNK] ;

/* old version
if ( a->local_to_nnfs && !a->filtered && !a->read_error)
*/
/* To fix test47 the 13/12/2002 */
G_PF("local2nnfs %d filtered %d readerr=%d nnfs2local %d\n",
   a->local_to_nnfs, a->filtered
     ,a->read_error,a->nnfs_to_local) ;
if ( a->local_to_nnfs && !a->filtered
     && ( !a->read_error || !a->nnfs_to_local)
     )
   {
   g_Display_Bar() ;
   G_PF("File = %s\n",nnfs->state.name) ;

   switch( a->hosts[nnfs->header.fs_id].type )
	{
	case g_Is_A_File :
		r = fopen(nnfs->state.name,"r") ;
		G_PF("Open %s = %d\n", nnfs->state.name,(int)r) ;
		if ( r )
		   {
		   if ( g_Copy_Stream(r, nnfs->state.output,
			 a->hosts[nnfs->header.fs_id].size) )
			{
			a->write_error = g_Error_Read ;
			G_PF("error copy\n") ;
			}
		   if ( fclose(r) )
			{
			a->error = ERROR_NUMBER ;
			a->write_error = g_Error_Close ;
			}
		   }
		else
		   {
/*		   g_fprintf(nnfs->state.output, "11\ncan't read\n") ;*/
		   a->error = ERROR_NUMBER ;
		   a->write_error = g_Error_Open ;
                   strcpy(buf, nnfs->state.name) ;
                   for(i=strlen(buf);i<G_LINE_CHUNK;i++)
                       buf[i] = " Source File Can't be Read\n"[i%26];
                   i = a->hosts[nnfs->header.fs_id].size ;
                   while( i>G_LINE_CHUNK )
                        {
			fwrite(buf, 1, G_LINE_CHUNK, nnfs->state.output) ;
                        i -= G_LINE_CHUNK ;
                        }
		   fwrite(buf, 1, i, nnfs->state.output) ;
		   }
		g_File_Changement(nnfs,a) ;
		break ;
	case g_Is_A_Directory :
	case g_Is_A_FIFO :
	case g_Is_A_Deleted :
		break ;
	case g_Is_A_Link :
		size = readlink(nnfs->state.name, buf, G_LINE_CHUNK) ;
		if ( size <= 0 )
			{
			a->write_error = g_Error_Read_Link ;
			a->error = ERROR_NUMBER ;
			g_fprintf(nnfs->state.output, "1\n?") ;
                        PERROR(nnfs->state.name) ;
			}
		else
			{
			g_fprintf(nnfs->state.output, "%d\n", size) ;
			fwrite(buf, 1, size, nnfs->state.output) ;
			}
		g_File_Changement(nnfs,a) ;
		break ;
	case g_Is_Not_Supported:
		G_PRINTF("Bug!") ;
		g_Display_File_Info(nnfs,a) ;
		G_EXIT(1) ;
	}
   }

if ( a->write_error != g_No_Error )
{
fprintf(stderr, "NNFS: %s\nNNFS: Error ", nnfs->state.name) ;
switch( a->write_error )
{
 case g_Error_Deleted:
   fprintf(stderr, "the file was deleted while NNFS was running\n") ;
   break ;
 case g_Error_Type_Changed :
   fprintf(stderr, "the file type changed while NNFS was running\n") ;
   break ;
 case g_Error_Mode_Changed:
   fprintf(stderr, "the file mode changed while NNFS was running\n") ;
   break ;
 case g_Error_Size_Changed:
   fprintf(stderr, "the file size changed while NNFS was running\n") ;
   break ;
 case g_Error_Time_Changed:
   fprintf(stderr, "the file was modified while NNFS was running\n") ;
   break ;
 case g_Error_Read:
   fprintf(stderr, "while reading the file\n") ;
   break ;
 case g_Error_Close:
   fprintf(stderr, "closing the file\n") ;
   fprintf(stderr, "%s\n", strerror(a->error)) ;
   break ;
 case g_Error_Open:
   fprintf(stderr, "opening the file\n") ;
   fprintf(stderr, "%s\n", strerror(a->error)) ;
   break ;
 case g_Error_Read_Link:
   fprintf(stderr, "reading the symbolic link\n") ;
   fprintf(stderr, "%s\n", strerror(a->error)) ;
   break ;
 case g_No_Error:
   fprintf(stderr, "BUG !\n") ;
   break ;
 default:
   fprintf(stderr, "unknown\n") ;
   break ;
}
}

G_RETURN(;) ;
}

/*
 *
 */

void G_FUNCTION(g_Always_Delete_A_File,(g_NNFS *nnfs, g_File_Info *a))

g_Display_Bar() ;
switch( a->hosts[nnfs->header.nb_fs].type )
	{
	case g_Is_A_Directory :
  	     if ( g_rmdir(nnfs->state.name) )
  		{
                  if ( errno!=ENOENT )
		    {
                      g_Printf("I can't delete the directory ``%s''\n",
			       nnfs->state.name) ;
                      g_Printf("Delete the directory yourself\n");
                      g_Ask_User_To_Free_Disk_Space(nnfs,"free", nnfs->state.name, errno) ;
		    }
		}
	    break ;
	case g_Is_A_Deleted :
		break ;
	default :
		g_Historize(nnfs,a) ;
		break ;
	}
G_RETURN(;) ;
}

void G_FUNCTION(g_Delete_A_File,(g_NNFS *nnfs, g_File_Info *a))

int i ;

if ( a->to_delete )
	g_Always_Delete_A_File(nnfs, a) ;
/* 15/08/2001 Add the if */

if ( ( !a->local_change && a->hosts[a->up_to_date].type != g_Is_A_Directory)
|| ( a->local_change &&  a->hosts[nnfs->header.nb_fs].type != g_Is_A_Directory)
     )
      G_RETURN(;) ;


   for(i=0;i<a->nb_children;i++)
	if ( a->children[i]->nnfs_to_local )
		{
		a->need_time_stamp = g_True ;
		break ;
		}

G_RETURN(;) ;
}


/*
 *
 */

void G_FUNCTION(g_Time_Stamp_A_File,(g_NNFS *nnfs, g_File_Info *a))

struct utimbuf t ;
struct stat st ;
int i ;

G_PF("Time stamp %s\n", nnfs->state.name) ;
g_Display_Bar() ;
/* 17/4/1998
if ( a->read_error )
    a->filtered = g_True ;
*/

a->update_nnfs     = a->local_change && !a->need_time_stamp ;
/* if ( a->conflict ) a->update_nnfs = g_False ; */ /* 06/08/97 */

a->need_time_stamp |= a->nnfs_to_local
	           && a->hosts[a->up_to_date].type != g_Is_A_Link ;

if ( a->hosts[a->up_to_date].type == g_Is_A_Deleted
   || a->read_error )
	a->need_time_stamp = g_False ;

a->read_file_info  = a->nnfs_to_local
                  && a->hosts[a->up_to_date].type == g_Is_A_Link
                  && !a->read_error ;

if ( a->nnfs_to_local && !a->read_error)
	{
	G_PF("Take up to date informations\n") ;
	a->hosts[nnfs->header.fs_id] = a->hosts[a->up_to_date] ;
	a->hosts[nnfs->header.fs_id].up_to_date = g_True ;
	}

if ( a->need_time_stamp )
	{
	t.actime  = a->hosts[a->up_to_date].date ;
	t.modtime = a->hosts[a->up_to_date].date ;
	if ( utime(nnfs->state.name, &t) )
		{
                PERROR(nnfs->state.name) ;
                fprintf(stderr, "NNFS: Error setting file date\n") ;
		a->error = ERROR_NUMBER ;
		a->read_error = g_Error_Get_Time ;
		}
	else
		{
		/* Directory case, date modified */
	        G_PF("Set file date informations: OK\n") ;
               a->hosts[nnfs->header.fs_id].date = a->hosts[a->up_to_date].date ;
		}
        }

if ( a->read_file_info )	/* Only for Symbolics links */
	{
	G_PF("Reread ALL file informations\n") ;
	if ( lstat( nnfs->state.name, &st ) )
		{
                PERROR(nnfs->state.name) ;
                fprintf(stderr, "NNFS: Error reading file informations\n") ;
		a->read_error = g_Error_Lstat ;
                g_Time_Stamp_A_File(nnfs, a) ;
        	G_RETURN(;) ;
		}
	else
		{
                g_File_On_Host n ;

		g_Stat_To_File_On_Host(&st, &n, &nnfs->config ) ;
                if ( n.type == a->hosts[nnfs->header.fs_id].type
		&& n.size == a->hosts[nnfs->header.fs_id].size /* 18/08/2001 */
		)
                   {
		   a->hosts[nnfs->header.fs_id] = n ;
		   a->hosts[nnfs->header.fs_id].up_to_date = g_True ;
                   }
                else
                   {
                      PERROR(nnfs->state.name) ;
                      fprintf(stderr,
	                      "NNFS: file type changed while reading\n") ;
                      a->read_error    = g_Error_Type_Change_While_Read ;
                      g_Time_Stamp_A_File(nnfs, a) ;
        	      G_RETURN(;) ;
                   }                   
		}
	}

if ( a->update_nnfs )
	{
	G_PF("Update nnfs informations\n") ;
	a->hosts[nnfs->header.fs_id] = a->hosts[nnfs->header.nb_fs] ;
	for(i=0;i<nnfs->header.nb_fs;i++)
        		a->hosts[i].up_to_date = g_False ;
        a->hosts[nnfs->header.fs_id].up_to_date = g_True ;
	}

for(i=0;i<nnfs->header.nb_fs;i++)
        if ( a->hosts[i].up_to_date )
		G_RETURN(;) ;

g_Display_File_Info(nnfs,a) ;
G_EXIT(1) ;
}
