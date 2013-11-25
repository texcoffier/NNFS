/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-1997  Thierry EXCOFFIER (exco@ligia.univ-lyon1.fr)
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
#ifndef G_TYPE_H
#define G_TYPE_H

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

enum g_boolean 
		{
		g_False,
		g_True
		} ;
typedef enum g_boolean g_Boolean ;

enum g_processing
		{
		g_Stop,
		g_Continue
		} ;
typedef enum g_processing g_Processing ;

enum g_result 
		{
		g_Error,
		g_Ok
		} ;
typedef enum g_result  g_Result ;

enum g_solution
		{
		g_No_Solution,
		g_A_Solution,
		g_An_Error
		} ;
typedef enum g_solution g_Solution ;

enum g_where
		{
		g_Out,
		g_In
		} ;
typedef enum g_where g_Where ;

typedef float g_Float_0_1 ;

typedef unsigned long g_Nb ;

typedef void* g_Id ;

#endif
