/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1997-1998  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#include "g_ui.h"
#include "g_secure.h"

/*
User Interface,
	mainp: informations from main program
	medium: informations from medium read or write (medium name: 3char)
	error: error messages of all the programs
	answer: Used to restart one of the 3 programs

	from main process, messages starts by \000\len1\len2 :
		"S%d %s", maximum_display_bar_size, display_bar_name
		"C%d %d",current_value_of_display_bar, current_value_of_error

		"F%d %d %s", column, error, file_name
		"H%s", hostname
		"N%d", number_of_mount
		"D%s", directory
		"L%d", date_of_last_mount
		"A%s", alias
		"C%s", current host
		"R%d %d %d %d %d %d %d %d %d", statistics
		"?" (no argument : ask question)
		other : error of format

It displays (for the text interface) :

Read files ; 0000000000
Insert medium AAA and press return
Read files ; 000000000000000000000000000000...
*/

int g_Ui( void (*interface)(int,int,int,int,pid_t,int *, char***, g_Config *, g_Options *opt), int *argc, char ***argv, g_Config *c, g_Options *opt)
{
int mainp[2], error[2], answer[2], medium[2] ;
pid_t fils ;

g_pipe(mainp) ;
g_pipe(error) ;
g_pipe(answer) ;
g_pipe(medium) ;

fils = g_fork() ;
if ( fils!=0 )
	{
	/* In father */
	close(answer[0]) ;
	close(mainp[1]) ;
	close(error[1]) ;
	close(medium[1]) ;
	(*interface)(mainp[0], medium[0], error[0], answer[1], fils, argc, argv, c, opt) ;
	}
else
	{
	dup2(answer[0],0) ;
	dup2(mainp[1],1) ;
	dup2(error[1],2) ;
	close(answer[1]) ;
	close(mainp[0]) ;
	close(error[0]) ;
	close(medium[0]) ;
	}

return(medium[1]) ;
}




