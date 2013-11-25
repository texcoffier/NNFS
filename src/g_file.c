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

#include "g_file.h"
#include "g_debug.h"
#include "g_malloc.h"
#include "g_secure.h"
#include "g_const.h"
#include "g_merge.h"
#include "g_dspbar.h"
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

/*
 *
 */

g_File_Info* G_FUNCTION(g_Init_File_Info,(int nb_hosts))

g_File_Info *local ;
int size ;

/* It is "nb_hosts+1" because there is the value for current host */
local = 0 ;
size = (char*)&local->hosts[nb_hosts+1] - (char*)local ;
local = g_Malloc( size ) ;
memset(local, 0, size) ;

G_RETURN(local) ;
}

/*
 *
 */
char *g_file_info_mode(g_File_Info *f, int host)
{
int m ;
static char mode[11]  ;

  switch(f->hosts[host].type)
    {
    case g_Is_A_File       : mode[0] = '-' ; break ;
    case g_Is_A_Link       : mode[0] = 'l' ; break ;
    case g_Is_A_Directory  : mode[0] = 'd' ; break ;
    case g_Is_A_FIFO       : mode[0] = 'p' ; break ;
    case g_Is_A_Deleted    : mode[0] = 'D' ; break ;
    case g_Is_Not_Supported: mode[0] = 'U' ; break ;
    }
  m = f->hosts[host].mode ;
  mode[1] = (m&0400) ? 'r' : '-' ;
  mode[2] = (m&0200) ? 'w' : '-' ;
  mode[3] = (m&0100) ?
    ( (m&04000) ? 's' : 'x' ) :
    ( (m&04000) ? 'S' : '-' ) ;
  mode[4] = (m&040) ? 'r' : '-' ;
  mode[5] = (m&020) ? 'w' : '-' ;
  mode[6] = (m&010) ?
    ( (m&02000) ? 's' : 'x' ) :
    ( (m&02000) ? 'S' : '-' ) ;
  mode[7] = (m&04) ? 'r' : '-' ;
  mode[8] = (m&02) ? 'w' : '-' ;
  mode[9] = (m&01) ?
    ( (m&01000) ? 't' : 'x' ) :
    ( (m&01000) ? 'T' : '-' ) ;
  mode[10] = '\0' ;
  return mode ;
}


char *g_other_file_info(int nb_fs, g_File_Info *f, int tab)
{
int i ;
static char buf[G_LINE_CHUNK]  ;
char *pc ;
char *mode ;

pc = buf ;
for(i=0;i<=nb_fs;i++)
{
  mode = g_file_info_mode(f, i) ;
#ifdef ROOTNNFS
  sprintf(pc, "%s%s %5d %5d %9ld %s",
	  tab ? "\t" : "",
	  mode, f->hosts[i].uid, f->hosts[i].gid,
	  (long)f->hosts[i].size, ctime(&f->hosts[i].date)) ;
#else
  sprintf(pc, "%s%s %9ld %s",
	  tab ? "\t" : "",
	  mode,
	  (long)f->hosts[i].size, ctime(&f->hosts[i].date)) ;
#endif
  pc += strlen(pc) ;
}
return(buf) ;
}

/*
 * Read a string terminated by a '\0'
 * We do so to add the possibility of file name with any characters
 */

char *g_Read_Name(FILE *f)
{
char name[G_LINE_CHUNK] ;
int i, c ;

i = 0 ;
do
	{
	c = getc(f) ;
	if ( c == EOF ) return(NULL) ;
	name[i] = c ;
        if ( i == G_LINE_CHUNK - 2 )
           {
             g_Printf("NNFS: Filename too long\n") ;
             name[i+1] = '\0' ;
             g_Printf("NNFS: %s\n",name) ;
             return(NULL) ;
           }
	}
while( name[i++] ) ;

return( g_Save_String(name) ) ;
}

/*
 *
 */

g_File_Info* G_FUNCTION(g_Read_Nnfs_File, (FILE* f, int nb_hosts, int add))

int i, j ;
g_File_Info *info ;

g_Display_Bar() ;

info = g_Init_File_Info(nb_hosts) ;
info->name = g_Read_Name(f) ;
if ( info->name == NULL )
	{
	info->name = g_Save_String(".") ;
	IF ( nb_hosts != 1 ) G_EXIT(1) ;
	G_RETURN(info) ;
	}
G_PF("name = %s\n", info->name) ;

info->on_medium = g_read_int(f) ;
j = g_read_int(f) ;
info->nnfs_filtered = j&1 ;
info->nnfs_error    = j/2 ;
info->nb_children = g_read_int_hexa(f) ;
for(i=0;i<nb_hosts-add;i++)
	{
          info->hosts[i].type         = g_read_int(f) ;
          info->hosts[i].mode         = g_read_int_hexa(f) ;
          info->hosts[i].date         = g_read_long_hexa(f) ;
          info->hosts[i].size         = g_read_long_hexa(f) ;
          info->hosts[i].up_to_date   = g_read_int(f) ;
          info->hosts[i].to_historize = g_read_int(f) ;
#ifdef ROOTNNFS
          info->hosts[i].uid          = g_read_int_hexa(f) ;
          info->hosts[i].gid          = g_read_int_hexa(f) ;
#endif
	getc(f) ; /* YES, not in the scanf */
	}

if ( info->nb_children )
	{
	G_MALLOC(info->children, info->nb_children ) ;
	for(i=0;i<info->nb_children;i++)
		{
		info->children[i] = g_Read_Nnfs_File(f, nb_hosts, add) ;
		IF ( info->children[i] == NULL ) G_EXIT(1) ;
		}
	}
G_RETURN(info) ;
}
