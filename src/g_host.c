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

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include "g_host.h"
#include "g_debug.h"
#include "g_malloc.h"
#include "g_unix.h"
#include "g_help.h"
#include "g_nnfs.h"
#include "g_secure.h"

/*
 ******************************************************************************
 * List all the hosts
 ******************************************************************************
 */

void G_FUNCTION(g_List_Hosts,(const g_Header *header))

int i,j ;
char buf[G_LINE_CHUNK] ;

if ( header->nb_fs==0 ) G_RETURN(;) ;

G_PF("nb_fs : %d\n", header->nb_fs) ;
for(i= 0 ;i<header->nb_fs;i++)
	{
        G_PF("i : %d\n", i) ;
	g_Output(G_HOST,header->fs[i].host) ;
	sprintf(buf,"%lu", header->fs[i].nb_mounts) ;
	g_Output(G_NB_MOUNT,buf) ;
	g_Output(G_DIRECTORY,header->fs[i].home) ;
	sprintf(buf,"%lu", (unsigned long)header->fs[i].update) ;
	g_Output(G_LAST_MOUNT,buf) ;
	for(j=0;header->fs[i].nicknames[j]!=0;j++)
		g_Output(G_ALIAS,header->fs[i].nicknames[j]) ;
	g_Output(G_HOST_DONE,buf) ;
	}

G_RETURN(;) ;
}


/*
 ******************************************************************************
 * 
 ******************************************************************************
 */

void G_FUNCTION(g_Add_New_Host,(g_Header *header, const g_Options *options))

G_PF("current nb_fs to increment : %d\n", header->nb_fs) ;
header->fs = (g_Fs_Data*)g_Realloc( (char*)header->fs, 
              (header->nb_fs+1)*sizeof(*header->fs)) ;

header->fs[header->nb_fs].host = g_Save_String( options->hostname )   ;
header->fs[header->nb_fs].home = g_Save_String( options->new )   ;
header->fs[header->nb_fs].update       = time(0) ;
header->fs[header->nb_fs].uid          = g_Getuid() ;
header->fs[header->nb_fs].nb_mounts    = 0 ;
G_MALLOC(header->fs[header->nb_fs].nicknames,1) ;
header->fs[header->nb_fs].nicknames[0] = 0 ;

header->nb_fs++ ;
G_RETURN(;) ;
}


/*
 ******************************************************************************
 * Search the host in the NNFS structure
 ******************************************************************************
 */

int G_FUNCTION(g_Real_Search_Hostname,(const g_Header *header,
					const char *hostnam))

int i,j ;

for( i=0 ; i<header->nb_fs ; i++ )
	{
	if ( strcmp( hostnam, header->fs[i].host ) == 0 ) G_RETURN(i) ;
	for(j=0;header->fs[i].nicknames[j]!=0;j++)
		if ( strcmp( hostnam,header->fs[i].nicknames[j] ) == 0 )
			 G_RETURN(i) ;
	}
G_RETURN(i) ;
}

/* 
 *
 */

void G_FUNCTION(g_Add_Nickname,(g_Header *header, const char *hostn, const char *nickn))

int i,j ;

i = g_Real_Search_Hostname(header, nickn) ;
if ( i != header->nb_fs )
	{
	g_Printf("NNFS: %s is yet an hostname or an alias\n", nickn) ;
	exit(14) ;
	}

i = g_Real_Search_Hostname(header, hostn) ;
if ( i == header->nb_fs )
	{
	g_Printf("NNFS: %s is not an existing hostname or an alias\n", hostn) ;
	g_Printf("NNFS: Possible names are in:\n") ;
	g_List_Hosts(header) ;
	exit(15) ;
	}

for(j=0;header->fs[i].nicknames[j]!=0;j++) ;

header->fs[i].nicknames = (char**)g_Realloc( (char*)header->fs[i].nicknames,
			(j+2)	* sizeof(char*) ) ;
G_MALLOC(header->fs[i].nicknames[j], strlen(nickn)+1 ) ;
strcpy( header->fs[i].nicknames[j], nickn ) ;
header->fs[i].nicknames[j+1] = 0 ;
G_RETURN(;) ;
}

/* 
 *
 */

void G_FUNCTION(g_Search_Hostname,(g_Header *header, g_Options *options))

int i ;
g_Fs_Data *host ;
char temp[G_LINE_CHUNK] ;

if ( options->alias )
	g_Add_Nickname(header,options->aliasname,options->hostname) ;

/* Search hostname */

i = g_Real_Search_Hostname(header, options->hostname) ;

if ( i == header->nb_fs )
   {
   if ( options->add )
	{
	g_Add_New_Host(header, options) ;
	}
   else
	{
        g_List_Hosts(header) ;

        if ( options->x11 )
           {
           g_Output(G_HOST_NOT_IN_NNFS,"") ;
	   /*
	    * Retrieve directory to incorporate
	    */
	   i = 0 ;
	   do
	     {
	       g_read(0,&temp[i],1) ;
	     }
	   while( temp[i++] != '\0' ) ;

           switch( temp[0] )
	     {
               case G_ADD_HOST:
	           options->new = strdup(temp+1) ;
	           options->add = g_True ;
                   g_Search_Hostname(header, options) ;
                   g_Output(G_CURRENT_HOST,g_Get_Hostname()) ;
                   G_RETURN(;) ;
               case G_ADD_NICKNAME:
	           options->aliasname = strdup(header->fs[temp[1]-1].host) ;
                   options->alias = g_True ;
                   g_Search_Hostname(header, options) ;
                   g_Output(G_CURRENT_HOST,g_Get_Hostname()) ;
                   G_RETURN(;) ;
               default:
	           g_Printf("Internal error " __FILE__ "\n") ;
                   exit(16) ;
	     }
	   }
        else
           {
           g_Printf("NNFS: This host doesn't participate in NNFS\n") ;
           g_Output(G_HOST_NOT_IN_NNFS,"") ;
           exit(17) ;
           }
        }
   }
else
   if ( options->add /* && i!=header->nb_fs-1 */ )
	{
	g_Printf("NNFS: This host (%s) is yet in NNFS system\n",
			options->hostname) ;
	g_Printf("NNFS: Can't add it twice, the option is not taken into account.\n") ;
	}


G_PF("Normal case\n") ;
header->fs_id = i ;
header->fs[i].update = time(0) ;

host = &header->fs[header->fs_id] ;

if ( host->uid == -1 ) host->uid = g_Getuid() ;
	else
		if ( host->uid != g_Getuid() )
			{
			if ( options->uid )
				host->uid = g_Getuid() ;
			else
				g_Uid_Help( g_Getuid(), host->uid) ;
			}

g_List_Hosts(header) ;
G_RETURN(;) ;
}


