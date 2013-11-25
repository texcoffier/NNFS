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

#ifndef G_UNIX_H
#define G_UNIX_H

#include "g_config.h"
#include "g_option.h"
#include "g_state.h"
#include "g_nnfsrc.h"

enum g_signal_type
{
  g_Signal_User1,
  g_Signal_User2,
  g_Signal_Pipe,
  g_Signal_Interrupt,
  g_Signal_Background_Output,
  g_Signal_Terminate
} ;
typedef enum g_signal_type g_Signal_Type ;



void  g_Read_Config_Char(char *nnfsrc_name, g_Nnfsrc_Var *, const char *hostname) ;

FILE *g_Open_Input_File (const g_Config*, int answer, int* pid_unsplit) ;
FILE *g_Open_Output_File(const g_Config*, g_Boolean multiple,
			 g_Boolean format, int answer) ;
char *g_Get_Hostname    () ;
g_Nb  g_Get_Free_Size   (const char *file) ;
char *g_Nnfs_Dir        () ;
char *g_Nnfsrc          (char *home) ;
char *g_LogFile         (char *home) ;
char *g_FilteredLogFile (char *home) ;
int   g_Getuid          () ;
int   g_Get_Columns     () ;
int   g_Get_Lines       () ;
void  g_Signal          (g_Signal_Type st, void (*fct)(int)) ;
void  g_Kill            (int pid, g_Signal_Type st) ;

void g_Signal_Ignore(int) ;

#endif
