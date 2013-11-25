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

#ifndef G_FILTER_H
#define G_FILTER_H

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_REGEX_H && HAVE_REGCOMP
#include <regex.h>
#else
#include "./regex.h"
#endif

#include "g_file.h"
#include "g_nnfs.h"

struct g_Filter
{
  enum { Addition, Subtraction } what ;
  int perm ;
  int minus_perm ;
  int plus_perm ;
  int plus_size ;
  int minus_size ;
  g_Type_Of_File type ;
  regex_t name ;
  char *regex ;
  char *t_plus_size, *t_minus_size ;
  
  struct g_Filter *next ;
} ;
typedef struct g_Filter g_Filter ;

g_Filter *g_Compile_Filter(char *f) ;
void g_Print_Filter(g_Filter*) ;
void g_Free_Filter(g_Filter*) ;
g_Boolean g_Eval_Filter(char *, g_File_On_Host *, g_Filter *) ;

#endif
