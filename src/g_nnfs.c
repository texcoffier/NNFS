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
#include "g_nnfs.h"
#include "g_debug.h"
#include "g_malloc.h"
#if STDC_HEADERS
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>		/* For read() */
#endif
#include "g_init.h"
#include "g_unix.h"
#include "g_merge.h"
#include "g_update.h"
#include "g_host.h"
#include "g_finish.h"
#include "g_tree.h"
#include "g_secure.h"
#include "g_histo.h"
#include "g_dspbar.h"
#include "g_report.h"
#include "g_read.h"
#include "g_trim.h"
#include "g_dir.h"
#include "g_ui.h"
#include "g_filter.h"

char *g_Informations = 
"@(#) NNFS: Non-Networked File System, Release "NNFS_VERSION"\n\
Copyright (C) 1995-2001 Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)" ;

/*
 *
 */


void g_Debug_File_Info(g_NNFS *nnfs, g_File_Info *info)
{
if ( strcmp(nnfs->options.debug_file, nnfs->state.name) == 0 )
	g_Display_File_Info(nnfs,info) ;
}


void dumping(g_NNFS *nnfs)
{
if ( nnfs->options.dump )
	{
	g_Output_Display_Bar("Dump database", nnfs->header.nb_files) ;
	g_Tree_Travel(nnfs, g_Display_File_Info,NULL) ;
	}
if ( nnfs->options.debug_file )
	{
	g_Tree_Travel(nnfs, g_Debug_File_Info,NULL) ;
	}
}

void g_Output_Binary(char code, void *mem, int len)
{
  if ( len >= G_LINE_CHUNK-4 )
    {
      g_Printf("Increase G_LINE_CHUNK to %d and recompile\n", len+1) ;
      g_Printf("And mail to the maintener\n") ;
      exit(21) ;
    }
  len += 4 ;
  printf("%c%c%c%c",'\0', len/256, len%256, code) ;
  if ( fwrite(mem, len-4, 1, stdout) != 1 )
    {
      exit(22) ; 
    }
  fflush(stdout) ;
}

void g_Output_Binary2(char code, void *mem, int len, char* s)
{
  int len2 ;
  char x[G_LINE_CHUNK] ;

  len2 = len+4+strlen(s)+1 ;
  if ( len2 >= G_LINE_CHUNK-4 )
    {
      g_Printf("Increase G_LINE_CHUNK to %d and recompile\n", len2+1) ;
      g_Printf("And mail to the maintener\n") ;
      exit(23) ;
    }
  sprintf(x,"%c%c%c%c",'\0', len2/256, len2%256, code) ;
  memcpy(x+4,mem, len) ; 
  memcpy(x+4+len,s, strlen(s)+1) ; 
  if ( fwrite(x, len2, 1, stdout) != 1 )
    {
      exit(24) ;
    }
  fflush(stdout) ;
}

void g_Output(char code, char *string)
{
  int len ;
  
  len = strlen(string) + 1 ;
  g_Output_Binary(code, string, len) ;
}


void g_T(g_NNFS *nnfs, char *name, int nb,
 			void (*fct)(g_NNFS *, g_File_Info*),
			void (*post)(g_NNFS *, g_File_Info*))
{
g_Output_Display_Bar(name, nb) ;
g_Tree_Travel(nnfs, fct, post) ;
g_Finish_Display_Bar() ;
dumping(nnfs) ;
}


/*
 *
 */

int main(int argc, char **argv)
{
g_NNFS nnfs ;
struct stat st ;
int status ;
char temp[G_LINE_CHUNK] ;
int medium ;
int pipe_dir[2] ;
int pid_unsplit, pid_merge ;


g_Signal( g_Signal_Background_Output, g_Signal_Ignore) ;
g_Signal( g_Signal_Pipe, g_Signal_Ignore) ;
/* signal( SIGCHLD, SIG_IGN) ; */
g_Interpret_Debug_Args(&argc,argv) ;
g_Init(&nnfs, argc, argv) ;			/* Initialisations */
g_Option(&nnfs.options) ;               	/* Get line options */
g_Read_Config(&nnfs.config, nnfs.options.hostname) ;	/* Get config */

g_sync_readonly_dir(nnfs.config.sync_readonly_dir) ;


#if HAVE_GTK_INIT
#ifndef __CHECKER__
if ( nnfs.options.x11 )
  {
    medium = g_Ui(g_Ui_GTK,&argc,&argv,&nnfs.config, &nnfs.options) ;
    if ( nnfs.config.nnfsrc_created )
      {
	g_Output(G_NNFSRC_CREATED,nnfs.config.nnfsrc) ;
      }
    else
      {
	g_Output(G_NNFSRC_NOT_CREATED,"") ;
      }
    g_read(0,temp,1) ; /* Read user answer : format or continue */
    /*
    g_Free_Config(&nnfs.config, g_False) ;
    g_Read_Config(&nnfs.config, nnfs.options.hostname) ;
    */
    switch( temp[0] )
      {
      case G_FORMAT:
	nnfs.options.format = g_True ;
	break ;
      case G_CONTINUE:
	break ;
      default:
	g_Printf("Internal error g_nnfs.c\n") ;
      }
  }
else
#endif
#endif
  {
    medium = g_Ui(g_Ui_Txt,&argc,&argv,&nnfs.config,&nnfs.options) ;
    if ( nnfs.config.nnfsrc_created )
      {
	g_Output(G_NNFSRC_CREATED,nnfs.config.nnfsrc) ;
	if ( 1 )
	  {
	    nnfs.options.format = g_True ;
	    nnfs.options.new = getenv("HOME") ;
	    nnfs.options.add = g_True ;
	  }
	else
	  {
	    exit(0) ;
	  }
      }
  }

/*
 * Initialisation that need configuration files
 */
nnfs.config.output        = g_Compile_Filter(nnfs.config.file_filter) ;
if ( nnfs.options.dump )
	{
	g_Printf("Output filter:\n") ;
	g_Print_Filter(nnfs.config.output) ;
	}

nnfs.config.history = g_History_Dir(nnfs.config.nnfs,
					 nnfs.config.history_format);

if ( !nnfs.options.format )
    nnfs.state.input = g_Open_Input_File(&nnfs.config, medium,&pid_unsplit) ;
else
  {
    nnfs.state.input = g_popen("echo not_an_integer","r") ;
    pid_unsplit = -1 ;
  }

nnfs.state.filteredlogfile = g_fopen(nnfs.config.filteredlogfile, "w") ;
nnfs.state.logfile = g_fopen(nnfs.config.logfile, "w") ;
fprintf(nnfs.state.logfile,
	"For each file, there is a line of information by host.\n"
	"The last line is the very current information for current host.\n"
	"Informations are :\n"
	"   - The mode. File type U for UNSUPPORTED, D for DELETED.\n"
	"   - The number of files contained if it is a directory.\n"
	) ;
#ifdef ROOTNNFS
fprintf(nnfs.state.logfile,
	"   - The UID and GID.\n"
	) ;
#endif
fprintf(nnfs.state.logfile,
	"   - The size in byte.\n"
	"   - The last write date.\n"
	"\n"
	"R       Read the file from medium\n"
	" W      Write the file to medium\n"
	"  D     Delete the local file\n"
	"   C    Conflict of update\n"
	"    f   File is nnfs_filtered (it was filtered by previous host)\n"
	"     F  File is filtered, it will be not wrote on medium\n"
	"      H Historize the file\n"
	"\n"
	) ;
/*
 * Read header
 */
g_Read_Header(nnfs.state.input,&nnfs.header) ;	/* Read Header */

g_Output(G_CURRENT_HOST,g_Get_Hostname()) ;
g_Search_Hostname(&nnfs.header, &nnfs.options) ;
/*
 * Start directory browsing process
 */
nnfs.header.fs[nnfs.header.fs_id].update = time(0L) ;
g_chdir(nnfs.header.fs[nnfs.header.fs_id].home) ;

if ( nnfs.options.pipe )
  {
   g_pipe(pipe_dir) ;
   pid_merge = g_fork() ;
   if ( pid_merge==0 )
     {
       close(pipe_dir[0]) ;
       g_Pipe_Directory( fdopen(pipe_dir[1],"w"), ".") ;
       exit(0) ;
     }
  }
else
  pid_merge = 0 ; /* To avoid a compiler warning */
/*
 * Read file information
 */
g_Output_Display_Bar("Reading header", nnfs.header.nb_files) ;
nnfs.state.nnfs =
  g_Read_Nnfs_File(nnfs.state.input, nnfs.header.nb_fs, nnfs.options.add) ;
g_Finish_Display_Bar() ;
/*
 * Top dir initialization
 */
if ( lstat(".", &st) )
  {
    PERROR(".") ;
    exit(25) ;
  }
g_Stat_To_File_On_Host(&st, &nnfs.state.nnfs->hosts[nnfs.header.nb_fs], &nnfs.config) ;
 nnfs.config.device_number_to_copy = st.st_dev ;

/*
 * Merging local and nnfs files
 */
g_Output_Display_Bar("Merge Local/Remote", nnfs.header.nb_files) ;

if ( nnfs.options.pipe )
  {
    g_Merge_Pipe(&nnfs, nnfs.state.nnfs, fdopen(pipe_dir[0], "r")) ;
    waitpid(pid_merge, &status, 0) ; /* stop zombi */
  }
else
  {
    g_Merge(&nnfs, nnfs.state.nnfs, ".") ;
  }
g_Finish_Display_Bar() ;
dumping(&nnfs) ;
if ( nnfs.options.exec_after_merge )
  if ( system( nnfs.options.exec_after_merge ) )
    PERROR(nnfs.options.exec_after_merge) ;
nnfs.header.nb_files = g_Nb_Files(&nnfs) ;
/*
 * Go on
 */
g_T(&nnfs,"Action choice"   ,nnfs.header.nb_files, NULL           , g_Update) ;
g_T(&nnfs,"Output filtering",nnfs.header.nb_files, g_Filter_A_File, NULL) ;

sprintf(temp,"%lu %lu %lu %lu %lu %lu %lu %lu %lu",
	nnfs.state.nb_local_file,    nnfs.header.nb_files,
	nnfs.state.nb_nnfs_to_local, nnfs.state.nb_local_to_nnfs,
	nnfs.state.nb_nnfs_change,   nnfs.state.nb_local_change,
	nnfs.state.nb_conflict,      nnfs.state.nb_to_delete,
	nnfs.state.nb_byte
	) ;
g_Output(G_STATS, temp) ;

fclose(nnfs.state.logfile) ;
fclose(nnfs.state.filteredlogfile) ;

/*
 * Ask the user if he wants to continue
 */

if (   (  nnfs.config.confirmation == 2
     && (nnfs.state.nb_to_delete || nnfs.state.nb_conflict) )
       ||  nnfs.config.confirmation == 1 )
	{
	g_Output(G_QUESTION, nnfs.config.logfile) ;
	g_read(0,temp,1) ;
	/*
	g_Free_Config(&nnfs.config, g_False) ;
	g_Read_Config(&nnfs.config, nnfs.options.hostname) ;
	*/
	if ( temp[0] != G_CONTINUE ) goto nnfsexit ;
	}

g_T(&nnfs, "Delete files", nnfs.state.nb_to_delete, NULL, g_Delete_A_File) ;

if ( 1 || nnfs.state.nb_nnfs_to_local )
  g_T(&nnfs, "Read files", nnfs.state.nb_nnfs_to_local, g_Read_A_File, NULL) ;
else
  {
    if ( !nnfs.options.format )
      g_Kill(pid_unsplit, g_Signal_User1) ;
  }
fclose(nnfs.state.input) ;

waitpid(pid_unsplit, &status, 0) ; /* retrieve media number and stop zombi */
status = WEXITSTATUS(status) ;

if ( nnfs.state.nb_nnfs_to_local )
  {
    if ( status==2 )
      {
	g_Printf("NNFS: Error reading medium\n") ;
	/*	exit(1) ; commented 20/06/2001 */
      }
  }
else
  if ( nnfs.options.format )
    status = 0 ;

g_T(&nnfs, "Timestamp files", nnfs.header.nb_files, NULL, g_Time_Stamp_A_File) ;

g_Output_Display_Bar("Trim empty directories", nnfs.header.nb_files) ;
g_Trim(nnfs.state.nnfs, nnfs.header.nb_fs) ;
g_Finish_Display_Bar() ;
nnfs.header.nb_files = g_Nb_Files(&nnfs) ;

/*
 * Write NNFS file system on medium
 */

nnfs.state.output = g_Open_Output_File(&nnfs.config, (g_Boolean)status,
				       nnfs.options.format, medium) ;
nnfs.header.nb_byte = nnfs.state.nb_byte ;
g_Write_Header(nnfs.state.output,&nnfs.header) ;

g_T(&nnfs, "Write informations", nnfs.header.nb_files, g_Write_An_Nnfs_File, NULL) ;
g_T(&nnfs, "Write files", nnfs.state.nb_local_to_nnfs, g_Write_A_File, NULL) ;

g_Output(G_FLUSHING_DATA, "") ;

fclose(nnfs.state.output) ;
wait(&status) ; /* To wait the end of compression */

 nnfsexit:

g_Finish(&nnfs) ;			/* Free memory and disc */

if ( !g_Is_Debug_Clean() ) return(10) ;
if ( !g_Is_Memory_Clean() ) return(11) ;

return(WEXITSTATUS(status)) ;
}

