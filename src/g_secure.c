/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2005  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_secure.h"
#include "g_const.h"
#include "g_malloc.h"
#include "g_debug.h"
#include "g_unix.h"

#include <stdio.h>
#if HAVE_UNISTD_H
#include <unistd.h>	/* For chdir */
#endif
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_DIRENT_H
#include <dirent.h>
#endif
#if HAVE_STDARG_H
#include <stdarg.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

/*
 ******************************************************************************
 * Function that abort if any problem
 ******************************************************************************
 */


void g_fclose(FILE *f)
{
if ( fclose(f) )
	{
	perror("On fclose") ;
	exit(30) ;
	}
}

FILE *g_popen(const char *file, const char *mode)
{
FILE *f ;
f = popen(file,mode) ;
if ( f == NULL )
	{
	perror(file) ;
	exit(31) ;
	}
return(f) ;
}

void g_pclose(FILE *f)
{
if ( pclose(f) )
  {
    perror("On pclose") ;
    exit(32) ;
  }
}

void g_chmod(const char* nom, int mode)
{
if ( chmod(nom, mode) )
	{
	perror(nom) ;
	exit(34) ;
	}
}


void g_chdir(const char* nom)
{
if ( chdir(nom) )
	{
	perror(nom) ;
	exit(35) ;
	}
}

void g_pipe(int fildes[2])
{
if ( pipe(fildes) )
	{
	perror("Pipe open") ;
	exit(36) ;
	}
}

int g_fork()
{
int i ;
fflush(stdout) ;
fflush(stderr) ;
i = fork() ;
if ( i<0 )
	{
	perror("Fork") ;
	exit(37) ;
	}
else
  {
    /* 2/6/2001 I don't want to die on pipe writing
    if ( i==0 )
      signal(SIGPIPE, 0 ) ;
    */
  }

return(i) ;
}




void g_fprintf(FILE *f, const char *format, ...)
{
  va_list ap;

  va_start(ap, format);
  if ( vfprintf( f, format, ap ) <= 0 )
	{
	g_Printf("Format = %s\n", format) ;
	perror("On fprintf") ;
	exit(38) ;
	}
  va_end(ap);
}

/* Not portable
void g_fscanf(FILE *f, const char *format, ...)
{
  va_list ap;
  int i, nb ;

  va_start(ap, format);

  nb = 0 ;
  for(i=0;format[i]!=0;i++)
    if ( format[i]=='%' )
      {
      if ( format[i+1]!='%' && format[i+1]!='*' )
	nb++ ;
      else
	i++ ;
      }
  
  if ( vfscanf( f, format, ap ) != nb )
	{
	g_Printf("Fscanf Format = %s Nb=%d\n", format, nb) ;
	exit(1) ;
	}
  va_end(ap);
}
*/

int g_read_int(FILE *f)
{
  int v ;
  if ( fscanf(f, "%d", &v) != 1 )
    {
	g_Printf("Error reading header, exiting.\n") ;
	exit(39) ;
    }
  return(v) ;
}

int g_read_int_hexa(FILE *f)
{
  unsigned int v ;
  if ( fscanf(f, "%x", &v) != 1 )
    {
	g_Printf("Error reading header, exiting.\n") ;
	exit(40) ;
    }
  return(v) ;
}

long g_read_long(FILE *f)
{
  long v ;
  if ( fscanf(f, "%ld", &v) != 1 )
    {
	g_Printf("Error reading header, exiting.\n") ;
	exit(41) ;
    }
  return(v) ;
}

long g_read_unsigned_long(FILE *f)
{
  unsigned long v ;
  if ( fscanf(f, "%lu", &v) != 1 )
    {
	g_Printf("Error reading header, exiting.\n") ;
	exit(42) ;
    }
  return(v) ;
}

long g_read_long_hexa(FILE *f)
{
  unsigned long v ;
  if ( fscanf(f, "%lx", &v) != 1 )
    {
	g_Printf("Error reading header, exiting.\n") ;
	exit(43) ;
    }
  return(v) ;
}

/*
 * Read line with memory allocation
 */


char *g_Read_Line(FILE *f)
{
  char *start ;
  char *m ;
  char *result ;
  int  len ;
  int  total_len ;

  G_MALLOC(start, G_LINE_CHUNK ) ;
  m = start ;
  total_len = 0 ;
  for(;;)
    {
      result = fgets(m, G_LINE_CHUNK - 1, f) ;
      if ( result == NULL )
	{
	  g_Free(start) ;
	  return(NULL) ;
	}
      len = strlen( m ) ;
      total_len += len ;
      if ( m[len-1] == '\n' )
	{
	  m[len - 1] = '\0' ;
	  return( g_Realloc( start, total_len+1 ) ) ;
	}
      start = g_Realloc( start, total_len + G_LINE_CHUNK ) ;
      m = start + total_len ;
    }
}

/*
 *
 */

char *g_Formatted_Strings(const char *format, ...)
{
  va_list ap;
  char *buf, *w ;
  int len ;
  int i ;

  va_start(ap, format);

  len = strlen(format) ;
  for(i=0;format[i]!=0;i++)
    {
      if ( format[i]=='%' )
        {
	if ( format[i+1]!='%' )
	  {
	    len += strlen( va_arg(ap, char*) ) - 1 ;
	  }
	else
	  i++ ;
        }
    }
  va_end(ap);


  va_start(ap, format) ;
  G_MALLOC(buf, len+1 ) ;
  w = buf ;
  for(i=0;format[i]!=0;i++)
    {
      if ( format[i]=='%' )
        {
	if ( format[i+1]!='%' )
	  {
	    strcpy(w, va_arg(ap, char*) ) ;
	    w += strlen(w) ;
	  }
	else
	  {
	    *w++ = '%' ;
	    i++ ;
	  }
	}
      else
	*w++ = format[i] ;
    }
  *w = '\0' ;
  va_end(ap);

  return(buf) ;
}

/*
 *
 */

int g_fread(char *buffer, int t, int u, FILE *f)
{
int l ;

l = fread(buffer, t, u, f) ;
if ( l<0 )
	{
	perror("fread") ;
	exit(44) ;
	}
return(l) ;
}

/*
 *
 */

int g_read(int fildes, char *buffer, int size)
{
  if ( read(fildes, buffer, size) != size )
    {
      perror("read") ;
      exit(45) ;
    }
  return(size) ;
}


/*
 * Returns an allocated string
 */
char *g_dirname(const char *filename)
{
  char *the_dirname, *c, *last_slash ;

  the_dirname = strdup(filename) ;

  last_slash = NULL ;
  for(c = the_dirname ; *c ; c++)
    if ( *c == G_CHAR_SEPARATOR )
      last_slash = c ;
  if ( last_slash )
    last_slash[1] = '\0' ;

  return the_dirname ;
}

/*
 * These two macros allow to perform an action with
 * the directory modes modified to allow user write.
 */

#define G_DIR_TMP_WRITE(DIR, ACTION)				\
{								\
struct stat _stat_buf ;						\
if ( stat(DIR, &_stat_buf) == 0 )				\
  {								\
    if ( (_stat_buf.st_mode & 0300) != 0300 )			\
      {								\
	if ( chmod(DIR, _stat_buf.st_mode | 0300) == 0 )	\
	  {							\
	    ACTION ;						\
	    chmod(DIR, _stat_buf.st_mode) ;			\
	  }							\
      }								\
  }								\
}

#define G_TMP_WRITE(FILENAME, ACTION)		\
{						\
  char *_the_dir ;				\
  _the_dir = g_dirname(FILENAME) ;		\
  G_DIR_TMP_WRITE(_the_dir, ACTION) ;		\
  free(_the_dir) ;				\
}

static int sync_readonly_dir = 0 ;

void g_sync_readonly_dir(int bool)
{
  sync_readonly_dir = bool ;
}

/*
 *
 */

int g_symlink(const char *value, const char *name)
{
  int r ;
  r = symlink(value, name) ;
  if ( r )
    {
    perror(value);
    fprintf(stderr, "errno = %d\n", errno);
    }
  if ( r && (errno == EACCES ))
    G_TMP_WRITE(name, r = symlink(value, name)) ;
  if ( r )
    {
    perror(value);
    fprintf(stderr, "errno2 = %d\n", errno);
    }
  return r ;
}

int g_creat(const char *name, int mode)
{
  int r ;
  r = creat(name, mode) ;
  if ( r < 0 && (errno == EACCES && sync_readonly_dir))
    G_TMP_WRITE(name, r = creat(name, mode)) ;
  return r ;
}

int g_rename(const char *name, const char *new_name)
{
  int r ;
  r = rename(name, new_name) ;
  if ( r && (errno == EACCES && sync_readonly_dir))
    G_TMP_WRITE(name, r = rename(name, new_name)) ;
  return r ;
}

int g_remove(const char *name)
{
  int r ;
  r = remove(name) ;
  if ( r && (errno == EACCES && sync_readonly_dir))
    G_TMP_WRITE(name, r = remove(name)) ;
  return r ;
}

int g_rmdir(const char *name)
{
  int r ;
  r = rmdir(name) ;
  if ( r && (errno == EACCES && sync_readonly_dir))
    G_TMP_WRITE(name, r = rmdir(name)) ;
  return r ;
}

int g_mkfifo(const char *name, int mode)
{
  int r ;
  r = mkfifo(name, mode) ;
  if ( r && (errno == EACCES && sync_readonly_dir))
    G_TMP_WRITE(name, r = mkfifo(name, mode)) ;
  return r ;
}

int g_mkdir(const char *name, int mode)
{
  int r ;
  r = mkdir(name, mode) ;
  if ( r && (errno == EACCES && sync_readonly_dir))
    G_TMP_WRITE(name, r = mkdir(name, mode)) ;
  return r ;
}

FILE *g_fopen(const char *file, const char *mode)
{
FILE *f ;
f = fopen(file,mode) ;
if ( f == NULL && mode[0] == 'w' && (errno == EACCES && sync_readonly_dir) )
  G_TMP_WRITE(file, f = fopen(file,mode)) ;
/*
if ( f == NULL )
	{
	perror(file) ;
	exit(29) ;
	}
*/
return(f) ;
}
