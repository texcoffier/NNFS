/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2001  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_unix.h"
#include "g_const.h"
#include "g_debug.h"
#include "g_secure.h"
#include "g_malloc.h"
#include "g_copy.h"
#include "g_split.h"
#include "g_nnfsrc.h"

#include <stdio.h>
#include <math.h>
#if STDC_HEADERS
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_SIGNAL_H
#include <signal.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_SYS_VFS_H
#include <sys/vfs.h>
#endif
#if HAVE_SYS_STATFS_H
#include <sys/statfs.h>
#endif
#if HAVE_LIMITS_H
#include <limits.h>
#endif
#if HAVE_PWD_H
#include <pwd.h>
#endif

#ifndef ULONG_MAX
#define ULONG_MAX 4294967295UL
#endif
/*
 ******************************************************************************
 * 
 ******************************************************************************
 */

#ifndef SIGUSR2
#ifdef _SIGUSR2
#define SIGUSR2 _SIGUSR2
#endif
#endif

#ifndef SIGUSR1
#ifdef _SIGUSR1
#define SIGUSR1 _SIGUSR1
#endif
#endif

#ifndef SIGPIPE
#ifdef _SIGPIPE
#define SIGPIPE _SIGPIPE
#endif
#endif

void g_Signal_Ignore(int i)
{
}

void G_FUNCTION(g_Signal,(g_Signal_Type st, void (*fct)(int)))

if ( fct == g_Signal_Ignore )
     fct = (void (*)(int)) SIG_IGN ;

switch(st)
{
 case g_Signal_User1: signal(SIGUSR1, fct) ; break ;
 case g_Signal_User2: signal(SIGUSR2, fct) ; break ;
 case g_Signal_Pipe: signal(SIGPIPE, fct) ; break ;
 case g_Signal_Background_Output:
#ifdef SIGTTOU
   signal(SIGTTOU, fct) ;
#endif
   break ;
 default:
   G_EXIT(1) ;
}

G_RETURN(;) ;
}

void G_FUNCTION(g_Kill,(int pid, g_Signal_Type st))

switch(st)
{
case g_Signal_User1: kill(pid, SIGUSR1) ; break ;
case g_Signal_Interrupt: kill(pid, SIGINT) ; break ;
case g_Signal_Terminate: kill(pid, SIGTERM) ; break ;
 default:
   G_EXIT(1) ;
}

G_RETURN(;) ;
}


/*
 ******************************************************************************
 * 
 ******************************************************************************
 */

int G_FUNCTION(g_Getuid,())

G_RETURN( (int) getuid() ) ;
}

/*
 ******************************************************************************
 * 
 ******************************************************************************
 */

int G_FUNCTION(g_Get_Columns,())

char *c ;

c = getenv("COLUMNS") ;
G_RETURN( c ? atoi(c) : 80 ) ;
}

int G_FUNCTION(g_Get_Lines,())

char *c ;

c = getenv("LINES") ;
G_RETURN( c ? atoi(c) : 25 ) ;
}

/*
 ******************************************************************************
 * Retrieve a name for the current computer
 ******************************************************************************
 */


char * G_FUNCTION(g_Get_Hostname,())

static char hn[G_HOSTNAME_LEN] ;
char *pc ;

if ( gethostname(hn,G_HOSTNAME_LEN) )
  {
  perror("gethostname") ;
  exit(52) ;
  }
strcat(hn,".") ;

#ifdef __CHECKER__
#undef HAVE_GETDOMAINNAME
#endif


#ifdef HAVE_GETDOMAINNAME
pc = hn + strlen(hn) ;
getdomainname( pc, G_HOSTNAME_LEN - (pc-hn) ) ;
if ( strcmp(pc, "(none)") == 0 )
     pc[0] = '\0' ;
#else
strcat( hn,"NO_GETDOMAINNAME" ) ;
#endif

strcat(hn,".") ;
pc = getpwuid(geteuid())->pw_name ;
 
if ( pc )
     strcat(hn, pc) ;


G_RETURN(hn) ;
}

/*
 ******************************************************************************
 * Retrieve free size on filesystem
 ******************************************************************************
 */

#ifndef HAVE_F_BAVAIL
#define f_bavail f_bfree
#endif

g_Nb G_FUNCTION(g_Get_Free_Size,(const char *file))

#if HAVE_STATFS2 || HAVE_STATFS3

struct statfs sfs ;

if (
#if HAVE_STATFS2
 statfs(file, &sfs)
#else
 statfs(file, &sfs, sizeof(sfs), 0)
#endif
 )
{
perror(file) ;
G_EXIT(1) ;
}

if ( sfs.f_bfree < 0 )
     G_RETURN(0) ;

if ( sfs.f_bsize * (float)sfs.f_bavail > pow(2, sizeof(g_Nb)*8) )
     G_RETURN(ULONG_MAX) ;


G_RETURN( sfs.f_bsize * sfs.f_bavail ) ;

#else

G_RETURN(ULONG_MAX) ;

#endif
}


/*
 ******************************************************************************
 * Retrieve a name of the "configuration file"
 ******************************************************************************
 */

char * G_FUNCTION(g_Nnfs_Dir,())

char *home ;

home = getenv("HOME") ;
if ( home==0 )
        {
        g_Printf("NNFS: HOME shell variable isn't set!\n") ;
        g_Printf("NNFS: Please, ask why to your UNIX guru.\n") ;
        G_EXIT(1) ;
        }

home = g_Formatted_Strings( G_NNFS_DIR, home ) ;
mkdir( home, 0700) ;
G_RETURN(home) ;
}

char * G_FUNCTION(g_Nnfsrc,(char *home))

G_RETURN( g_Formatted_Strings( G_NNFS_NNFSRC, home ) ) ;
}

char * G_FUNCTION(g_LogFile,(char *home))

G_RETURN( g_Formatted_Strings( G_NNFS_LOG, home ) ) ;
}

char * G_FUNCTION(g_FilteredLogFile,(char *home))

G_RETURN( g_Formatted_Strings( G_NNFS_FILTERED_LOG, home ) ) ;
}



/*
 ******************************************************************************
 * Retrieve the unix configuration
 ******************************************************************************
 */

void G_FUNCTION(g_Get_Var, (FILE *f, char** var, const char *name))

int i, l ;
char tmp[9999] ;

for(i=0;i<sizeof(tmp);i++)
{
  l = fread( &tmp[i], 1, 1, f ) ;
  if ( l != 1 )
   {
     G_PRINTF("i=%d l=%d\n",i,l) ;
     G_EXIT(1) ;
   }
  if ( i>=4
  && tmp[i-4] == '\005'
  && tmp[i-3] == '\030'
  && tmp[i-2] == '\003'
  && tmp[i-1] == '\017'
  && tmp[i-0] == '\n'
  )
    {
      tmp[i-4] = '\0' ;
      *var = g_Save_String(tmp) ;
      if ( i == 0 )
	{
 	g_Printf("NNFS: Warning, you should initialise %s in nnfsrc\n", name) ;
        }
      G_PF("read %s : [[[%s]]]\n",name, *var) ;
      G_RETURN(;) ;
    }
}

G_EXIT(2) ;
}

/*
 *
 */

void G_FUNCTION(g_Read_Config_Char,(char *nnfsrc_name,g_Nnfsrc_Var *v, const char *hostname))

char *buf, *buf2 ;
int i ;
FILE *f ;


if ( hostname && hostname[0] )
    buf = g_Formatted_Strings("export NNFS2_HOSTNAME=%\n",  hostname) ;
else
    buf = g_Save_String("") ;

buf2 = g_Formatted_Strings("%. % ; cat <<_EOT_exco_EOT_\n", buf, nnfsrc_name) ;
g_Free(buf) ;

for( i=0 ; v[i].name ; i++) 
  {
  buf = g_Formatted_Strings("%${%}\005\030\003\017\n", buf2, v[i].name) ;
  g_Free(buf2) ;
  buf2 = buf ;
  }
buf2 = g_Formatted_Strings("%_EOT_exco_EOT_\n", buf) ;
g_Free(buf) ;
G_PF("pipe = %s\n", buf2) ;

f = popen( buf2, "r") ;
g_Free(buf2) ;

for( i=0 ; v[i].name ; i++) 
{
  if ( v[i].content )
    g_Free(v[i].content) ;
  g_Get_Var(f, &v[i].content, v[i].name) ;
}

G_PF("Before\n") ;
g_pclose(f) ;
G_PF("After\n") ;

G_RETURN(;) ;

}


/*
 *
 */

FILE* G_FUNCTION(g_Open_Input_File,(const g_Config *config, int medium,
int *pid_unsplit))

int fildes[2] ;
int uncompress[2] ;
int pid_uncompress, status ;
FILE *w ;
g_Boolean multiple ;

g_pipe(fildes) ;
*pid_unsplit = g_fork() ;
if ( *pid_unsplit==0 )
	{
	G_PF("In unsplit son!\n") ;
	close(fildes[0]) ;
	g_pipe(uncompress) ;
	pid_uncompress = g_fork() ;
	G_PF("PID uncompress = %d\n", pid_uncompress) ;
	if ( pid_uncompress==0 )
		{
                G_PF("In the uncompress child\n") ;
		close(uncompress[1]) ;
		dup2(fildes[1],1) ;
		dup2(uncompress[0],0) ;
		G_PF("uncompress %s\n", config->uncompress_filter) ;
		exit( system(config->uncompress_filter)!=0 ) ;
		}
        G_PF("In the split_input parent\n") ;
	close(uncompress[0]) ;
	w = fdopen(uncompress[1],"a") ;
	multiple = g_Split_Input(w, config, medium) ;
	g_fclose(w) ;
        /* ensure all data sent */
/*
        if ( close(uncompress[1]) )
             G_EXIT(1) ;
*/
	G_PF("multiple=%d Wait end of uncompress\n", multiple) ;
/*
 * The following line was here for a long time
 * Why it worked with this line ?
 * Sometime It kills the uncompress before the job is done
 */
/*  g_Kill(pid_uncompress, g_Signal_Terminate) ; */
	waitpid(pid_uncompress,&status,WUNTRACED) ;
	G_PF("End of uncompress pid=%d status=%d\n", pid_uncompress, WEXITSTATUS(status)) ;
        if ( WEXITSTATUS(status)!=0 )
	  exit(2) ;
	exit(multiple) ;
	}
G_PF("In unsplit father!\n") ;
close(fildes[1]) ;
G_PF("PID unsplit = %d\n", *pid_unsplit) ;

G_RETURN( fdopen(fildes[0], "r") ) ;
}

/*
 *
 */

FILE* G_FUNCTION(g_Open_Output_File,(const g_Config *config, g_Boolean multiple, g_Boolean format, int medium))

int fildes[2] ;
int compress[2] ;
int pid_split, pid_compress, status ;
FILE *f ;

g_pipe(fildes) ;
pid_split = g_fork() ;
if ( pid_split==0 )
	{
	G_PF("In split son!\n") ;
	close(fildes[1]) ;
	g_pipe(compress) ;
	pid_compress = g_fork() ;
	if ( pid_compress==0 )
		{
		close(compress[0]) ;
		dup2(fildes[0],0) ;
		dup2(compress[1],1) ;
		exit( system(config->compress_filter)!=0 ) ;
		}
	G_PF("PID compress = %d\n", pid_compress) ;
	close(compress[1]) ;
        f = fdopen(compress[0],"r") ;
	g_Split_Output( f, config, multiple, format, medium) ;
        g_fclose(f) ;
	G_PF("Wait end of compress\n") ;
	waitpid(pid_compress,&status,WUNTRACED) ;
	G_PF("End of compress status=%d\n", WEXITSTATUS(status)) ;
	exit(WEXITSTATUS(status)) ;
	}
G_PF("In split father!\n") ;
close(fildes[0]) ;
G_PF("PID split = %d\n", pid_split) ;

G_RETURN( fdopen(fildes[1],"a") ) ;
}

/*
 * Just to find bugs
 */

void g_exit_verbose(int v)
{
  char c[99] ;

  g_Printf("PID %d Children of %d\n", getpid(), getppid()) ;

  sprintf(c, "cat /proc/%d/status | fgrep kB >&2", getpid()) ;
  system(c) ;
  _exit(v) ;
}
