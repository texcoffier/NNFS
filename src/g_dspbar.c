/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1997  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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
#include "g_dspbar.h"
#include "g_unix.h"
#include <stdio.h>
#if HAVE_LIMITS_H
#include <limits.h>
#endif
#if HAVE_TIME_H
#include <time.h>
#endif

static int g_display_bar_current ;
static int g_display_bar_error ;
static char *title ;
/*
 *
 */

void g_Output_Display_Bar(char *string, int max)
{
char buf[G_LINE_CHUNK] ;

title = string ;
g_display_bar_error   = 0 ;
g_display_bar_current = 0 ;
if ( string )
	{
	sprintf(buf,"%d %s",max,string) ;
	g_Output(G_SET_DISPLAY_BAR,buf) ;
	fflush(stdout) ;
	}
}


void g_Display_Bar_Error()
{
g_display_bar_error++ ;
}

void g_Finish_Display_Bar()
{
char buf[G_LINE_CHUNK] ;
sprintf(buf,"%d",g_display_bar_error) ;
g_Output(G_FINISH_BAR,buf) ;
}


void g_Display_Bar()
{
static time_t last = 0 ;
time_t current ;
char buf[G_LINE_CHUNK] ;

if ( title==NULL )
	return ;

time(&current) ;
if ( current != last )
	{
	last = current ;
	sprintf(buf,"%d %d",g_display_bar_current,g_display_bar_error) ;
	g_Output(G_BAR,buf) ;
	}
g_display_bar_current++ ;
}

