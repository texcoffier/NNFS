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

/*
 * Spliter and Unspliter need "popen" function.
 * So it is UNIX world.
 */

#include "g_const.h"
#include "g_debug.h"
#include "g_secure.h"
#include "g_malloc.h"
#include "g_split.h"
#include "g_unix.h"

#include <stdio.h>
#include <signal.h>
#if STDC_HEADERS
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

/*
 *
 */

static char medium_in_the_floppy[G_LINE_CHUNK] ;

/*
 *
 */

void G_FUNCTION(g_Incr,(char* s))

int i ;

i = strlen(s) - 1 ;
G_PF("Incr (%s) Len=%d\n", s, i) ;
while( i>=0 )
	{
	if ( s[i] != 'Z' )
		{
		s[i]++ ;
		G_RETURN(;) ;
		}
	s[i] = 'A' ;
	i-- ;
	}
G_EXIT(1) ;
}

/*
 *
 */

static void not_the_good_medium()
{
  strcpy(medium_in_the_floppy, "---") ;
}

static void no_medium()
{
  strcpy(medium_in_the_floppy, "000") ;
}

static void name_medium(const char *name)
{
  strcpy(medium_in_the_floppy, name) ;
}

/*
 *
 */

static void eject_medium_from_the_floppy(const char *eject)
{
  char *b ;

  if ( strcmp(medium_in_the_floppy, "000") == 0 )
    return ;
  
  b = g_Formatted_Strings(eject,
			  medium_in_the_floppy,
			  medium_in_the_floppy,
			  medium_in_the_floppy) ;
  system(b) ;
  g_Free(b) ;
  not_the_good_medium() ;
}

/*
 * Returns 0 if medium not loaded
 */

static int G_FUNCTION(load_medium_inside_the_floppy,
		       (const g_Config *config,
			int medium,
			char *name, g_Boolean for_read))
     int c ;

  if ( strcmp(name, medium_in_the_floppy)==0 )
    G_RETURN(1) ;

  G_PF("Current medium in floppy : %s, asked medium %s\n",
       medium_in_the_floppy, name) ;

  eject_medium_from_the_floppy(config->eject) ;
  G_PF("Medium ejected\n") ;
  if ( config->ask_medium )
    {
      G_PF("Name = (%s)\n", name) ;
      name[strlen(name)-4] = for_read ? 'R' : 'W' ;
      G_PF("Name = (%s) medium=%d strlen=%d\n", name, medium, strlen(name)) ;
      if ( write(medium, name+strlen(name)-4, 4) != 4 )
	G_EXIT(1) ;
      G_PF("Name = (%s)\n", name) ;
      name[strlen(name)-4] = '.' ;
      G_PF("Wait answer\n") ;
      c = getchar() ;
      if ( c == G_CONTINUE )
	{
	  name_medium(name) ;
	  G_RETURN(1) ;
	}
      if ( c == G_ABORT_READING )
	G_RETURN(0) ;

      G_EXIT(1) ;
    }
  G_PF("Name medium\n") ;
  name_medium(name) ;

  G_RETURN(1) ;
}

/*
 *
 */

static int stop_flag ;

void g_set_stop_flag(int i)
{
  stop_flag = 1 ;
}


g_Boolean G_FUNCTION(g_Split_Input,(FILE* w, const g_Config *config, int medium))

char buf[G_LINE_CHUNK] ;
char name[G_LINE_CHUNK] ;
char *b ;
int i ;
int sum ;
FILE *r ;
g_Boolean multiple ;

stop_flag = 0 ;
g_Signal(g_Signal_User1,g_set_stop_flag) ;
r = NULL ;
sum = 0 ; /* Remove a checker error */
sprintf(name, "%s.AAA", config->medium_name) ;
multiple = g_False ;
name_medium(name) ;
for(;!stop_flag;)
   {
     G_PF("r=%p, sum=%d\n",r,sum) ;
     /*
      * Open medium if not yet opened
      */
     if ( r == NULL )
       {
         sum = 0 ;
	 if ( load_medium_inside_the_floppy(config, medium, name, g_True)==0 )
            {
              G_RETURN(multiple) ;
            }
	 b = g_Formatted_Strings(config->read_medium, name,name,name);
	 r = g_popen(b, "r") ;
	 g_Free(b) ;
       }
     /*
      * Read medium
      */
     i = fread(buf, 1, MINIMUM(G_LINE_CHUNK, config->medium_size-1-sum), r) ;
     G_PF("must read %d, read %d\n",MINIMUM(G_LINE_CHUNK, config->medium_size-1-sum), i) ;
     /*
      * Write in the pipe
      */
     if ( i>0 )
       {
       if ( fwrite(buf, 1, i, w) != i )
		{
		/* Possible if NNFS don't read the medium
		 * because it knowns there is nothing new.
		 * So it closes the pipe
		 */
		 break ;
		}
       sum += i ;
       }
     /*
      * If end of medium read
      */
     if ( feof(r) )
       {
         G_PF("End of File, i=%d, sum=%d\n",i,sum) ;
	 pclose(r) ; /* Don't verify error return code */
	 r = NULL ;
	 if ( sum==0 && config->ask_medium )
	   {
	     /*
	      * If 0 byte read : the medium is the bad one
	      * So, ask the good medium
	      */
	     not_the_good_medium() ;
	     continue ;
	   }
         fflush(w) ;
     	 break ;
       }
     /*
      * If a complete medium had been readed
      */
     if ( sum == config->medium_size - 1 )
       {
	 i = fread(buf, 1, 1, r) ;
	 if ( feof(r) )
	   {
	     break ;
	   }
         if ( i != 1 ) 
	   {
	     G_PRINTF("Impossible case...\n") ;
	     break ;
	   }
	 if ( buf[0] != '+' )
	   {
	     G_PRINTF("Impossible case...\n") ;
	     break ;
	   }
	 G_PF("Before\n") ;
	 g_pclose(r) ;
	 G_PF("After\n") ;
	 r = NULL ;
	 g_Incr(name) ;
	 multiple = g_True ;
       }
   }
G_PF("No more input or stopped\n") ;
G_RETURN(multiple) ;
}

/*
 *
 */

void G_FUNCTION(g_Split_Output,(FILE* r, const g_Config *config, g_Boolean multiple, g_Boolean format, int medium))

char buf[G_LINE_CHUNK] ;
char name[G_LINE_CHUNK] ;
char *b ;
int i ;
int sum ;
FILE *w ;
char *mem ;

w = NULL ;
sum = 0 ;
sprintf(name, "%s.AAA", config->medium_name) ;
G_PF("multiple = %d\n", multiple) ;
if ( multiple )
    not_the_good_medium() ;
else
   if ( format )
      no_medium() ;
   else
      name_medium(name) ;

G_PF("--------------\n") ;

if ( config->tmp_file == 1/* MEmory */ )
   {
     mem = g_Malloc(config->medium_size) ;
   }
else
   {
     mem = NULL ;
   }

G_PF("--------------\n") ;

for(;;)
  {
  /*
   * If not output file : open one
   */
  G_PF("--------------\n") ;
  if ( w == NULL )
  	{
        G_PF("sum=%d mem=%d\n", sum, mem) ;
        if ( sum!=0 && mem==NULL )
                G_RETURN(;) ;
        if ( load_medium_inside_the_floppy(config, medium, name, g_False)==0 )
            {
              G_RETURN(;) ;
            }
  	b = g_Formatted_Strings(config->write_medium,name,name,name ) ;
        G_PF("open a medium with : %s\n", b) ;
  	w = g_popen(b, "w") ;
        G_PF("After popen (((sum=%d)))\n", sum) ;
  	g_Free(b) ;
        if ( sum!=0 )
           {
                if ( fwrite(mem, 1, sum, w) != sum )
                   goto write_error ;
           }
        }
  /*
   * Read pipe
   */
  G_PF("Before read\n") ;
  i = fread(buf, 1, MINIMUM(G_LINE_CHUNK, config->medium_size-1-sum),r) ;
  G_PF("Read %d byte from pipe, sum=%d\n", i, sum) ;
  /*
   * Write on medium
   */
  G_PF("Write %d byte on medium\n", i) ;
  if ( i<0 )
     G_EXIT(1) ;
  if ( i>0 )
     {
     if ( mem )
        memcpy(mem+sum, buf, i) ;
     sum += i ;
     if ( fwrite(buf, 1, i, w) != i )
      {
      G_PF("Problem when writing\n") ;
      if ( mem==NULL )
         G_EXIT(1) ;
      /*
       * Eject and ask to insert a new medium
       * Then, write again the start of the buffer
       */
write_error:
       pclose(w) ;
write_error2:
       G_PF("write_error2\n") ;
       if ( !config->ask_medium )
           G_RETURN(;) ;
       not_the_good_medium() ;
       w = NULL ;
       continue ;
      }
     }
  /*
   * If nothing to read : quit the function
   */
  G_PF("Verify if end of input file\n") ;
  if ( feof(r) )
  	{
          G_PF("End of input file\n") ;
  	  if ( pclose(w) )
              {
              goto write_error2 ;
              }
          G_PF("Eject last medium\n") ;
          eject_medium_from_the_floppy(config->eject) ;
          G_PF("No more to write on medium\n") ;
  	  G_RETURN(;) ;
  	}
  /*
   * Verify if medium is full
   */
  G_PF("Verify if medium full\n") ;
  if ( sum == config->medium_size - 1 )
  	{
        i = fgetc(r) ;
        if ( i>=0 )
           {
           G_PF("Not the last medium\n") ;
           ungetc(i, r) ;
  	   if ( fwrite("+", 1, 1, w) != 1 )
              goto write_error ;
           G_PF("+ added at the medium end\n") ;
           }
        else
           G_PF("Last medium\n") ;
        G_PF("Before pclose\n") ;
  	if ( pclose(w) )
           goto write_error2 ;
        G_PF("after last medium test name=%s\n", name) ;
  	w = NULL ;
        sum = 0 ;
        G_PF("Before g_Incr\n") ;
	g_Incr(name) ;
        G_PF("End if\n") ;
  	}
  G_PF("End for\n") ;
  }
}


