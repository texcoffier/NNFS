/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-1998  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_merge.h"
#include "g_update.h"
#include "g_debug.h"
#include "g_nnfs.h"
#include "g_dir.h"
#include "g_malloc.h"
#include "g_dspbar.h"
#include "g_secure.h"
#if HAVE_UNISTD_H
#include <unistd.h>		/* For chdir */
#endif

/*
 * Insert local tree (may be only a leaf) in NNFS tree.
 */

#define MERGE_FOR(ACTION_NNFS,ACTION_BOTH,ACTION_LOCAL) \
for(i=0,j=0,nb_merged=0 ; ; nb_merged++) \
   { \
   G_PF("i=%d j=%d nb=%d nb_merged=%d\n", i, j, nb, nb_merged) ; \
   if ( i<f->nb_children ) { \
      if ( j<nb ) k = strcmp(f->children[i]->name, local[j].name) ; \
             else k = -1 ; } \
   else \
      { if ( j<nb ) k = 1 ; \
             else break ; } \
   if ( k<0 ) { ACTION_NNFS ; } \
   else \
   { if ( k>0 ) { ACTION_LOCAL ; } \
   else \
   		{ ACTION_BOTH ; } } \
   if ( k<=0 && i<f->nb_children ) i++ ; \
   if ( k>=0 && j<nb ) j++ ; \
   }


/*
 * Merge with PIPE
 */

void G_FUNCTION(g_Merge_Pipe,(g_NNFS *nnfs, g_File_Info *f, FILE* input))

  int i, j, k ;
  g_Dir *local ;
  g_File_Info **after_merge ;
  int nb, nb_merged ;
  
  g_Read_Pipe_Directory(&local, &nb, input, &nnfs->config) ;
  MERGE_FOR( {} , {} , {} ) ;

  G_PF("nb_merged = %d nb_local = %d\n", nb_merged, nb ) ;
  if ( nb_merged )
    {
      G_MALLOC(after_merge, nb_merged) ;
      G_PF("after_merge = %x nb = %d\n", after_merge, nb_merged) ;
    }
  else
      after_merge = NULL ; /* To avoid a compiler warning */

  MERGE_FOR(
	    {
	      G_PF("f %d : %s\n",nb_merged, f->children[i]->name) ;
	      after_merge[nb_merged] = f->children[i] ;
	    },
	    {
	      G_PF("f/local %d : %s\n",nb_merged, f->children[i]->name) ;
	      after_merge[nb_merged] = f->children[i] ;
	      after_merge[nb_merged]->hosts[nnfs->header.nb_fs] = local[j].info ;
	    },
	      {
		G_PF("local %d : %s\n",nb_merged, local[j].name) ;
		after_merge[nb_merged] = g_Init_File_Info(nnfs->header.nb_fs) ;
		G_PF("after_merge[] = %x\n", after_merge[nb_merged]) ;
		after_merge[nb_merged]->name  = local[j].name ;
		after_merge[nb_merged]->hosts[nnfs->header.nb_fs] = local[j].info ;
		local[j].name = NULL ;
		G_PF("end\n") ;
	      }
	    ) ;
  G_PF("Free local names\n" ) ;
  for(i=0;i<nb;i++)
    if ( local[i].name )
      g_Free( local[i].name ) ;
  G_PF("Free local\n" ) ;
  g_Free( local ) ;

  if ( f->nb_children )
    g_Free( f->children ) ;

  f->children = after_merge ;
  f->nb_children = nb_merged ;

  for(i=0;i<nb_merged;i++)
    if ( f->children[i]->hosts[nnfs->header.nb_fs].type == g_Is_A_Directory )
      g_Merge_Pipe(nnfs, f->children[i], input) ;

  G_RETURN(;) ;
}






/*
 *
 */

void G_FUNCTION( g_Merge, (g_NNFS *nnfs, g_File_Info *f, char *name))

int i, j, k ;
g_Dir *local ;
g_File_Info **after_merge ;
int nb, nb_merged ;
char *new_name ;

G_PF("name = %s\n", name ) ;
g_Read_Directory(&local, &nb, &nnfs->config) ;
MERGE_FOR( {} , {} , {} ) ;

G_PF("nb_merged = %d nb_local = %d\n", nb_merged, nb ) ;
if ( nb_merged )
	{
	G_MALLOC(after_merge, nb_merged) ;
	G_PF("after_merge = %x nb = %d\n", after_merge, nb_merged) ;
	}
else
        after_merge = NULL ;

MERGE_FOR(
	{
	G_PF("f %d : %s\n",nb_merged, f->children[i]->name) ;
	after_merge[nb_merged] = f->children[i] ;
	},
	{
	G_PF("f/local %d : %s\n",nb_merged, f->children[i]->name) ;
	after_merge[nb_merged] = f->children[i] ;
	after_merge[nb_merged]->hosts[nnfs->header.nb_fs] = local[j].info ;
	},
	{
	G_PF("local %d : %s\n",nb_merged, local[j].name) ;
	after_merge[nb_merged] = g_Init_File_Info(nnfs->header.nb_fs) ;
	G_PF("after_merge[] = %x\n", after_merge[nb_merged]) ;
	after_merge[nb_merged]->name  = local[j].name ;
        G_PF("end1\n") ;
	after_merge[nb_merged]->hosts[nnfs->header.nb_fs] = local[j].info ;
        G_PF("end2\n") ;
	local[j].name = NULL ;
        G_PF("end3\n") ;
	}
	) ;
G_PF("Free local names\n" ) ;
for(i=0;i<nb;i++)
    if ( local[i].name )
	g_Free( local[i].name ) ;
G_PF("Free local\n" ) ;
g_Free( local ) ;

if ( f->nb_children )
	g_Free( f->children ) ;

f->children = after_merge ;
f->nb_children = nb_merged ;


for(i=0;i<nb_merged;i++)
   {
   g_Display_Bar() ;
   new_name = g_Formatted_Strings("%" G_STRING_SEPARATOR "%", name, f->name);
   if ( f->children[i]->hosts[nnfs->header.nb_fs].type == g_Is_A_Directory )
     {
       if ( chdir( f->children[i]->name ) == 0 )
	 {
	   g_Merge(nnfs, f->children[i], new_name) ;
	   if ( chdir("..") )
	     if ( chdir(name) )
	       {
		 g_Free(new_name) ;
		 G_RETURN(;) ;
	       }
	 }
     }
   g_Free(new_name) ;
   }

G_RETURN(;) ;
}

