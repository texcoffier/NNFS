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

#ifndef G_UPDATE_H
#define G_UPDATE_H

#include "g_nnfs.h"

void g_Update( g_NNFS*, g_File_Info * ) ;
void g_Display_File_Info(g_NNFS*, g_File_Info*) ;
void g_Display_Nnfs_File(const g_File_On_Host *f) ;
char *g_Type_To_String(g_Type_Of_File t) ;
g_Type_Of_File g_String_To_Type(const char *s) ;

#endif
