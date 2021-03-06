/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-1997  Thierry EXCOFFIER (exco@ligia.univ-lyon1.fr)
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
#ifndef G_DEBUG_H

#define G_DEBUG_H

#include "g_type.h"
#include "g_debugP.h"

extern int g_Printf(const char *format, ...) ;

#define PERROR(X) do { g_Printf("\n%s:%d\n",__FILE__,__LINE__) ;perror(X); } \
  while(0)


#ifdef G_DEBUG

void		g_Debug_List() ;
g_Result	g_Set_Debug_Level(const char *level) ;
g_Boolean	g_Is_Debug_Clean() ;
void		g_Indent() ;
void		g_Exit() ;
void		g_Interpret_Debug_Args(int *argc, char**argv) ;

#endif

#endif
