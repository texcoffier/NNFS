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

#ifndef G_NNFS_H
#define G_NNFS_H

#include "g_header.h"
#include "g_option.h"
#include "g_config.h"
#include "g_state.h"

struct g_nnfs
	{
	g_Options	options ;	/* Command line options */
	g_Header	header ;	/* Stored on the NNFS media */
	g_Config	config ;	/* Data from configuration */
	g_State		state ;		/* Current state */
	} ;
typedef struct g_nnfs g_NNFS ;

void g_Output(char,char*) ;
void g_Output_Binary(char code, void *mem, int len) ;
void g_Output_Binary2(char code, void *mem, int len, char* s) ;

#endif
