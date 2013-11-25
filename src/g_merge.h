/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-1997  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#ifndef G_MERGE_H
#define G_MERGE_H

#include "g_nnfs.h"

void g_Set_All_Info(g_File_Info *local, int nb_hosts) ;
void g_Merge( g_NNFS *, g_File_Info*, char *name) ;
void g_Merge_Pipe(g_NNFS *nnfs, g_File_Info *f, FILE* input) ;

#endif
