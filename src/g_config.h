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

#ifndef G_CONFIG_H
#define G_CONFIG_H

struct g_Filter ;
#include "g_type.h"

struct g_config
{
  char		  *nnfs ;	/* Name of nnfs directory */
  char		  *nnfsrc ;	/* Complete name of nnfsrc */
  g_Boolean        nnfsrc_created ;
  char		  *logfile ;	/* Complete name of logfile */
  char		  *filteredlogfile ; /* Complete name of logfile */
  char		  *history ;	/* Complete name of history dir */
  char		  *history_format ;
  char		  *read_medium ;
  char		  *write_medium ;
  char		  *compress_filter ;
  char		  *uncompress_filter ;
  char		  *file_filter ;
  char		  *eject ;
  int		   medium_size ;
  char            *medium_name ;
  int             historize ;
  struct g_Filter *output ;
  int              ask_medium ;
  int              confirmation ;
  int              auto_start ;
  int              sync_readonly_dir ;
  int              tmp_file ;
  char            *tmp ;
  char            *comment ;
  char            *switchcase ;
  int              copy_hard_link ;
  int              cross_mount_point ;

  int              mode_change_sensitive ;
  int              device_number_to_copy ; /* Should not be here .... */
	} ;

typedef struct g_config g_Config ;

#endif
