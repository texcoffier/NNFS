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
#ifdef G_DEBUG

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdio.h>
#ifndef FILENAMEmax
#define FILENAMEmax 8192
#endif

#define G_MAX_FCT_STACK (1<<10)		/* Must power of 2 */

struct g_debugging
	{
	char *name ;	/* Function name */
	int verbose ;	/* The verbose level */
	int input ;	/* Test input level */
	int output ;	/* Test output level */
	int internal ;	/* Test internal level */
	int info ;	/* Information for user */
	int start ;	/* Print starting information */
	int end ;	/* Print ending information */
	unsigned long nb ; /* Number of call */
	int nb_in ;	/* Number of recursive call */
	char *starting ;/* Function to call a start of this one */
	char *ending ;  /* Function to call a end of this one */
	int (*itself)() ; /* Pointer to me */
	} ;

typedef struct g_debugging g_Debugging ;

g_Debugging*    g_Init_Function( const char* name, int (*)() ) ;

extern int g_debug_level ;
extern g_Debugging *g_function_stack[G_MAX_FCT_STACK] ;


#define G_NAME g_fct_info->name

#define G_PRINTF g_Indent(), \
		 g_Printf("%s:%d:%s ",__FILE__,__LINE__,G_NAME), \
		 g_Printf

#define G_FUNCTION(NAME,ARGS) \
   NAME ARGS \
   { \
   static g_Debugging *g_fct_info = 0 ; \
   int (*g_sten_)() ; \
   int unused_variable = ( \
	   (g_fct_info==0 ? \
		g_fct_info = g_Init_Function(#NAME, \
					(int (*)())NAME) : \
		0 \
	   ), \
	   (g_fct_info->start!=0 ? \
		G_PRINTF("START"#ARGS"\n") : \
		unused_variable \
	   ), \
	   g_fct_info->nb++, \
	   g_fct_info->nb_in++, \
	   g_function_stack[g_debug_level&(G_MAX_FCT_STACK-1)] = g_fct_info, \
	   g_debug_level++, \
	   g_fct_info->starting[0]!=0 && strcmp(g_fct_info->starting, #NAME)!=0 &&  strcmp(g_fct_info->ending, #NAME)!=0  ? \
		( \
		(g_sten_ = g_Init_Function(g_fct_info->starting,0)->itself),\
		(g_sten_ ? (*g_sten_)() : 0) \
		) : 0 \
	 ) ; 

   /* The do while(0) must be here, it is to avoid warning */

#define G_RETURN(X) \
		 do { if ( g_fct_info->ending[0]!=0 && strcmp(g_fct_info->starting, g_fct_info->name)!=0 &&  strcmp(g_fct_info->ending, g_fct_info->name)!=0 ) \
		    { \
		    g_sten_ = g_Init_Function(g_fct_info->ending,0)->itself ; \
		    if ( g_sten_ ) (*g_sten_)() ; \
		    } \
		 g_fct_info->nb_in-- ; \
		 g_debug_level-- ; \
		 if ( g_fct_info->end!= 0 ) G_PRINTF("END\n") ; \
		    return X ; } while(0)

#define IF(B) if ( (B) ? G_PRINTF("IF("#B")\n") : 0 )

#define G_D(X) X

#define G_VERBOSE (g_fct_info->verbose)
#define G_START (g_fct_info->start)
#define G_END (g_fct_info->end)
#define G_INPUT (g_fct_info->input)
#define G_OUTPUT (g_fct_info->output)
#define G_INFO (g_fct_info->info)
#define G_INTERNAL (g_fct_info->internal)
#define G_PF if(G_VERBOSE) G_PRINTF

#define G_EXIT(V) { G_PRINTF("EXIT\n") ; g_Exit() ; exit(V) ; }
#else

#define G_FUNCTION(NAME,ARGS) NAME ARGS {
#define G_NAME "BuGeD"
#define G_PRINTF	g_Printf
#define IF(B) if (0)
#define G_PF if(0) g_Printf
#define G_D(X)
#define G_RETURN(X)	return X 
#define G_VERBOSE	0
#define G_START		0
#define G_END		0
#define G_INPUT		0
#define G_OUTPUT	0
#define G_INFO		0
#define G_INTERNAL	0
#define G_EXIT(V) exit(V)
#define g_Exit(V) { G_PRINTF("EXIT\n") ; exit(V) ; }

/* To not call the debugging functiond */
#define g_Init_Function(A,B)
#define g_Debug_List() 
#define g_Set_Debug_Level(A)
#define g_Is_Debug_Clean() 1
#define g_Indent()
#define g_Interpret_Debug_Args(A,B)

#endif

