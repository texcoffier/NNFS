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

#include "g_report.h"
#include "g_nnfs.h"
#include "g_type.h"
#include "g_malloc.h"
#include "g_debug.h"
#include "g_const.h"

/*
 *
 */

struct
   {
   g_Nb nb_cols ;
   g_Nb nb_lines ;
   g_Nb size ;
   struct
	{
	char *text ;
	g_Nb current_line ;
	} *cols ;
   } screen ;

void G_FUNCTION(g_Set_Report,(int lines, int columns, int nb_col))

int i, j ;

screen.nb_lines = lines ;
screen.nb_cols  = nb_col ;
screen.size = columns/nb_col ;
G_MALLOC(screen.cols, nb_col) ;

for(i=0;i<nb_col;i++)
   {
   G_MALLOC(screen.cols[i].text, lines * screen.size ) ;
   screen.cols[i].current_line = 0 ;
   for(j=0;j<lines;j++)
	sprintf(&screen.cols[i].text[j*screen.size],
		 "%*s",
		 (int)(screen.size-1),
		 "") ;
   }
G_RETURN(;) ;
}

void G_FUNCTION(g_Real_Report,(char *name, int col, int err))

int i, j ;
char *text ;

G_PF("%s in col %d\n", name, col) ;
if ( screen.cols[col].current_line == screen.nb_lines )
	G_RETURN(;) ;

text = & screen.cols[col].text[ screen.cols[col].current_line * screen.size ] ;
if ( screen.cols[col].current_line == screen.nb_lines - 1 )
	{
	for(i=0; i<screen.size-1; i++)
		text[i] = '.' ;
	screen.cols[col].current_line++ ;
	G_RETURN(;) ;
	}

text[0] = err ;
text[1] = ' ' ;
for(i=0; name[i] && i<screen.size-3; i++)
	text[i+2] = name[i] ;
if ( name[i] )
	{
        j = strlen(name) - 1 ;
	i = screen.size - 2 ;
	while(    j>=0
	       && i>=2
               && ( name[j]!=G_CHAR_SEPARATOR ||  i>(screen.size+2)/2 )
             )
		text[i--] = name[j--] ;
	
	if ( i>=4 )
		{
		 text[i--] = '.' ;
		 text[i--] = '.' ;
		 text[i--] = '.' ;
		}
	else
		{
		 text[2] = '.' ;
		 text[3] = '.' ;
		 text[4] = '.' ;
		}
	}
screen.cols[col].current_line++ ;
G_RETURN(;) ;
}

void G_FUNCTION(g_Print_Report,(FILE *f))

int i, j, k, n ;
char *s ;

n = 0 ;
for(j=0;j<screen.nb_cols;j++)
	if ( screen.cols[j].current_line > n )
		n = screen.cols[j].current_line ;
if ( n!=1 )
{
fprintf(f,"\n") ;
for(i=0;i<n;i++)
	for(j=0;j<screen.nb_cols;j++)
		{
		s = &screen.cols[j].text[screen.size*i] ;
		for(k=0;s[k];k++)
			if ( s[k]<' ' )
				s[k] = '?' ;
		fprintf(f, "%s%s", s, j==screen.nb_cols-1 ? "\n" : " ") ;
		}
fprintf(f,"\n") ;
}

G_PF("Freeing memory\n") ;

for(i=0;i<screen.nb_cols;i++)
	g_Free(screen.cols[i].text) ;
g_Free(screen.cols) ;
G_RETURN(;) ;
}

