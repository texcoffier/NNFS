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

#include "g_debug.h"
#include "g_malloc.h"
#include "g_nnfs.h"
#include "g_file.h"
#include "g_trim.h"
#include "g_finish.h"
#include "g_dspbar.h"
#include "g_const.h"
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_UTIME_H
#include <utime.h>
#endif

/*
 *
 */

void g_Trim( g_File_Info *f, int nb_hosts )
{
int i, j ;

g_Display_Bar() ;
for(i=0;i<f->nb_children;i++)
	{
	g_Trim(f->children[i], nb_hosts) ;
	for(j=0;j<nb_hosts;j++)
		if ( f->children[i]->hosts[j].type != g_Is_A_Deleted ) break ;
	if ( j == nb_hosts )
		{
		g_Free_File_Info( f->children[i] ) ;
		memmove(&f->children[i],
			&f->children[i+1],
			sizeof(f->children[i])*(f->nb_children-i-1) ) ;
		f->nb_children-- ;
		i-- ;
		}
	if ( f->nb_children == 0 )
		g_Free( f->children ) ;
	}
}
