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

#ifndef G_HOST_H
#define G_HOST_H

#include "g_header.h"
#include "g_option.h"

void  g_List_Hosts  (const g_Header *) ;
void  g_Add_Nickname(g_Header *, const char *host, const char* alias) ;
void  g_Search_Hostname(g_Header *, g_Options *) ;

#endif
