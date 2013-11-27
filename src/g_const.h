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

#ifndef G_CONST_H
#define G_CONST_H

/*
 * Next constants define really a limitation
 */
#define G_HOSTNAME_LEN	256

/*
 * Special values
 */
#define G_DATE_DELETED_FILE	0	/* Value for g_File_On_Host.time */
#define G_DATE_FILTRED_FILE	1	/* Value for g_File_On_Host.time */

/*
 * Next constants are parameters, not limitations
 */
#define G_LINE_CHUNK    8192
#define G_MODE_MASK     (unsigned int)07777	/* Take 12 bit of file mode */
#define ERRNO_BITS	16

/*
 * File separator
 */
#define G_STRING_SEPARATOR	"/"
#define G_CHAR_SEPARATOR        '/'
#define G_CHAR_VERSION_SEPARATOR ':'
/*
 * Some file names
 */
#define G_NNFS_DIR          "%" G_STRING_SEPARATOR ".nnfs"
#define G_NNFS_NNFSRC	    "%" G_STRING_SEPARATOR "nnfsrc"
#define G_NNFS_LOG	    "%" G_STRING_SEPARATOR "log~"
#define G_NNFS_FILTERED_LOG "%" G_STRING_SEPARATOR "filtered.log~"

/*
 *
 */

#ifndef MINIMUM
#define MINIMUM(A,B) ( (A)<(B) ? (A) : (B) )
#endif

#define G_MALLOC(X,N) ( (X) = g_Malloc( sizeof(*(X)) * N ) )
#define G_REALLOC(X,N) ( (X) = g_Realloc( X, sizeof(*(X)) * N ) )
#define ERRNO_MAX  (1<<(ERRNO_BITS-1))
#define ERROR_NUMBER ((errno>ERRNO_MAX || errno<0) ? ERRNO_MAX-1 : errno)

#ifdef G_FILE
#undef G_FILE
#endif

#define G_SET_DISPLAY_BAR     'S'
#define G_BAR		      'B'
#define G_FINISH_BAR	      '$'
#define G_HOST		      'H'
#define G_NB_MOUNT	      'N'
#define G_DIRECTORY	      'D'
#define G_LAST_MOUNT	      'L'
#define G_ALIAS		      'A'
#define G_HOST_DONE	      '.'
#define G_CURRENT_HOST	      'C'
#define G_STATS	  	      'R'
#define G_FILE  	      'F'
#define G_QUESTION  	      '?'
#define G_NNFSRC_CREATED      '1'
#define G_NNFSRC_NOT_CREATED  '2'
#define G_FORMAT              'f'
#define G_CONTINUE            'c'
#define G_HOST_NOT_IN_NNFS    '3'
#define G_ADD_HOST            '4'
#define G_ADD_NICKNAME        '5'
#define G_ABORT_READING       'a'
#define G_CAN_NOT_WRITE       'W'
#define G_FLUSHING_DATA       '|'

/*
 * From "autoconf" info file :
 */

#if STDC_HEADERS
# include <string.h>
#else
# ifndef HAVE_STRCHR
#  define strchr index
#  define strrchr rindex
# endif
char *strchr (), *strrchr ();
# ifndef HAVE_MEMCPY
#  define memcpy(d, s, n) bcopy ((s), (d), (n))
#  define memmove(d, s, n) bcopy ((s), (d), (n))
# endif
#endif

#include <sys/types.h>
#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#endif
