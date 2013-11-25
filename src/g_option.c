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

#include "g_option.h"
#include "g_debug.h"
#include "g_help.h"

#include <stdio.h>
#include <stdlib.h>
#if HAVE_GETOPT_H
#include <getopt.h>
#else
#include "getopt.h"
#endif

/*
 ******************************************************************************
 */

void G_FUNCTION(g_Option,(g_Options *options))

int c ;

static struct option long_options[] =                                    
{
  {"help"   	     , 0, 0, 'h'},
  {"licence"	     , 0, 0, 'l'},
  {"add-first" 	     , 1, 0, 'f'},
  {"add"             , 1, 0, 'a'},
  {"add-nickname"    , 1, 0, 'n'},
  {"no-x11"          , 0, 0, 'x'},

  {"uid"    	     , 0, 0, 'u'},

  {"pipe"   	     , 0, 0, 'p'},
  {"dump"   	     , 0, 0, 'd'},
  {"config"          , 0, 0, 'c'},
  {"host"            , 1, 0, 'H'},
  {"debug_file"      , 1, 0, 'D'},
  {"exec_after_merge", 1, 0, 'e'},
  {0, 0, 0, 0}                                                           
};                                                                       

options->x11 = g_True ;

while (1)
{
  c = getopt_long (options->argc, options->argv, "hlf:a:n:xupdcH:D:e:", long_options, NULL);
  if (c == -1)
    break;
  
  switch (c)
    {
    default:
    case 0:
      g_Help() ;
      exit(27) ;
    case 'f':
      options->format = g_True ;
      /* fall through */
    case 'a' :
      options->new = optarg ;
      options->add = g_True ;
      if ( options->new[0] != G_CHAR_SEPARATOR )
	{
	  g_Printf("NNFS: %s is an invalid filename\n",options->new) ;
	  g_Printf("NNFS: Don't forget " G_STRING_SEPARATOR
		   " in the beginning\n") ;
	  exit(28) ;
	}
      break ;
    case 'p':
      options->pipe = g_True ;
      break ;
    case 'd':
      options->dump = g_True ;
      break ;
    case 'x':
      options->x11 = g_False ;
      break ;
    case 'u':
      options->uid = g_True ;
      break ;
    case 'c':
      options->display_conf = g_True ;
      break ;
    case 'H' :
      options->hostname = optarg ;
      break ;
    case 'D' :
      options->debug_file = optarg ;
      break ;
    case 'e' :
      options->exec_after_merge = optarg ;
      break ;
    case 'n' :
      options->aliasname = optarg ;
      options->alias = g_True ;
      break ;
    case 'h':
      g_Help() ;
      exit(0) ;
      break ;
    case 'l':
      g_Printf("\n%s\n\n",g_Informations ) ;
      g_Printf("%s%s%s\n",
"This program is free software; you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation; either version 2, or (at your option)\n"
"any later version.\n"
	       "\n",
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU General Public License for more details.\n"
	       "\n",
"You should have received a copy of the GNU General Public License\n"
"along with this program; if not, write to the Free Software\n"
"Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n"
  ) ;
      exit(0) ;
    }
}

if ( getenv("DISPLAY") == NULL || strcmp(getenv("DISPLAY"),"")==0 )
  options->x11 = g_False ;

G_RETURN(;) ;
}

