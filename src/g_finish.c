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

#include "g_finish.h"
#include "g_debug.h"
#include "g_const.h"
#include "g_secure.h"
#include "g_malloc.h"
#include "g_filter.h"
#include "g_nnfsrc.h"

void G_FUNCTION(g_Free_File_Info,(g_File_Info *f))

int i ;
for(i=0;i<f->nb_children;i++)
	g_Free_File_Info(f->children[i]) ;
g_Free(f->name) ;
if ( f->nb_children )
	g_Free(f->children) ;
g_Free(f) ;
G_RETURN(;) ;
}


void G_FUNCTION(g_Free_Config,(g_Config *c, g_Boolean all))

int i ;
void *ptr ;

G_PF("Before Free Filtrer\n") ;
g_Free_Filter(c->output) ;

if ( all )
{
g_Free( c->nnfsrc ) ;
g_Free( c->nnfs ) ;
g_Free( c->logfile ) ;
g_Free( c->filteredlogfile ) ;
}

g_Free( c->history ) ;

for(i=0; g_Nnfsrc_Var_Table()[i].name; i++)
  {
g_Free(g_Nnfsrc_Var_Table()[i].content) ;

  ptr = (char*)c + g_Nnfsrc_Var_Table()[i].offset_in_g_config ;

  switch(g_Nnfsrc_Var_Table()[i].type )
{
    case g_Var_String:
    case g_Var_Big_String:
    case g_Var_Filter:
       g_Free(*(char**)ptr) ;
       break ;
    case g_Var_Choice:
    case g_Var_Integer:
    case g_Var_Boolean:
    break ;
    }
}

G_RETURN(;) ;

}

/*
 ******************************************************************************
 */

void G_FUNCTION(g_Finish,(g_NNFS *nnfs))

int i,j ;


g_Free_Config(&nnfs->config, g_True) ;

G_PF("Before Free host informations\n") ;
for(i=0;i<nnfs->header.nb_fs;i++)
        {
        for(j=0;nnfs->header.fs[i].nicknames[j]!=0;j++)
                g_Free( (char*)nnfs->header.fs[i].nicknames[j] ) ;
        g_Free( (char*)nnfs->header.fs[i].nicknames ) ;
	g_Free( nnfs->header.fs[i].home ) ;
	g_Free( nnfs->header.fs[i].host ) ;
        }
g_Free( (char*)nnfs->header.fs ) ;
g_Free_File_Info( nnfs->state.nnfs ) ;
g_Free( nnfs->state.name ) ;

G_RETURN(;) ;
}



