/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2005  Thierry EXCOFFIER (exco@liris.univ-lyon1.fr)
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
#include "g_copy.h"
#include "g_nnfs.h"
#include "g_openp.h"
#include "g_secure.h"
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif


/*
 * Secure move
 */

g_Boolean G_FUNCTION(g_Rename,(const char *old, const char *new))

g_Boolean err ;

G_PF("%s is renamed as %s\n",old, new) ;
err = g_False ;
if ( g_Open_Path(new) )
	{
	  /* Can't create 'new' */
	  (void)remove(old) ;
	  G_RETURN(g_True) ;
	}
if ( g_rename(old,new) )
  {
    switch(errno)
      {
      case ENOENT:
	/* old does not exists: nothing to rename */
	break ;
      default:
	if ( errno != EXDEV ) /* On the same device */
	  {
	    g_Printf("Error renaming %s to %s\n",old, new) ;
	    PERROR("rename") ;
	  }
	/* Try to make a copy */
	err = g_Copy_File(old, new) ;
	if ( ! err )
	  (void)g_remove(old) ;
	break ;
      }
  }
G_RETURN(err) ;
}



char * G_FUNCTION(g_History_Dir,(const char* nnfs, const char* format))

char *d ;
time_t t ;
G_PF("Fomart = %s\n",format) ;

G_MALLOC(d,strlen(nnfs)+1+2*strlen(format)+1) ;
sprintf(d,"%s%c", nnfs, G_CHAR_SEPARATOR) ;
time(&t) ;
#if HAVE_STRFTIME
strftime( d+strlen(d), 2*strlen(format)+1, format, localtime(&t) ) ;
#else
sprintf( d+strlen(d), "%d", t) ;
#endif
G_RETURN(d) ;
}

/*
 *
 */
g_Boolean G_FUNCTION(g_Historize,(g_NNFS *nnfs, g_File_Info *f))

char *new_name ;
g_Boolean err ;

fflush(stderr) ;

err = g_False ;
if ( f->to_historize )
{
  /* 06/08/97 +2 */
  new_name = g_Formatted_Strings("%" G_STRING_SEPARATOR "%",
				 nnfs->config.history,
				 strncmp(nnfs->state.name,"./",2)
				 ? nnfs->state.name : nnfs->state.name+2) ;
  G_PF("new_name=%x (%s)\n", (int)new_name, new_name) ;
  err = g_Rename( nnfs->state.name, new_name) ;
  G_PF("new_name=%x (%s)\n", (int)new_name, new_name) ;
  g_Free(new_name) ;
  f->to_historize = g_False ;
}
else
  (void)g_remove(nnfs->state.name) ;

G_RETURN(err) ;
}

