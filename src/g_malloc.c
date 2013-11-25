/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2001  Thierry EXCOFFIER (exco@ligia.univ-lyon1.fr)
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "g_malloc.h"
#include "g_debug.h"

static void ** global_pointers = NULL ;
static int global_first_free = -1 ; /* Indices in global_pointers */
static int global_pointers_size = 0 ;

#define PF if (0) g_Printf

#define Index int

static int the_index(void *p)
{
  PF("the_index of %p\n", p) ;
  return(((Index*)p)[-1]) ;
}

static void is_valid(void *p)
{
  Index i ;

  PF("is valid : %p\n", p) ;
  i = the_index(p) ;
  PF("i = %d\n", i) ;
  if ( i<0
       || i>=global_pointers_size
       || ( global_pointers[i] != p )
       )
    {
      g_Printf("Invalid pointer: %p\n", p) ;
      exit(18) ;
    }
  PF("OK\n") ;
}

static void reset_index(void *p)
{
  PF("reset_index of %p\n", p) ;
  global_pointers[ the_index(p) ] = p ;
}

void *g_Realloc(void *old , unsigned int n)
{
  PF("realloc of %p with %d\n", old, n) ;
  if ( old==NULL ) return( g_Malloc(n) ) ;
  is_valid(old);
  old = (char*)realloc( (char*)old-sizeof(Index) , n+sizeof(Index) ) ;
  PF("new = %p\n", old) ;
  if ( old == NULL )
    {
      g_Printf("Realloc. No more memory, bye bye.\n") ;
      g_Printf("You have asked %d bytes\n",n) ;
      exit(19) ;
    }
  old = (char*)old + sizeof(Index)  ;
  PF("new after = %p\n", old) ;
  reset_index(old) ;
  return(old) ;
}

void *g_Malloc(unsigned int n)
{
  void *v ;
  int nb, i ;
  
  PF("malloc(%d)\n", n) ;
  if ( n==0 ) n = 1 ;
  
  v = malloc(n+sizeof(Index)) ;
  
  if ( v==0 )
    {
      g_Printf("Malloc. No more memory, bye bye\n") ;
      g_Printf("You have asked %d bytes\n",n) ;
      exit(20) ;
    }

  v = (char*)v + sizeof(Index) ;
  
  PF("First free = %d\n",  global_first_free) ;

  if (  global_first_free < 0 )
    {
      PF("change global_pointers before=%p\n", global_pointers) ;
      nb = global_pointers_size ;
      global_pointers_size = 16 + 1.5*global_pointers_size ;
      global_pointers = realloc(global_pointers
				, sizeof(*global_pointers)*global_pointers_size) ;
      PF("change global_pointers after=%p\n", global_pointers) ;
      for(i=nb; i<global_pointers_size; i++)
	global_pointers[i] = (void*)(i+1) ;
      global_pointers[global_pointers_size-1] = (void*)-1 ;

      global_first_free = nb ;

      PF("New First free = %d\n",  global_first_free) ;
    }

  PF("set index\n", global_pointers) ;
  i = global_first_free ;
  global_first_free = (int)global_pointers[global_first_free] ;  
  global_pointers[ i ] = v ;
  ((Index*)v)[-1] = i ;

  return(v) ;
}

void g_Free( void *v )
{
  int i ;

  PF("free %p\n", v) ;
  is_valid(v);
  i = the_index(v) ;
  global_pointers[ i ] = (void*)global_first_free ;
  global_first_free = i ;
  free((char*)v-sizeof(Index)) ;
  PF("end free %p\n", v) ;
}

g_Boolean g_Is_Memory_Clean()
{
  int i, nb ;

  PF("memory clean ?\n") ;
  nb = 0 ;
  for(i=global_first_free; i!=-1; i = (int)global_pointers[i] )
    {
      nb++ ;
    }
  PF("memory clean ? nb=%d global_pointers_size=%d\n", nb, global_pointers_size ) ;
  if ( nb != global_pointers_size )
    return(g_False) ;
  else
    return(g_True) ;
}

char *g_Save_String(const char* s)
{
char *n ;

n = g_Malloc( strlen(s) + 1 ) ;
strcpy( n , s ) ;
return( n ) ;
}

int g_Nb_Malloc()
{
return( 0 ) ;
}
