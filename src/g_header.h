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

#ifndef G_HEADER_H
#define G_HEADER_H

/*
 *
 */

#include "g_type.h"
#include "g_const.h"
#include <stdio.h>
#if HAVE_TIME_H
#include <time.h>
#endif

/*
 *
 */

/* Release number for file format */
/* A change in this number indicate an INCOMPATIBLE file format */
		/* 0   -> 1   the  8 May 1996 */
		/* 1   -> 2   the 14 Aug 1996 */
		/* 2   -> 3/4 the 5  Apr 1998 */
                /* 3/4 -> 5/6 the 13 Dec 2002 for FIFO files */
#ifdef ROOTNNFS
#define RELEASE 5
#else
#define RELEASE	6
#endif

/*
 *
 */

struct g_fs_data
	{
	char	*host ;		/* Host name */
	char	*home ;		/* Home position */
	time_t	update ;	/* Time of update */
	int	uid ;		/* User UID */
	char  **nicknames ;	/* Alias of the host */
	g_Nb	nb_mounts ;
	} ;
typedef struct g_fs_data g_Fs_Data ;

struct g_header
	{
	g_Nb		nb_files ;
	g_Nb		nb_fs ;	/* Number of host */
	g_Fs_Data	*fs ;	/* Host informations */

	time_t		creation ;
	time_t		update ;	/* Time of last NNFS mount */
	g_Nb		release ;
	g_Nb		nbmount ;
	g_Nb            nb_byte ;

	/* Computed from previous informations */
	
	g_Nb		fs_id ;		/* current fs_id */
	} ;
typedef struct g_header  g_Header ;
/*
 *
 */

void  g_Read_Header (FILE*, g_Header *) ;
void  g_Write_Header(FILE*, const g_Header *) ;

#endif
