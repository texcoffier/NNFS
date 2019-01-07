/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2001  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_const.h"
#include "g_update.h"
#include "g_debug.h"
#include "g_malloc.h"
#include "g_dspbar.h"
#include "g_filter.h"
#include "g_tree.h"
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

/*
 *
 */

char *g_Type_To_String(g_Type_Of_File t)
{
  switch( t )
    {
    case g_Is_A_File       : return( "Reg. file" ) ;
    case g_Is_A_Link       : return( "Sym. link" ) ;
    case g_Is_A_Directory  : return( "Directory" ) ;
    case g_Is_A_FIFO       : return( "FIFO     " ) ;
    case g_Is_A_Deleted    : return( "Deleted  " ) ;
    case g_Is_Any          : return( "Any      " ) ;
    case g_Is_Not_Supported: return( "Unknown  " ) ;
    }
  return( "Bug in " __FILE__ ) ;
}

/*
 *
 */

g_Type_Of_File g_String_To_Type(const char *s)
{
  if ( strcmp("Reg. file",s) == 0 ) return(g_Is_A_File       ) ;
  if ( strcmp("Sym. link",s) == 0 ) return(g_Is_A_Link       ) ;
  if ( strcmp("FIFO     ",s) == 0 ) return(g_Is_A_FIFO       ) ;
  if ( strcmp("Directory",s) == 0 ) return(g_Is_A_Directory  ) ;
  if ( strcmp("Deleted  ",s) == 0 ) return(g_Is_A_Deleted    ) ;
  if ( strcmp("Any      ",s) == 0 ) return(g_Is_Any          ) ;
  if ( strcmp("Unknown  ",s) == 0 ) return(g_Is_Not_Supported) ;
 abort() ;
 return(g_Is_Any) ;
}

/*
 * Display debug information on NNFS file
 */

void g_Display_Nnfs_File(const g_File_On_Host *f)
{
  g_Printf("date=") ;
  switch( f->date )
    {
    case G_DATE_DELETED_FILE: g_Printf("Del.file") ; break ;
    case G_DATE_FILTRED_FILE: g_Printf("Fil.file") ; break ;
    default                 : g_Printf("%9lx", f->date) ; break ;
    }
  g_Printf(" size=%8lx", f->size) ;
#ifdef ROOTNNFS
  g_Printf(" uid=%d gid=%d", f->uid, f->gid) ;
#endif
  g_Printf(" mode=%05o type=%s UpToDate=%d histo=%d\n",
	   f->mode,
	   g_Type_To_String((g_Type_Of_File)f->type),
	   f->up_to_date,
	   f->to_historize) ;
}


/*
 *
 */

void g_Display_File_Info(g_NNFS *nnfs, g_File_Info *a)
{
  int i ;
  g_Printf("%s "             , nnfs->state.name) ;
  if ( a->nnfs_to_local ) g_Printf("nnfs2local ") ;
  if ( a->local_to_nnfs ) g_Printf("local2nnfs ") ;
  if ( a->on_medium     ) g_Printf("onMedium ") ;
  if ( a->local_change  ) g_Printf("localChange ") ;
  if ( a->nnfs_change   ) g_Printf("nnfsChange ") ;
  if ( a->to_delete     ) g_Printf("toDelete ") ;
  if ( a->read_error    ) g_Printf("readErr=%d ", a->read_error) ;
  if ( a->write_error   ) g_Printf("writeErr=%d ", a->write_error) ;
  if ( a->need_time_stamp) g_Printf("needTimerStamp ") ;
  if ( a->read_file_info) g_Printf("readFileInfo ") ;
  if ( a->update_nnfs   ) g_Printf("updateNnfs ") ;
  if ( a->conflict      ) g_Printf("conflict ") ;
  if ( a->error         ) g_Printf("error=%d ", a->error) ;
  if ( a->to_historize  ) g_Printf("toHistorize ") ;
  if ( a->nb_children   ) g_Printf("nb_children=%d ", a->nb_children) ;
  g_Printf("up_to_date=%d ", a->up_to_date) ;
  g_Printf("up_to_date_on_medium=%d ", a->up_to_date_on_medium) ;
  if ( a->filtered  ) g_Printf("filtered ") ;
  if ( a->nnfs_filtered  ) g_Printf("nnfs_filtered ") ;
  if ( a->nnfs_error  ) g_Printf("nnfs_error ") ;
  if ( a->nnfs_to_local_child ) g_Printf("nnfs2localChild ") ;
  g_Printf("\n") ;
  for(i=0;i<=nnfs->header.nb_fs;i++)
	{
	g_Printf("%c%c%c%d ",
		 i==nnfs->header.fs_id ? 'C' : ' ',
		 i==a->up_to_date ? 'U' : ' ',
		 i==a->up_to_date_on_medium ? 'M' : ' ',
		 i) ;
	g_Display_Nnfs_File(&a->hosts[i]) ;
	}
  g_Printf("\n") ;

}

/*
 * This function is only called
 * when we add an host.
 * Otherwise, local always lost conflicts
 */

void G_FUNCTION(g_Local_Win_Conflict,
		(g_NNFS *nnfs, g_File_Info *f))

int i ;

f->conflict = g_True ;
f->hosts[nnfs->header.fs_id].up_to_date = g_True ;
for(i=0;i<nnfs->header.nb_fs;i++)
    if ( i != nnfs->header.fs_id )
	{
	f->hosts[i].up_to_date = g_False ;
	f->hosts[i].to_historize = g_True ;
	}
f->up_to_date = nnfs->header.fs_id ; /* 05/08/97 */
/* f->local_change = g_True ; */ /* 21/4/98 */
G_RETURN(;) ;
}

/*
 *
 */

void G_FUNCTION(g_Local_Lost_Conflict,
		(g_NNFS *nnfs, g_File_Info *f))

f->conflict = g_True ;
f->hosts[nnfs->header.fs_id].to_historize = g_True ;
f->local_change = g_False ;
G_RETURN(;) ;
}

void G_FUNCTION(g_Mark_As_Deleted,
		(g_NNFS *nnfs, g_File_Info *f))


f->conflict = g_True ;
f->to_delete = g_True ;
f->to_historize = g_True ;
f->local_to_nnfs = g_False ;
f->local_change = g_False ;
f->hosts[nnfs->header.fs_id].to_historize = g_True ;
f->hosts[nnfs->header.fs_id].type = g_Is_A_Deleted ; 
f->hosts[nnfs->header.fs_id].date = G_DATE_DELETED_FILE ;
f->hosts[nnfs->header.fs_id].size = 0 ;
f->hosts[nnfs->header.fs_id].mode = 0 ;
f->hosts[nnfs->header.fs_id].up_to_date = g_True ;

G_RETURN(;) ;
}


/*
 *
 */
g_Boolean g_Info_Different(int mode_change_sensitive,
                           g_File_On_Host *a, g_File_On_Host *b)
{
return( (g_Boolean)( a->type != b->type
		     || ( mode_change_sensitive ? (a->mode != b->mode) : 0 )
		     || a->size != b->size
		     || a->date != b->date
#ifdef ROOTNNFS
		     || a->uid  != b->uid
		     || a->gid  != b->gid
#endif
		     ) ) ;
}

/*
 *
 */

g_Boolean G_FUNCTION(g_Change_In_Child,(g_File_Info *f))

int i ;

for(i=0;i<f->nb_children;i++)
 if (f->children[i]->local_change) /* 22/4/98 was local_to_nnfs */
	G_RETURN(g_True) ;
G_RETURN(g_False) ;
}

/*
 *
 */

void G_FUNCTION( g_Update, (g_NNFS *nnfs, g_File_Info *f) )

g_File_On_Host *last,*local ;
g_File_Info *tmp ;
int i ;
g_Boolean nnfs2local ;

g_Display_Bar() ;
G_PF("%s\n",nnfs->state.name);

last    = &f->hosts[nnfs->header.fs_id] ;	   /* Last local mount */
local   = &f->hosts[nnfs->header.nb_fs] ;

/*
 * For test38
 */
for(i=0;i<f->nb_children;i++)
     if (f->children[i]->nnfs_to_local_child)
       {
          f->nnfs_to_local_child = g_True ;
          break ;
       }
/*
 * For test39
 */
for(i=0;i<f->nb_children;i++)
     if (f->children[i]->local_to_nnfs_child)
       {
          f->local_to_nnfs_child = g_True ;
          break ;
       }
/*
 *
 */

if ( last->up_to_date ) f->nnfs_filtered = g_False ; /* COMPLEX */

/* 
 * Compute "up_to_date"
 */

for(i=0;i<nnfs->header.nb_fs;i++) if ( f->hosts[i].up_to_date ) break ;
f->up_to_date_on_medium = i ;
f->up_to_date = i ;

/*
 * case of local file or -add
 */

if ( i == nnfs->header.nb_fs )
	{
	G_PF("File is only local\n") ;
	/* File is only local, and so it is up to date */
	if ( local->date == G_DATE_DELETED_FILE )
		{
		G_PRINTF("%s\n", nnfs->state.name) ;
		g_Display_File_Info(nnfs,f) ;
		G_EXIT(1) ;
		}
        f->hosts[i].up_to_date = g_True ;

	*last = *local ;
	f->up_to_date = nnfs->header.fs_id ; /* 05/08/97 */
	if ( !nnfs->options.add )
		f->local_change = g_True ; /* 14/09/97 */
	}
else
   {
   if ( nnfs->options.add )
	{
	*last = *local ;
	if ( last->type != g_Is_A_Deleted )
            {
	    if ( g_Info_Different(nnfs->config.mode_change_sensitive,last, &f->hosts[f->up_to_date] ) )
		{
		if ( last->type == g_Is_A_Directory
		     && f->hosts[f->up_to_date].type != g_Is_A_Directory )
			g_Local_Win_Conflict(nnfs,f) ;
		else
		if ( last->type != g_Is_A_Directory
		     && f->hosts[f->up_to_date].type == g_Is_A_Directory )
			{
			f->on_medium = g_True ;
			g_Local_Lost_Conflict(nnfs,f) ;
			}
		else
                   {
		   if ( last->date >= f->hosts[f->up_to_date].date )
			g_Local_Win_Conflict(nnfs,f) ;
		   else
			g_Local_Lost_Conflict(nnfs,f) ;

		   if ( last->type == g_Is_A_Directory
		     && f->hosts[f->up_to_date].type == g_Is_A_Directory )
                        f->conflict = g_False ;
                   }
		}
	      else
		last->up_to_date = g_True ;
           }
	}
   f->local_change = g_Info_Different(nnfs->config.mode_change_sensitive, last, local )
                       /* 21/4/1998 */ || g_Change_In_Child(f) ;

   /*
    * Solve test38 problems: Transform a file into a directory
    * if this directory contains files that needs to be updated
    * in the local host.
    */
   if ( f->local_change
        && local->type != g_Is_A_Directory
        && f->nnfs_to_local_child
     )
        {
            G_PF("test38 bug correction\n") ;
            f->nnfs_to_local = g_True ;
            g_Local_Lost_Conflict(nnfs,f) ;
        }
   }
/*
 *
 */
f->nnfs_change  = !last->up_to_date ;
if ( nnfs->header.nb_fs == 1 )
	f->nnfs_change = g_False ;
/*
 * In fact, nnfs_change is False only if filtered, not if errors
 */
if ( f->nnfs_filtered )
	f->nnfs_change = g_False ;

/*
 * The file need to be copied on local host ?
 */

if ( f->nnfs_change )
   {
   if ( f->local_change /* || g_Change_In_Child(f) */ ) /* 21/4/1998 Add ||CIC */
	{
	if ( local->type == last->type &&
	     last->type == g_Is_A_Directory &&
	     f->hosts[f->up_to_date].type == g_Is_A_Directory
		)
		{
   		f->nnfs_to_local = g_True ;
		f->local_change = g_False ;
		}
	else
		{
		  /* 02/11/2001 Added the "if" "else" so the local
		   * can win the conflict
		   */
			 
		  if ( f->on_medium )
		    {
			   g_Local_Lost_Conflict(nnfs,f) ;
   			   f->nnfs_to_local = g_True ;
		    }
		  else
		    {
			   g_Local_Win_Conflict(nnfs,f) ;
		    }
		}
	}
   else
   		f->nnfs_to_local = g_True ;
   }


/*
 *
 */

if (    f->nnfs_to_local
     && (    f->hosts[f->up_to_date].type == g_Is_A_Deleted
          || (    f->hosts[f->up_to_date].type != g_Is_A_Directory
               && local->type == g_Is_A_Directory
             )
        )
   )
   {
	f->to_delete = g_True ;

        /*
	 * Solve test39 test40 problem: A remote file change conflict
	 * with a local directory with the same name, but the local 
	 * directory contains changed files.
	 * Theses changed files must be not considered up to date.
	 */
        if ( f->local_to_nnfs_child )
              {
                  int size ;
                  G_PF("Correct test39 behavior\n") ;
                  tmp = g_Init_File_Info(nnfs->header.nb_fs) ;
                  size = sizeof(*tmp) + nnfs->header.nb_fs * sizeof(g_File_On_Host) ;

                  memcpy(tmp, f, size) ;
                  g_Tree_Travel1(nnfs, f, NULL, g_Mark_As_Deleted) ;
                  memcpy(f, tmp, size) ;
                  g_Free(tmp) ;
              }
   }

/*
 * Not very clean....
 */

nnfs2local = (g_Boolean)f->nnfs_to_local ;
if ( f->nnfs_to_local && f->nnfs_error
&& f->hosts[f->up_to_date].type != g_Is_A_Directory /* added 16/07/2001 */
     )
   {
      f->read_error = g_Error_Because_Nnfs_Error ;
      f->nnfs_to_local = g_False ;
   }

/*
 * Important
 */

if (    f->hosts[f->up_to_date].type != g_Is_A_Directory
      && !f->on_medium )
	f->nnfs_to_local = g_False ;

/*
 * Is the file needed elsewhere ?
 */

   f->local_to_nnfs = f->local_change ;
   if ( !f->local_to_nnfs )
	for(i=0;i<nnfs->header.nb_fs;i++)
	   {
	   if ( i == nnfs->header.fs_id )
		continue ;
	   if ( !f->hosts[i].up_to_date )
		{
		f->local_to_nnfs = g_True ;
		break ;
		}
	   }
/*
 * Case raised by read error the previous nnfs run
 */

   if ( last->up_to_date==g_False && f->nnfs_to_local==g_False &&
	!f->local_change )
	f->local_to_nnfs = g_False ;


f->to_historize = last->to_historize ;

if ( nnfs->config.historize )
        f->to_historize |= f->nnfs_to_local ;

/*
 * For test38
 */
if ( nnfs2local && f->hosts[f->up_to_date].type != g_Is_A_Deleted )
     f->nnfs_to_local_child = g_True ;
/*
 * For test39
 */

if ( f->local_to_nnfs &&  local->type != g_Is_A_Deleted )
     f->local_to_nnfs_child = g_True ;



G_RETURN(;) ;
}

