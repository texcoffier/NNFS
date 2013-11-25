/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2005  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#ifndef G_SECURE_H
#define G_SECURE_H

#include <stdio.h>
						       
void  g_fclose      (FILE*) ;
FILE *g_popen       (const char*, const char*) ;
void  g_pclose      (FILE*) ;
void  g_pipe        (int fildes[2]) ;
void  g_chmod       (const char*, int) ;
int   g_fread       (char*, int, int, FILE*) ;
int   g_fork        () ;
void  g_chdir       (const char*) ;
void  g_fprintf     (FILE*, const char*, ... ) ;
char *g_Read_Line   (FILE*) ;
char *g_Formatted_Strings(const char *format, ...) ;
int   g_read        (int fildes, char *buffer, int size) ;
char *g_dirname     (const char *filename) ;

int g_read_int(FILE *f) ;
int g_read_int_hexa(FILE *f) ;
long g_read_long(FILE *f) ;
long g_read_unsigned_long(FILE *f) ;
long g_read_long_hexa(FILE *f) ;


/* The next function may change temporary the directory mode
 * in order to work on protected directories if the errno is EACCES
 */
void g_sync_readonly_dir(int bool) ; /* Do we want to change the mode */

FILE *g_fopen(const char *name, const char*) ;
int g_symlink(const char *name, const char *value) ;
int g_creat  (const char *name, int mode) ;
int g_rename (const char *name, const char *new_name) ;
int g_remove (const char *name) ;
int g_rmdir  (const char *name) ;
int g_mkfifo (const char *name, int mode) ;
int g_mkdir  (const char *name, int) ;

#endif
