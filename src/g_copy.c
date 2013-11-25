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

#include "g_const.h"
#include "g_debug.h"
#include "g_copy.h"
#include "g_secure.h"
#include "g_malloc.h"
#if STDC_HEADERS
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

/*
 *
 */

g_Boolean G_FUNCTION(g_Copy_Stream,(FILE *r, FILE *w, int nb))

char buf[G_LINE_CHUNK] ;
g_Boolean err ;
int j, n ;

err = g_False ;
while( nb )
	{
        j = MINIMUM(G_LINE_CHUNK, nb) ;
	n = fread(buf, 1, j , r) ;
	if ( n != j )
		{
		G_PF("%d bytes read %d expected\n",n,j) ;
		err = g_True ;
				/* break ; 7/2/1998 */
		}
	n = fwrite(buf, 1, j, w) ;
	if ( n != j )
		{
		G_PRINTF("%d bytes wrote %d expected\n",n,j) ;
		err = g_True ;
				/* break ; 7/2/1998 */
		}
	G_PF("%d bytes wrote\n",n) ;
	nb -= j ;
	}

G_RETURN(err) ;
}

/*
 *
 */

g_Boolean G_FUNCTION(g_Copy_Link,(const char *old, const char *new))

int size ;
char buf[G_LINE_CHUNK] ;

size = readlink(old, buf, G_LINE_CHUNK) ;
if ( size<0 )
	G_RETURN(g_False) ;
buf[size] = '\0' ;
if ( g_symlink(buf, new) )
      G_RETURN(g_True) ;
G_RETURN(g_False) ;
}

g_Boolean G_FUNCTION(g_Copy_Regular,(const char *old, const char *new,
		struct stat *st))

FILE *in, *out ;
int w ;
g_Boolean err ;

err = g_False ;
in = fopen(old,"r") ;
if ( in != NULL )
	{
	w = g_creat(new, st->st_mode) ;
	if ( w>=0 )
		{
		out = fdopen(w, "w") ;
		if ( out != NULL )
			{
			err = g_Copy_Stream(in, out, st->st_size) ;
			fclose(out) ;
			}
		else
			err = g_True ;
		close(w) ;
		}
	else
		err = g_True ;
	fclose(in) ;
	}
G_RETURN(err) ;
}

g_Boolean G_FUNCTION(g_Copy_File,(const char *old, const char *new))

struct stat st ;

if ( lstat(old,&st) )
	{
	G_RETURN(g_False) ;
	}

if ( S_ISLNK(st.st_mode) ) { G_RETURN(g_Copy_Link(old,new)) ; }
else
if ( S_ISREG(st.st_mode) ) G_RETURN(g_Copy_Regular(old,new,&st)) ;
else
	{
	G_PRINTF("Not supported file type : 0x%x for %s\n",
		 st.st_mode, old) ;
	G_RETURN(g_True) ;
	}
}


