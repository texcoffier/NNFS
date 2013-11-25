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

#ifndef G_OPTION_H
#define G_OPTION_H

#include "g_type.h"

struct g_options
{
  /* Options from line command */

  int		argc ;
  char		**argv ;
  g_Boolean	add ;		/* Add an host to NNFS */
  char		*new;		/* Local NNFS name to add */
  g_Boolean	alias ;		/* Add an alias nickname to NNFS */
  char		*aliasname ;	/* nickname */
  char		*debug_file ;	/* Print debuging data for this file */
  g_Boolean	display_conf ;	/* If true, display configuration */
  char		*hostname ;     /* Local hostname */
  g_Boolean	uid ;		/* Accept UID change */
  g_Boolean	dump ;		/* Dump database */
  g_Boolean	pipe ;		/* Use special process to read hierarchy */
  g_Boolean     format ;        /* Don't read medium, write it */
  char		*exec_after_merge ; /* Shell command */
  g_Boolean     x11 ;
} ;
typedef struct g_options g_Options ;

/*
 *
 */

extern char* g_Informations ;

void  g_Option      (g_Options *) ;

#endif
