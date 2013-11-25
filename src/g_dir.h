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

#ifndef G_DIR_H
#define G_DIR_H

#include "g_type.h"
#include "g_file.h"
#include "g_config.h"
#include "g_const.h"

struct g_dir
	{
	char *name ;
	g_File_On_Host info ;
	} ;
typedef struct g_dir g_Dir ;

struct stat ;
void g_Read_Directory(g_Dir**, int *nb, const g_Config *config) ;
void g_Stat_To_File_On_Host(const struct stat*, g_File_On_Host*, const g_Config *config) ;

void g_Pipe_Directory(FILE *output, char *name) ;
void g_Read_Pipe_Directory(g_Dir**, int *nb, FILE*input, const g_Config *config) ;

#endif




