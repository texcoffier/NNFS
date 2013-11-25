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

#include "g_openp.h"
#include "g_secure.h"
#include "g_const.h"
#include "g_malloc.h"
#include "g_debug.h"
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

/*
 * The name is a FILE name.
 * So name is not a directory name
 * /a/b/c create /a and /b
 */

g_Boolean G_FUNCTION(g_Open_Path,(const char *name))

char *dir ;
int i ;
struct stat st ;

if ( lstat(name, &st) )
	{
	dir = g_Save_String(name) ;
	for(i=1;dir[i];i++)
		if ( dir[i] == G_CHAR_SEPARATOR )
			{
			dir[i] = '\0' ;
			if ( lstat(dir,&st) )
				if ( mkdir(dir,0700) )
				  {
				    g_Free(dir) ;
				    G_RETURN(g_True) ;
				  }
			dir[i] = G_CHAR_SEPARATOR ;
			}
	g_Free(dir) ;
	}
G_RETURN(g_False) ;
}

