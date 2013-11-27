/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-1998  Thierry EXCOFFIER (exco@ligia.univ-lyon1.fr)
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
#include "g_unix.h"
#include <stdio.h>
#if HAVE_STDARG_H
#include <stdarg.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>

/* This variable is changed by ALL function at each CALL/RETURN
 * so it must be global.
 * It indication the depth in the stack of debugged function call
 */
int g_debug_level = 0 ;

/* Thank's to Brian V. SMITH for this function */
int g_Printf(const char *format, ...)
{
    va_list ap;
    int i ;

    va_start(ap, format);
    i = vfprintf( stderr, format, ap );
    va_end(ap);
    return(i) ;
}


#ifdef G_DEBUG

static g_Debugging **table=0 ;
static int nb=0 ;
static int max_nb=0 ;

g_Debugging *g_function_stack[G_MAX_FCT_STACK] ;

g_Debugging* g_Init_Function ( const char *name, int (*itself)() )
{
int i ;
static g_Debugging *defaults = 0 ;

for(i=0;i<nb;i++)
	{
	if ( strcmp( name , table[i]->name ) == 0 )
		{
		if ( table[i]->itself==0 )
		     table[i]->itself = itself ;
		return( table[i] ) ;
		}
	}
if ( nb>=max_nb )
	{
	max_nb = 16 + 2*max_nb ;
	table = (g_Debugging**) g_Realloc( (char*)table ,
					   max_nb*sizeof(*table) ) ;
	}

nb++ ;
table[i] = (g_Debugging*) g_Malloc( sizeof(*table[i]) ) ;
table[i]->name     = g_Save_String(name) ;
table[i]->nb       = 0 ;
table[i]->nb_in    = 0 ;

if ( strcmp("*",name)==0 )
	{
	table[i]->verbose  = 0 ;
	table[i]->input    = 0 ;
	table[i]->output   = 0 ;
	table[i]->internal = 0 ;
	table[i]->info     = 0 ;
	table[i]->start    = 0 ;
	table[i]->end      = 0 ;
	table[i]->ending   = "" ;
	table[i]->starting = "" ;
	table[i]->itself   = itself ;
	}
else
	{
	if ( defaults==0 ) defaults = g_Init_Function("*",0) ;
	table[i]->verbose  = defaults->verbose ;
	table[i]->input    = defaults->input ;
	table[i]->output   = defaults->output ;
	table[i]->internal = defaults->internal ;
	table[i]->info     = defaults->info ;
	table[i]->start    = defaults->start ;
	table[i]->end      = defaults->end ;
	table[i]->ending   = defaults->ending[0] ? g_Save_String(defaults->ending): "";
	table[i]->starting = defaults->starting[0] ? g_Save_String(defaults->starting): "" ;
	table[i]->itself   = itself ;
	}

return( table[i] ) ;
}


void g_Debug_List()
{
int i,j ;

g_Printf("g_Debug_List\n") ;
g_Printf("nb=number of function call\n") ;
g_Printf("Levels for verb=verbose, in=input, out=output int=internal\n") ;
g_Printf("           inf=info, strt=start end=end in=nb_call-nb_return\n") ;
g_Printf("           starting/ending call= pointer to the function hook\n") ;
g_Printf("           the function pointer is the function itself\n") ;
g_Printf("\n") ;

j = 0 ;
for(i=0;i<nb;i++)
	{
	g_Printf("%20s "    , table[i]->name) ;
	g_Printf("nb=%-7lu ", table[i]->nb) ;
	g_Printf("verb %d " , table[i]->verbose) ;
	g_Printf("in %d "   , table[i]->input) ;
	g_Printf("out %d "  , table[i]->output) ;
	g_Printf("int %d "  , table[i]->internal) ;
	g_Printf("inf %d "  , table[i]->info) ;
	g_Printf("strt %d " , table[i]->start) ;
	g_Printf("end %d "  , table[i]->end) ;
	g_Printf("in %d\n"  , table[i]->nb_in) ;
	g_Printf("startingcall (%s) endingcall (%s) itself %d\n",
				table[i]->starting,
				table[i]->ending,
				(int)table[i]->itself) ;
	}

if ( j==1 )
	{
	g_Printf("\n\nActive function or G_RETURN forgotten in:\n") ;
	for(i=0;i<nb;i++)
		{
		if ( table[i]->nb_in != 0 )
			g_Printf("	%s\n",table[i]->name) ;
		}
	g_Printf("\n\n") ;
	}
}

void g_Exit()
{
int i ;

g_Printf("Stack on G_EXIT:\n") ;
for(i=0;i<g_debug_level;i++)
	{
	g_Printf("\t%s\n",g_function_stack[i]->name) ;
	}
}

g_Boolean g_Is_Debug_Clean()
{
int i ;

for(i=0;i<nb;i++)
	{
	if ( table[i]->nb_in != 0 && strcmp(table[i]->name,"main")!=0 )
		{
		g_Printf("Debugging code is bg (missing G_RETURN)\n") ;
		g_Debug_List() ;
		return(g_False) ;
		}
        }
for(i=0;i<nb;i++)
	{
	g_Free( table[i]->name ) ;
	if ( table[i]->starting[0] )
	  {
	    g_Free(table[i]->starting) ;
	  }
	if ( table[i]->ending[0] )
	  {
	    g_Free(table[i]->ending) ;
	  }
	g_Free( (char*)table[i] ) ;
	}
g_Free( (char*)table ) ;
nb = 0 ;
max_nb = 0 ;
return(g_True) ;
}

/* This function parse something like : g_Debug_List#input#4 */

g_Result G_FUNCTION( g_Set_Debug_Level, ( const char* value ) )

char f_name[FILENAMEmax] ;
char type[FILENAMEmax] ;
char startend[FILENAMEmax] ;
int val ;
g_Debugging *d ;

IF ( G_VERBOSE>=1 ) G_PRINTF("Analyse : %s\n",value) ;
if ( sscanf(value,"%79[^#]#%79[^#]#%79s", f_name , type , startend ) != 3 )
	{
	IF ( G_VERBOSE>=1 ) G_PRINTF("Bad format\n") ;
	G_RETURN(g_Error) ;
	}
val = atoi(startend) ;

IF ( G_VERBOSE>=1 ) G_PRINTF("name=%s type=%s value=%d\n",
				f_name,type,val) ;
d = g_Init_Function( f_name,0 ) ;

     if ( strcmp(type,"input")    == 0 ) d->input    = val ;
else if ( strcmp(type,"output")   == 0 ) d->output   = val ;
else if ( strcmp(type,"internal") == 0 ) d->internal = val ;
else if ( strcmp(type,"start")    == 0 ) d->start    = val ;
else if ( strcmp(type,"starting") == 0 ) d->starting = g_Save_String(startend) ;
else if ( strcmp(type,"ending")   == 0 ) d->ending   = g_Save_String(startend) ;
else if ( strcmp(type,"end")      == 0 ) d->end      = val ;
else if ( strcmp(type,"verbose")  == 0 ) d->verbose  = val ;
else if ( strcmp(type,"info")     == 0 ) d->info     = val ;
else if ( strcmp(type,"*")        == 0 )
	d->input = d->output = d->internal = d->start = d->end =
	d->verbose = d->info = val ;
else
	{
	g_Printf("Invalide syntaxe : %s\n",value ) ;
	g_Printf("Valid : input,output,verbose,internal,info,start,end\n" ) ;
	}

G_RETURN(g_Ok) ;
}


void G_FUNCTION(g_Read_Debug_Args_From_File,(int unused))

char filename[FILENAMEmax] ;
char activity[FILENAMEmax] ;
FILE *f ;

g_Signal(g_Signal_User2,g_Read_Debug_Args_From_File) ;

sprintf(filename,"/tmp/g_debug.%d",(int)getpid()) ;
f = fopen(filename,"r") ;
IF ( f == NULL )
	{
	PERROR("g_Read_Debug") ;
	G_RETURN(;) ;
	}
while( fgets(activity,FILENAMEmax,f)!=0 )
	{
	activity[strlen(activity)-1] = '\0' ; /* Remove the final \n */
	g_Set_Debug_Level(activity) ;
	}
fclose(f) ;
G_RETURN(;) ;
}



void G_FUNCTION( g_Interpret_Debug_Args,(int *argc, char**argv ) )

int i,j ;

g_Signal(g_Signal_User2,g_Read_Debug_Args_From_File) ;

for( i=1 ; i<*argc ; i++ )
   {
   IF ( G_VERBOSE>=1 ) G_PRINTF("Analyse : %s\n",argv[i]) ;
   if ( g_Set_Debug_Level( argv[i] )==g_Ok )
	{
   	IF ( G_VERBOSE>=1 ) G_PRINTF("It was a valid debug option\n") ;
	for(j=i+1;j<*argc;j++) argv[j-1] = argv[j] ;
        i-- ;
	(*argc)-- ;
	}

   }
G_RETURN(;) ;
}


void g_Indent()
{
#define SPACE "                                                               "
static int len=0 ;
int i ;

if ( len==0 ) len = strlen(SPACE) ;
for( i=g_debug_level ; i>=len ; i-=len ) fputs(SPACE,stderr) ;
fputs( SPACE+len-i,stderr ) ;
}

#endif
