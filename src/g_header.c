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

#include "g_header.h"
#include "g_debug.h"
#include "g_malloc.h"
#include "g_secure.h"
#include "g_help.h"
#include "g_host.h"
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

/*
 *
 * NNFS header
 *
 * UPDATE update
 * RELEASE release_number
 * NBMOUNT nb
 * HOST host home update uid
 * HOST host home update uid
 * HOST host home update uid
 * NICKNAME host1 alias1
 * NICKNAME host1 alias2
 * NICKNAME host2 alias4
 * BEGIN
 * 
 */

/*
 ******************************************************************************
 */

int G_FUNCTION(g_Read_Header_Type,(FILE *input))

/* DON'T CHANGE ORDER */
static char *header_key[] = {	"--UPDATE--",   "--RELEASE--", "--HOST--",
				"--BEGIN--",    "--NBMOUNTS--",
				"--CREATION--", "--NICKNAME--",
				"--SIZE--"
				} ;
char *type ;
int i ;

type = g_Read_Line(input) ;
G_PF("Read : %s\n", type) ;
if ( type == NULL ) G_RETURN(-1) ;

for( i=0 ; i<sizeof(header_key)/sizeof(header_key[0]) ; i++ )
   	if ( strcmp(type,header_key[i]) == 0 )
	  {
	    g_Free(type) ;
	    G_RETURN(i) ;
	  }
g_Free(type) ;
G_RETURN( g_Read_Header_Type(input) ) ;
}

/*
 *
 */

void G_FUNCTION(g_Read_Header,(FILE *input, g_Header *header))

char *hostn, *nickn ;

header->nb_files = 0 ;

if ( fscanf(input, "%lu\n", &header->nb_files) != 1 )
   {
   G_PF("Can't read number of files\n") ;
   G_RETURN(;) ;
   }

do
   {	
   switch( g_Read_Header_Type(input) )
      {
      case -1:			/* Create a new header */
	G_PF("New NNFS header : Creation\n") ;
	G_RETURN(;) ;

      case 0: /* UPDATE */
        header->update = g_read_unsigned_long(input) ;
	G_PF("Update = %lu\n", header->update) ;
	break ;
      case 1: /* RELEASE */
        header->release = g_read_int(input) ;
	if ( header->release!=RELEASE )
		{
		g_Printf("NNFS: INCOMPATIBLE FILE FORMAT\n");
		g_Printf("NNFS: YOU NEED TO USE THE SAME NNFS RELEASE\n");
		g_Printf("NNFS: ON ALL THE HOSTS\n");
		exit(11) ;
		}
	G_PF("Release = %d\n", header->release) ;
	break ;
      case 2: /* HOST */
        header->fs = (g_Fs_Data*)g_Realloc( (char*)header->fs,
                              (header->nb_fs+1)*sizeof(*header->fs)) ;
	header->fs[header->nb_fs].host       = g_Read_Line(input) ;
	header->fs[header->nb_fs].home       = g_Read_Line(input) ;
        header->fs[header->nb_fs].update     = g_read_long_hexa(input) ;
        header->fs[header->nb_fs].uid        = g_read_int(input) ;
        header->fs[header->nb_fs].nb_mounts  = g_read_long(input) ;
	if ( header->fs[header->nb_fs].home[0] != G_CHAR_SEPARATOR )
		{
		G_PRINTF("Bad home directory\n" ) ;
		exit(12) ;
		}
 	G_MALLOC(header->fs[header->nb_fs].nicknames,1) ;
	header->fs[header->nb_fs].nicknames[0]	= 0 ;
	G_PF("Host=%s Home=%s update=%lx\n",
			header->fs[header->nb_fs].host,
			header->fs[header->nb_fs].home,
			header->fs[header->nb_fs].update) ;
	G_PF("UID=%d nb_mounts=%ld\n",
			header->fs[header->nb_fs].uid,
	                header->fs[header->nb_fs].nb_mounts) ;
	header->nb_fs++ ;
	break ;
      case 3: /* BEGIN */
	G_PF("BEGIN\n") ;
	G_RETURN(;) ;
	break ;
      case 4: /* NBMOUNT */
        header->nbmount = g_read_unsigned_long(input) ;
	G_PF("NbMount = %d\n", header->nbmount) ;
	break ;
      case 5: /* CREATION */
        header->creation = g_read_long_hexa(input) ;
	G_PF("Creation = %d\n", header->creation) ;
	break ;
      case 6: /* NICKNAMES */
        hostn = g_Read_Line(input) ;
        nickn = g_Read_Line(input) ;
	g_Add_Nickname(header,hostn,nickn) ;
	G_PF("Alias = %s,%s\n", hostn, nickn) ;
        g_Free(hostn) ;
        g_Free(nickn) ;
	break ;
      case 7: /* SIZE */
        header->nb_byte = g_read_long_hexa(input) ;
        break ;
      }
   }
while( 1 ) ;
}

/*
 ******************************************************************************
 */

void G_FUNCTION(g_Write_Header,(FILE *output, const g_Header *header))

int i,j ;

if ( output==NULL ) G_RETURN(;) ;

header->fs[header->fs_id].nb_mounts++ ;

g_fprintf(output,"%d\n",header->nb_files) ;
g_fprintf(output,"--RELEASE--\n%d\n",RELEASE) ;
g_fprintf(output,"--UPDATE--\n%lu\n",time(0)) ;
g_fprintf(output,"--NBMOUNTS--\n%lu\n",header->nbmount+1) ;
g_fprintf(output,"--CREATION--\n%lx\n",header->creation) ;
g_fprintf(output,"--SIZE--\n%lx\n",header->nb_byte) ;
for (i=0;i<header->nb_fs;i++)
	{
	g_fprintf(output,"--HOST--\n%s\n%s\n%lx\n%d\n%lu\n",header->fs[i].host,
		header->fs[i].home,
		header->fs[i].update,
		header->fs[i].uid,
		header->fs[i].nb_mounts) ;
	}

for (i=0;i<header->nb_fs;i++)
	{
	for(j=0;header->fs[i].nicknames[j]!=0;j++)
		g_fprintf(output,"--NICKNAME--\n%s\n%s\n",
			header->fs[i].host,header->fs[i].nicknames[j]) ;
	}

g_fprintf(output,"--BEGIN--\n") ;

G_RETURN(;) ;
}

