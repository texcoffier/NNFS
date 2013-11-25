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

#ifndef G_TREE_H
#define G_TREE_H

#include "g_nnfs.h"
#include "g_file.h"

void g_Tree_Travel       (g_NNFS*,
				 void (*)(g_NNFS*, g_File_Info*) ,
				 void (*)(g_NNFS*, g_File_Info*) ) ;
void g_Tree_Travel1       (g_NNFS*, g_File_Info*,
				 void (*)(g_NNFS*, g_File_Info*) ,
				 void (*)(g_NNFS*, g_File_Info*) ) ;
void g_Write_An_Nnfs_File  (g_NNFS*, g_File_Info*) ;
void g_Filter_A_File       (g_NNFS*, g_File_Info*) ;
void g_Time_Stamp_A_File   (g_NNFS*, g_File_Info*) ;
void g_Write_A_File        (g_NNFS*, g_File_Info*) ;
void g_Delete_A_File       (g_NNFS*, g_File_Info*) ;
void g_Always_Delete_A_File(g_NNFS*, g_File_Info*) ;
int  g_Nb_Files            (g_NNFS*) ;

#endif
