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

#include "g_filter.h"
#include "g_file.h"
#include "g_debug.h"
#include "g_malloc.h"
#include "g_nnfs.h"
#include "g_update.h" /* For File_Info */
#if HAVE_LIMITS_H
#include <limits.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if STDC_HEADERS
#include <stdlib.h>
#endif

char *g_Jump_Separator(char *s)
{
while(*s==' '|| *s=='\t') s++ ;
return(s) ;
}

char *g_Jump_Non_Separator(char *s)
{
while(*s!=' '&& *s!='\t' && *s!='\0' && *s!='\n') s++ ;
return(s) ;
}


void G_FUNCTION(g_Print_A_Filter,(g_Filter *f))

switch(f->what)
	{
	case Addition : g_Printf("+") ; break ;
	case Subtraction : g_Printf("-") ; break ;
	default : G_EXIT(1) ;
	}
switch(f->type)
	{
	case g_Is_A_File       : g_Printf("File") ; break ;
	case g_Is_A_Directory  : g_Printf("Dir.") ; break ;
	case g_Is_A_Link       : g_Printf("Link") ; break ;
	case g_Is_A_FIFO       : g_Printf("FIFO") ; break ;
	case g_Is_Not_Supported: g_Printf("Unsu") ; break ;
	case g_Is_Any          : g_Printf("    ") ; break ;
	default : G_EXIT(1) ;
	}
if ( f->perm != -1 ) g_Printf(" %04o", f->perm) ;
		else g_Printf("     ") ;
if ( f->minus_perm != 0 ) g_Printf("-%04o", f->minus_perm) ;
		     else g_Printf("     ") ;
if ( f->plus_perm != 0xFFFFFFFF) g_Printf("+%04o", f->plus_perm) ;
		            else g_Printf("     ") ;

if ( f->minus_size != INT_MAX ) g_Printf(" -%-9d", f->minus_size) ;
		     else g_Printf("       ") ;
if ( f->plus_size != -1) g_Printf("+%-9d", f->plus_size) ;
		            else g_Printf("           ") ;
	
if ( strcmp(".*",f->regex) ) g_Printf(" %s", f->regex) ;

g_Printf("\n") ;
G_RETURN(;) ;
}

void G_FUNCTION(g_Print_Filter,(g_Filter *f))

for(;f;f=f->next)
     g_Print_A_Filter(f) ;

G_RETURN(;) ;
}


g_Filter *G_FUNCTION(g_Compile_Filter,(char *ff))

g_Filter *filter ;
char *f, *e, tmp, save_e ;
int size ;

G_MALLOC(filter,1) ;
filter->next       = NULL ;
filter->regex      = g_Save_String(".*") ;
filter->perm       = -1 ;
filter->minus_perm = 0 ;
filter->plus_perm  = ~0 ;
filter->type       = g_Is_Any ;
filter->minus_size = LONG_MAX ;
filter->plus_size  = -1 ;
filter->t_plus_size = NULL ;
filter->t_minus_size = NULL ;

if ( G_VERBOSE )
     g_Printf("[[[%s]]]\n", ff) ;
f = g_Jump_Separator(ff) ;
switch(f[0])
	{
	case '+': filter->what = Addition ; break ;
	case '-': filter->what = Subtraction ; break ;
	case '\0': g_Free_Filter(filter) ; G_RETURN(NULL) ;
	default: g_Printf("You need to start filter line by + or -\n%s",
			f) ;
		 exit(10) ;
	}

f = g_Jump_Separator(f+1) ;
while( *f!='\0' && *f!='\n' )
	{
	e = g_Jump_Non_Separator(f) ;
        save_e = *e ;
	if ( strncmp(f,"regex=",6) == 0 )
		{
		tmp = *e ;
		*e = '\0' ;
		g_Free(filter->regex) ;
                if ( strncmp(f+6, "^\\./", 4) == 0 )
                         {
                         /* Remove the "\./" for configuration compatibility */
                         filter->regex = g_Save_String(f+9) ;
                         filter->regex[0] = '^' ;
                         }
		    else
                         filter->regex = g_Save_String(f+6) ;
                G_PF("Compile %s\n", filter->regex) ;
		if( regcomp(&filter->name, filter->regex,
				REG_EXTENDED|REG_NOSUB) )
			{
			g_Printf("Bad regex : %s\n",e) ;
			regcomp(&filter->name, ".*", REG_NOSUB) ;
			}
		*e = tmp ;
		}
	else
	if ( strncmp(f,"type=",5) == 0 )
		switch(f[5])
			{
			case 'd' : filter->type = g_Is_A_Directory ; break ;
			case 'f' : filter->type = g_Is_A_File      ; break ;
			case 'l' : filter->type = g_Is_A_Link      ; break ;
			case 'p' : filter->type = g_Is_A_FIFO      ; break ;
			default  :
				g_Printf("Unknown file type : %c\n",f[5]) ;
			}
	else
	if ( strncmp(f,"perm=-",6) == 0 )
		filter->minus_perm = strtol(f+6,NULL,8) ;
	else
	if ( strncmp(f,"perm=+",6) == 0 )
		filter->plus_perm = strtol(f+6,NULL,8) ;
	else
	if ( strncmp(f,"perm=",5) == 0 )
		filter->perm = strtol(f+5,NULL,8) ;
	else
	if ( strncmp(f,"size=",5) == 0 && (f[5]=='+' || f[5]=='-') )
		{
		size = atoi(f+6) ;
		switch(e[-1])
			{
			case 'c' : break ;
			case '0' : case '1' : case '2' : case '3' : case '4' :
			case '5' : case '6' : case '7' : case '8' : case '9' :
			case 'b' : size *= 512  ; break ;
			case 'k' : size *= 1024 ; break ;
			case 'm' : size *= 1024*1024 ; break ;
			default :
				g_Printf("Filter Unknown unit for size : %c\n",
					e[-1]) ;
				g_Printf("Use c for char, b for block,"
					"k for KiloByte, m for MegaByte\n") ;
				break ;
			}
                *e = '\0' ;

		if ( f[5]=='+' )
                        {
                        filter->t_plus_size = g_Save_String(f+6) ;
			filter->plus_size = size ;
                        }
		else
                        {
                        filter->t_minus_size = g_Save_String(f+6) ;
			filter->minus_size = size ;
                        }
                *e = save_e ;
		}
	else
	if ( f[0] == '#' )
		while( *e != '\n' && *e != '\0' ) e++ ;
	else
		g_Printf("Unknown filter option : %s\n",f) ;
	f = g_Jump_Separator(e) ;
	}

if ( G_VERBOSE )
   g_Print_A_Filter(filter) ;

if ( *f=='\n' )
	filter->next = g_Compile_Filter(f+1) ;

G_RETURN(filter) ;
}

/*
 *
 */

g_Boolean G_FUNCTION(g_Eval_Filter,(char *name, g_File_On_Host *a,
			 g_Filter *filter))

g_Filter *f ;
g_Boolean filtered ;

G_PF("%s\n", name) ;
if ( a->type == g_Is_Not_Supported ) G_RETURN(g_True) ;

if ( G_VERBOSE )
    g_Display_Nnfs_File(a) ;

filtered = g_False ;
for(f=filter;f;f=f->next)
 {
 if (
       ( f->type == a->type || f->type == g_Is_Any )
    && ( f->perm == a->mode || f->perm == -1 )
    && ( (a->mode & f->minus_perm) == f->minus_perm )
    && ( (a->mode & f->plus_perm) || f->plus_perm == 0xFFFFFFFF )
    && ( a->size <= f->minus_size )
    && ( a->size >= f->plus_size )
    && ( strcmp(f->regex,".*")==0 ||
	 !regexec( &f->name, name, 0, NULL,0) )
   )
     {
     G_PF("Verified\n") ;
     if ( f->what == Addition ) filtered = g_False ;
			else filtered = g_True ;
     }
   else
     G_PF("Not Verified\n") ;
 }

G_PF("return %d\n", filtered) ;
G_RETURN(filtered) ;
}


void G_FUNCTION(g_Free_Filter,(g_Filter *f))

if ( f == NULL ) G_RETURN(;) ;
if ( f->regex )
  {
    if ( strcmp(f->regex,".*") )
      regfree(&f->name) ;
    g_Free(f->regex) ;
  }
if ( f->t_minus_size )
     g_Free(f->t_minus_size) ;
if ( f->t_plus_size )
     g_Free(f->t_plus_size) ;


g_Free_Filter(f->next) ;
g_Free(f) ;
G_RETURN(;) ;
}
