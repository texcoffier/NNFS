/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-1997  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_state.h"
#include "g_debug.h"
#include "g_malloc.h"
#include "g_const.h"
#include <ctype.h>
#if STDC_HEADERS
#include <stdlib.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif

/*
 * Find a file name with auto-numbering
 *   boo   give boo:1 (or boo:2 if boo:1 exists and so on)
 *   boo:1 give boo:2 (or boo:3 ig boo:2 exists and so on)
 * This function is complex because numeric order is not string order
 */

char *G_FUNCTION(g_Next_File_Name,(const g_State *state))

int i, len, t ;
char *new_name, *new_full_name, *suffixe ;
int version ;
g_Boolean good ;

len = strlen(state->dir->children[state->child]->name) ;
new_name = g_Malloc(len+10) ; /* Some place to add the number */
strcpy(new_name, state->dir->children[state->child]->name) ;
for( i = len-1 ; i>=0 && isdigit((int)new_name[i]) ; ) i-- ;
if ( i==-1 || i == len-1 || new_name[i]!=G_CHAR_VERSION_SEPARATOR )
	{
	suffixe = new_name + len ; /* End of the filename */
	version = 0 ;
	}
else
	{
	suffixe = new_name + i ;
	version = atoi( &new_name[i+1] ) + 1 ;
	}
do
	{
	good = g_True ;
	sprintf(suffixe, "%c%d", G_CHAR_VERSION_SEPARATOR, version++) ;
	/* This strcmp can't be =0 */
	if ( strcmp(new_name, state->dir->children[state->child]->name)>0)
	   for(i=state->child+1;i<state->dir->nb_children;i++)
		{
		t = strcmp(new_name, state->dir->children[i]->name) ;
		if ( t == 0 ) good = g_False ;
		if ( t <= 0 ) break ;
		}
	else
	   for(i=state->child-1;i>=0;i--)
		{
		t = strcmp(new_name, state->dir->children[i]->name) ;
		if ( t == 0 ) good = g_False ;
		if ( t >= 0 ) break ;
		}
	}
while( !good ) ;

new_full_name = g_Malloc( strlen(new_name) + state->dir_len + 2 ) ;
strncpy(new_full_name, state->name, state->dir_len) ;
sprintf(new_full_name+state->dir_len, "%c%s", G_CHAR_SEPARATOR, new_name) ;
g_Free(new_name) ;

G_RETURN(new_full_name) ;
}

