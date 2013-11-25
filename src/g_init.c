/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2002  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_init.h"
#include "g_malloc.h"
#include "g_debug.h"
#include "g_unix.h"
#include "g_nnfsrc.h"
#include "g_histo.h"

/*
 ******************************************************************************
 */

void G_FUNCTION(g_Init,(g_NNFS *nnfs, int argc, char **argv))

g_File_On_Host t[2] ;
G_PF("Sizeof g_file_info    : %d\n", sizeof(g_File_Info)) ;
G_PF("   2 * g_file_on_host : %d\n", sizeof(t)) ;
G_PF("   TOTAL              : %d\n", sizeof(g_File_Info)+sizeof(t)) ;
G_PF("+name +children + 3 malloc\n") ;

nnfs->header.nb_fs        = 0 ;		/* Number of host in NNFS Filesystem */
G_MALLOC(nnfs->header.fs,1) ;
nnfs->header.update       = time(0) ;	/* Current time */
nnfs->header.release      = RELEASE ;	/* Current release */
nnfs->header.nbmount      = 0 ;		/* Number of mount */
nnfs->header.creation     = nnfs->header.update ;

nnfs->options.dump 	  = g_False ;
nnfs->options.pipe 	  = g_False ;
nnfs->options.format 	  = g_False ;
nnfs->options.add	  = g_False ;	/* True if an host is added */
nnfs->options.alias	  = g_False ; /* True if current host is a new alias */
nnfs->options.debug_file  = NULL ;	/* Name of the file to debug */
nnfs->options.exec_after_merge = NULL ;	/* Shell command */
nnfs->options.hostname    = g_Get_Hostname() ;
nnfs->options.aliasname   = 0 ;	/* No alias hostname */
nnfs->options.argc        = argc ;
nnfs->options.argv        = argv ;
nnfs->options.display_conf= g_False ;	/* Don't display configuration */
nnfs->options.uid 	  = g_False ;	/* Accept UID change */

nnfs->config.nnfs          = g_Nnfs_Dir();
nnfs->config.nnfsrc        = g_Create_Nnfsrc(nnfs->config.nnfs,
					     &nnfs->config.nnfsrc_created);
nnfs->config.logfile       = g_LogFile(nnfs->config.nnfs);
nnfs->config.filteredlogfile = g_FilteredLogFile(nnfs->config.nnfs);
nnfs->config.historize     = g_True ;
nnfs->config.confirmation  = g_True ;
nnfs->config.ask_medium    = g_True ;
nnfs->config.cross_mount_point = g_False ;
nnfs->config.copy_hard_link = g_False ;

nnfs->state.dir            = NULL ;
nnfs->state.max_file_name  = 2 ;
nnfs->state.name           = g_Malloc(nnfs->state.max_file_name) ;
nnfs->state.read_error     = g_False ;
nnfs->state.nb_to_delete	= 0 ;
nnfs->state.nb_local_to_nnfs	= 0 ;
nnfs->state.nb_historize    	= 0 ;
nnfs->state.nb_nnfs_to_local	= 0 ;
nnfs->state.nb_local_change	= 0 ;
nnfs->state.nb_nnfs_change	= 0 ;
nnfs->state.nb_read_error	= 0 ;
nnfs->state.nb_write_error	= 0 ;
nnfs->state.nb_local_file	= 0 ;
nnfs->state.nb_conflict  	= 0 ;
nnfs->state.nb_byte     	= 0 ;

strcpy(nnfs->state.name, ".") ;

G_RETURN(;) ;
}

