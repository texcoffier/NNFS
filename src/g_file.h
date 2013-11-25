/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2001  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#ifndef G_FILE_H
#define G_FILE_H

/*
 * In this structure,  there is not :
 *    UID/GID because NNFS is for one user on computers with maybe different
 *            Netherveless it is easy to change.
 */

#include <stdio.h>
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include "g_type.h"
#include "g_const.h"

enum g_type_of_file
{
		g_Is_A_Deleted,
		g_Is_A_Directory,
		g_Is_A_Link,
		g_Is_A_File,
		g_Is_Not_Supported,
		g_Is_A_FIFO,
		g_Is_Any	/* For the filter, MUST BE THE LAST */
	 } ;
typedef enum g_type_of_file g_Type_Of_File ;

enum  g_nnfs_error
{
	g_No_Error,
	g_Error_Create_File,
	g_Error_Write,
	g_Error_Close,
	g_Error_Open,
	g_Error_Read,
	g_Error_Type_Changed,
	g_Error_Time_Changed,
	g_Error_Size_Changed,
	g_Error_Mode_Changed,
	g_Error_Deleted,
	g_Error_Read_Link,
	g_Error_Chmod,
        g_Error_UidGid,
	g_Error_Is_On_Medium,
	g_Error_Rename_To_Real_Name,
	g_Error_Mkdir,
	g_Error_Lstat,
	g_Error_Delete,
	g_Error_Get_Time,
	g_Error_Create_Symbolic_Link,
	g_Error_Historize,
        g_Error_Type_Change_While_Read,
        g_Error_Because_Nnfs_Error,
	g_Error_Create_FIFO
	} ;
typedef enum  g_nnfs_error g_Nnfs_Error ;

/*
 * ATTENTION : This structure must be of size multiple of alignement
 * so the bits are at the beggining
 */

struct g_file_on_host
	{
	time_t date ;		/* Of modification */
	  /* Do not use "off_t", it doesn't work on IRIX (compiler bug?) */
	long size ;		/* File size */
	unsigned int type:3 ;	/* g_Type_Of_File */
	unsigned int mode:12 ;	/* 3*4 low bits of mode : rwxrwxrwx + sst */
	unsigned int up_to_date:1 ;
	unsigned int to_historize:1 ;
#ifdef ROOTNNFS
        uid_t        uid ;
        gid_t        gid ;
#endif
	} ;
typedef struct g_file_on_host g_File_On_Host ;

struct g_file_info
	{
	char *name ;
	struct g_file_info **children ;
	  /* 1 byte */
        unsigned int on_medium:1 ;
        unsigned int local_to_nnfs:1 ;
        unsigned int nnfs_to_local:1 ;
        unsigned int read_error:5 ;	/* g_Nnfs_Error */
	  /* 2 byte */
        unsigned int write_error:5 ;	/* g_Nnfs_Error */
        unsigned int local_change:1 ;
        unsigned int nnfs_change:1 ;
        unsigned int need_time_stamp:1 ;
	  /* 3 byte */
        unsigned int read_file_info:1 ;
        unsigned int update_nnfs:1 ;
        unsigned int to_delete:1 ;
        unsigned int conflict:1 ;
        unsigned int to_historize:1 ;
	unsigned int filtered:1 ;
	unsigned int nnfs_filtered:1 ;
        unsigned int nnfs_error:1;
	  /* 4 byte */
        unsigned int nnfs_to_local_child:1;
        unsigned int up_to_date_on_medium:7;
	  /* 5 byte */
        unsigned int up_to_date:7;
        unsigned int local_to_nnfs_child:1;
	  /* 6,7,8 byte */
	unsigned int nb_children:24 ;
	  /* 9-10 byte */
	unsigned int error:ERRNO_BITS ;
	g_File_On_Host hosts[1] ;/* Informations on file for each host */
	/* Yes former table go OUTSIDE of this structure */
	/* But it is correctly malloced. */
	/* This is done to remove a call to malloc */
	} ;
typedef struct g_file_info g_File_Info ;

g_File_Info* g_Read_Nnfs_File( FILE *f, int nb_hosts, int add ) ;
char *g_Read_Name(FILE *f) ;
g_File_Info* g_Init_File_Info(int nb_hosts) ;
char *g_other_file_info(int nb_fs, g_File_Info *f, int tab) ;
char *g_file_info_mode(g_File_Info *f, int host) ;

#endif
