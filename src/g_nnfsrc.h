/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1995-2005  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#ifndef G_NNFSRC_H
#define G_NNFSRC_H

#include <stdio.h>
#include "g_type.h"
#include "g_config.h"

enum g_var_type 
{
  g_Var_String,
  g_Var_Big_String,
  g_Var_Choice,
  g_Var_Boolean,
  g_Var_Integer,
  g_Var_Filter
} ;
typedef enum g_var_type g_Var_Type ;

struct g_nnfsrc_var
{
  char *name ;			/* Variable name */
  g_Var_Type type ;		/* Variable type */
  int page ;			/* Page for X configuration */
  int default_choice ;          /* Default value */
  char *tip ;
  char *widget_name ;
  char *choices[10][2] ;

  int offset_in_g_config ;	/* Computed offset, in g_Config struct */

  char *content ;		/* Content readed or to write */
  g_Boolean content_change ;	/* Content changed interactivly */
  void *menu ;
} ;
typedef struct g_nnfsrc_var g_Nnfsrc_Var ;

char *g_Create_Nnfsrc      (char *home, g_Boolean *created) ;

extern g_Nnfsrc_Var *g_Nnfsrc_Var_Table() ;
void  g_Read_Config     (g_Config*, const char *hostname) ;
void g_Compile_Config(g_Nnfsrc_Var *v, g_Config *config) ;
char *g_Case_Name(const char *hostname) ;
extern void g_Nnfsrc_Action(char *name,
			    g_Nnfsrc_Var *v,
			    char *in_case,
			    g_Boolean *in_case_found,
			    char ***cases, int *nb_cases,
			    FILE *g,
			    const char *hostname
			    ) ;

#endif
