/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1997-1998  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#ifndef G_UI_H
#define G_UI_H

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <ctype.h>
#include "g_unix.h"
#if STDC_HEADERS
#include <stdlib.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_TIME_H
#include <time.h> /* sys/time.h */
#endif

struct g_config ;
struct g_options ;

int g_Ui( void (*)(int,int,int,int,pid_t,int *, char***, struct g_config*, struct g_options*),int *, char***, struct g_config*, struct g_options*) ;
void g_Ui_Txt(int,int,int,int,pid_t,int *, char***, struct g_config*, struct g_options*) ;
void g_Ui_GTK(int,int,int,int,pid_t,int *, char***, struct g_config*, struct g_options*) ;
enum state { Reading, Writing } ;
#define FORMAT "NNFS: %21s:%-20s %5s %s"

#endif
