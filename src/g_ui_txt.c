/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1997-2002  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_const.h"
#include "g_debug.h"
#include "g_ui.h"
#include "g_report.h"
#include "g_secure.h"
#include "g_malloc.h"

static g_Boolean global_there_is_error = g_False ;

static void G_FUNCTION(ask_medium,(int answer))

  int i ;
  char *user_answer ;
  char uanswer ;

  printf("NNFS: Possible answers are\n"
	 "NNFS:     - '' RETURN for continue (normal operation)\n"
	 "NNFS:     - 'cancel' RETURN to abort medium operations\n"
	 ) ;
  fflush(stdout) ;

  for( i=0 ; i==0 ; g_Free(user_answer) )
    {
      user_answer = g_Read_Line(stdin) ;
      if ( user_answer == NULL )
	G_EXIT(1) ;
      i = !strcmp(user_answer, "cancel") ;
      if ( i )
	{
	  uanswer = G_ABORT_READING ;
	  if ( write(answer,&uanswer,1) != 1 )
	    PERROR("answer pipe stop") ;
	  printf("NNFS: I stop operation on medium\n") ;
	  G_PF("NNFS: I stop operation on medium\n") ;
	  continue ;
	}
      i = !strcmp(user_answer, "") ;
      if ( i )
	{
	  uanswer = G_CONTINUE ;
	  if ( write(answer,&uanswer,1) != 1 )
	    PERROR("answer pipe continue") ;
	  printf("NNFS: I continue operation on medium\n") ;
	  G_PF("NNFS: I continue operation on medium\n") ;
	  continue ;
	}
    }
}




void G_FUNCTION(g_Analyse,(char buf[G_LINE_CHUNK-3], int answer))

static char aliases[G_LINE_CHUNK] = "" ;
static char a_directory[G_LINE_CHUNK] ;
static char an_host[G_LINE_CHUNK] ;
static time_t mount_time ;
static char nb_mount[G_LINE_CHUNK] ;
static char display_bar_name[G_LINE_CHUNK] ;
static int display_bar_max ;
static int nb_hosts = 0 ;

unsigned long ul ;
int j ;
g_Nb s[9] ;
char command[G_LINE_CHUNK] ;
int nb, nbe ;
g_File_Info *f ;
char *name ;
int to_delete ;

G_PF("Code = %c\n",buf[0]) ;
switch(*buf++)
	{
	case G_HOST:
		strcpy(an_host,buf) ;
		break ;
	case G_NB_MOUNT:
		strcpy(nb_mount,buf) ;
		break ;
	case G_DIRECTORY:
		strcpy(a_directory,buf) ;
		break ;
	case G_LAST_MOUNT:
		sscanf(buf, "%lu", &ul) ;
		mount_time = (time_t)ul ;
		break ;
	case G_ALIAS:
		strcat(aliases, buf) ;
		strcat(aliases, ",") ;
		break ;
	case G_HOST_DONE:
		printf(FORMAT,an_host,a_directory,nb_mount,ctime(&mount_time)) ;
		if ( aliases[0] )
			printf("NNFS: Aliases : %s\n", aliases) ;
		aliases[0] = '\0' ;
		nb_hosts++ ;
		break ;
	case G_CURRENT_HOST:
		printf("NNFS: Current host is %s\n", buf) ;
		break ;
	case G_SET_DISPLAY_BAR:
		sscanf(buf, "%d", &display_bar_max) ;
		for(j=0;buf[j]!=' ';j++) /* empty */ ;
		strcpy(display_bar_name, &buf[j+1]) ;
		/* fall thru */
	case G_BAR:
		if ( buf[-1]==G_BAR )
			 sscanf(buf, "%d%d", &nb, &nbe) ;
		else
			nb = nbe = 0 ;
		printf("\rNNFS: %-30s ; %d/%d files, %d errors",
			display_bar_name,nb,display_bar_max,nbe) ;
		fflush(stdout) ;
		break ;
	case G_FINISH_BAR:
		sscanf(buf, "%d", &nbe) ;
		printf("\rNNFS: %-30s ; %d/%d files, %d errors\n",
			display_bar_name,display_bar_max,display_bar_max,nbe) ;
                if ( nbe )
                     global_there_is_error = g_True ;
		break ;
        case G_FLUSHING_DATA:
	        printf("NNFS: Flushing data on medium\n") ;
                break ;
        case G_CAN_NOT_WRITE:
	        printf("\nNNFS: %s\n", buf) ;
		printf("NNFS: Hit RETURN when problem is corrected") ;
		fflush(stdout) ;
		while( getchar() != '\n' ) { }
		if ( write(answer,"",1) != 1 )
		  PERROR("answer pipe flush") ;
                break ;
	case G_STATS:
		printf("\n") ;
		g_Print_Report(stdout) ;
		sscanf(buf, "%lu%lu%lu%lu%lu%lu%lu%lu%lu",
			s,s+1,s+2,s+3,s+4,s+5,s+6,s+7,s+8) ;
		printf(
       		   "NNFS: %6lu local files,           %6lu files on hosts\n"
       		   "NNFS: %6lu files from medium,     %6lu files to medium\n"
       		   "NNFS: %6lu remote file change,    %6lu local file change\n"
       		   "NNFS: %6lu update conflict,       %6lu file to delete\n"
       		   "NNFS: %lu bytes to compress and write on medium"
		    , s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8]) ;
		break ;
	case G_FILE:
	  f = (g_File_Info*)(buf) ;
	  name = (char*)&f->hosts[nb_hosts+1] ;
	  if ( name[0]=='.' )
	    {
	      name += 2 ;
	      to_delete = 0 ;
	    }
	  else
	    {
	      name++ ;
	      to_delete = 1 ;
	    }
	  if ( f->hosts[f->up_to_date].type != g_Is_A_Directory )
	    {
	      if ( (f->nnfs_to_local && !f->to_delete)
		   && (f->local_to_nnfs && !f->filtered) )
		g_Real_Report( name, 0, 'X') ;
	      else
		if ( f->nnfs_to_local && !f->to_delete )
		  g_Real_Report( name, 0, 'R') ;
		else
		  if ( f->local_to_nnfs && !f->filtered )
		    g_Real_Report( name, 0, 'W') ;
	    }
	  if ( f->conflict )
	    g_Real_Report( name, 1, 'C') ;
	  
	  if ( (f->to_delete && !to_delete) && f->nnfs_to_local )
	    g_Real_Report( name, 1, 'D') ;
	  break ;

	case G_QUESTION:
		printf("\n") ;
		for(;;)
		{
		printf("NNFS: Your choice: (E)xit, (C)ontinue, (V)iew details: ") ;
		fflush(stdout) ;
		j = tolower(getchar()) ;
		if ( getchar() != '\n' )
		  {
		    while( getchar() != '\n' ) /* empty */ ;
		    continue ;
		  }
		G_PF("j=%c\n", j) ;
		switch(j)
		   {
			case 'e':
			case 'E':
			        if ( write(answer,"e",1) != 1 )
				  PERROR("answer pipe exit") ;
				printf("NNFS: OK, I exit") ;
				G_PF("Answer E\n") ;
				G_RETURN(;) ;
			case 'c':
			case 'C':
			        if ( write(answer,"c",1) != 1 )
				  PERROR("answer pipe cont") ;
				printf("NNFS: OK, I continue the update") ;
				G_PF("Answer C\n") ;
				G_RETURN(;) ;
			case 'v':
			case 'V':
				sprintf(command,"more %s", buf) ;
				if ( system(command) )
				  PERROR(system) ;
				G_PF("Answer V\n") ;
				break ;
		   }
		}
		break ;
	case G_NNFSRC_CREATED:
	printf(
"NNFS: ``%s'' file doesn't exists.\n"
"NNFS: I create it with default values.\n"
"NNFS: So it is the first time you use NNFS2 on this connection directory.\n"
"NNFS: You can be in one of the following two cases :\n"
"NNFS:    1) It is the first time you use NNFS2.\n"
"NNFS:       So edit ``%s'' file to configure NNFS2.\n"
,buf, buf) ;
printf(
"NNFS:    2) NNFS2 had yet been configured on another host.\n"
"NNFS:       Overwrite the default ``nnfsrc'' file with the\n"
"NNFS:       file you have yet configured in another host.\n"
"NNFS: After the ``nnfsrc'' modification, run again ``nnfs2''\n"
"NNFS: It will give you more information.\n"
"NNFS:\n"
) ;
        break ;
	case G_HOST_NOT_IN_NNFS:
	printf(
"NNFS:----------------------------------------------------------------------\n"
"NNFS: To add an host to NNFS you must use ``--add'' option\n"
"NNFS: After the ``--add'' you indicate the local name of NNFS hierarchy\n"
"NNFS: If you want an identical connection directory, you could type:\n"
"NNFS:    nnfs2 --add $HOME\n"
"NNFS: Don't forget to run ``nnfs2'' command before and after working on a host\n"
) ;
printf(
"NNFS:----------------------------------------------------------------------\n"
"NNFS: If the current host access the same NFS files than a previous one\n"
"NNFS: you can add a nickname (alias) to an NNFS host.\n"
"NNFS: To do so, run 'nnfs2 --add-nickname existing_host.NIS_domain.user'\n"
"NNFS:----------------------------------------------------------------------\n"
) ;
        break ;

	default:
		G_EXIT(1) ;
	}
G_RETURN(;) ;
}





void G_FUNCTION(g_Ui_Txt,(int mainp, int medium, int error, int answer, pid_t fils,int *argc, char***argv, struct g_config*c, struct g_options *opt))

fd_set r,w,x ;
int max ;
char last_name[G_LINE_CHUNK] = "" ;
unsigned char buf[G_LINE_CHUNK] ;
int nb,nbt ;
int i ;
int exit_status ;

printf("NNFS: NNFS %s, No Warranty, GPL2\n", NNFS_VERSION) ;
printf(FORMAT,"Host.NIS.user","Directory", "#mnts", "Mount time\n") ;
g_Set_Report( g_Get_Lines()-8, g_Get_Columns(), 2) ;
g_Real_Report("(R)ead (W)rite (X)change: RW",0,' ');
g_Real_Report("(C)onflict (D)elete",1,' ');

max = mainp ;
if ( medium > max ) max = medium ;
if ( error > max ) max = error ;

FD_ZERO(&w) ;
for(;mainp>=0||medium>=0||error>=0;)
	{
	FD_ZERO(&r) ;
	if ( mainp>=0 ) FD_SET(mainp, &r) ;
	if ( medium>=0 ) FD_SET(medium, &r) ;
	if ( error>=0 ) FD_SET(error, &r) ;
	x = r ;
	G_PF("Before select\n") ;
	select(max+1, &r, &w, &x, 0) ;
	if ( error>=0 && (FD_ISSET(error,&r) || FD_ISSET(error,&x)) )
		{
		G_PF("Errors to read\n") ;
		nb = read(error,buf,G_LINE_CHUNK) ;
		if ( nb<=0 )
			{
			error = -1 ;
			continue ;
			}
		if ( write(2,buf,nb) != nb )
		  PERROR("stderr") ; /* LOL */
		G_PF("Errors printed\n") ;
		continue ;
		}
	if ( medium>=0 && (FD_ISSET(medium,&r) || FD_ISSET(medium,&x)) )
	   {
		G_PF("Medium to read\n") ;
		nb = read(medium,buf,G_LINE_CHUNK) ;
		if ( nb<=0 )
			{
			medium = -1 ;
			continue ;
			}
                if ( nb<4 )
                        {
                        G_PRINTF("Impossible case\n") ;
                        exit(51) ;
                        }
		buf[4] = '\0' ;
                if ( strcmp(last_name, ((char*)buf)+1 ) )
                    {
                       printf("\nNNFS: Insert medium %s for %s\n",
			  buf+1, buf[0]=='R' ? "read" : "write") ;

                       ask_medium(answer) ;
                    }
                else
                    {
                     if ( buf[0]=='R' )
                     {
		     printf("\nNNFS: Error reading medium."
                            " (Are you sure it is %s medium ?)\n"
			  ,buf+1) ;
                     ask_medium(answer) ;
                     }
                     else
                     {
                     printf("\nNNFS: Error writing medium (may be full),"
                            " insert a new %s medium\n",
			  buf+1) ;
                     ask_medium(answer) ;
                     }
                    }
                strcpy(last_name, ((char*)buf)+1) ;
		continue ;
		}
	if ( mainp<0 || !(FD_ISSET(mainp,&r) || FD_ISSET(mainp,&x)) )
		G_EXIT(1) ;

	G_PF("Stdin to read\n") ;
	nb = 0 ;
        nbt = 0 ; /* To avoid a compiler warning */
        i = -1 ; /* To avoid a compiler warning */
	do
		{
		if ( nb>=3 ) nbt = buf[1]*256+buf[2] ;
		while( nb<3 || nb<nbt )
			{
			i = read(mainp,&buf[nb], G_LINE_CHUNK-nb) ;
			if ( i<=0 )
				break ;
			if ( buf[0]!='\0' )
				{
				G_PRINTF("(((%s)))",buf) ;
				G_EXIT(1) ;
				}
			nb += i ;
			if ( nb>=3 ) nbt = buf[1]*256+buf[2] ;
			}
		if ( i<=0 )
			break ;
		G_PF("Stdin to read nb=%d nbt=%d\n", nb, nbt) ;
		g_Analyse((char*)&buf[3],answer) ;
		memmove(buf,&buf[nbt],nb-nbt) ;
		nb -= nbt ;
		}
	while( nb != 0 ) ;
	G_PF("End the reading of information block\n") ;

	if ( i<=0 )
		{
		mainp = -1 ;
                waitpid(fils, &exit_status, WUNTRACED);
                exit_status = WEXITSTATUS(exit_status) ;
		G_PF("mainp ended\n") ;
		continue ;
		}
	G_PF("End of stdin read\n") ;
	}

if ( global_there_is_error )
{
printf("NNFS: ************************************************************\n"
       "NNFS: ************************************************************\n"
       "NNFS: THERE IS AN ERROR (disk full, permission denied, ...)\n"
       "NNFS: ************************************************************\n"
       "NNFS: ************************************************************\n"
      ) ;
}


exit(exit_status) ;
}

