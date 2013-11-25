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

#ifndef G_STATE_H
#define G_STATE_H

#include <stdio.h>
#include "g_file.h"

struct g_state
	{
	g_File_Info	*nnfs ;		/* NNFS filesystem */
	FILE		*input ;	/* Input NNFS file */
	FILE		*output ;	/* Output NNFS file */
	FILE		*logfile ;	/* Log file */
	FILE		*filteredlogfile ; /* Log filtered file */
	/*
	 * Fields updated by g_Tree_Travel
	 */
	char		*name ;		/* Current full file name */
	g_Nb		max_file_name ;
        g_File_Info     *dir ;          /* current directory on Tree_Travel */
	g_Nb            child ;  	/* Child number in current dir */
	g_Nb		dir_len ;	/* len of directory name in "name" */
	/*
	 *
	 */
	g_Boolean	read_error ;
	/*
	 * Statistics
	 */
	g_Nb		nb_to_delete ;
	g_Nb		nb_historize ;
	g_Nb		nb_local_to_nnfs ;
	g_Nb		nb_nnfs_to_local ;
	g_Nb		nb_local_change ;
	g_Nb		nb_nnfs_change ;
	g_Nb		nb_read_error ;
	g_Nb		nb_write_error ;
	g_Nb		nb_local_file ;
	g_Nb		nb_conflict ;
	g_Nb            nb_byte ;
	} ;
typedef struct g_state g_State ;

char * g_Next_File_Name( const g_State * ) ;

#endif
