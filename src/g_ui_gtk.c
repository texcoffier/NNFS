/*    NNFS: Non-Networked File System.
 *    Copyright (C) 1998-2005  Thierry EXCOFFIER (exco@ligim.univ-lyon1.fr)
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

#define COPYRIGHT "©" /* UTF8 */
/* Define the maximum number of file displayed in the question page */
#define MAX_ELEMENT_IN_LIST 1000

#include <locale.h>
#include <gtk/gtk.h>

#include "g_ui.h"
#include "g_const.h"
#include "g_debug.h"
#include "g_config.h"
#include "g_malloc.h"
#include "g_nnfsrc.h"
#include "g_filter.h"
#include "g_update.h"
#include "g_secure.h"
#if HAVE_STRING_H
#include <string.h>
#endif

#define NH 16 /* Number of hosts */
#define HOST pages[search_page("Hosts")]

static int ganswer, gerror ;
static int exit_status ;
static pid_t gfils ;
static GtkWidget *notebook, *window, *continue_button ;
static GtkWidget *skip_reading, *ok_medium, *wquit ;
static GtkWidget *format_button, *configure_button ;
static GtkWidget *add_nickname_but[NH] = { NULL } ;
static GtkWidget *can_not_write = NULL ;
static int id_error=0, id_main=0, id_medium=0 ;
static GtkTooltips *tips ;
static int nb_hosts = 0 ;

static g_Config *gconfig ;
static g_Options *goptions ;

static g_Boolean global_there_is_error = g_False ;

struct pages
{
  char *name ;
  char *full_name ;
  char *tip ;
  GtkWidget *(*create)(int i) ;
  GtkWidget *eb ;
  GtkWidget *frame ;
  GtkWidget *content ;		/* Usualy vbox */
  GtkWidget *table, *label, *cancel, *ok, *tables[10], *button ;
  GtkListStore  *list ;
  GtkWidget *menu ;
  guint signal ;
} ;

extern struct pages pages[] ;

int search_page(char *name) ;

/*
 *
 */

void generic_goto_page(char *name, int set)
{
  int i ;
  static int old_i = -1 ;
  char buf[G_LINE_CHUNK] ;

  i = search_page(name) ;
  if ( set )
    if ( old_i != i )  
      {
	if ( old_i != -1 )
	  {
	    gtk_widget_set_name(pages[old_i].eb, "") ;
	  }
	gtk_widget_set_name(pages[i].eb, "current") ;
	old_i = i ;
	
	sprintf(buf, "NNFS2: %s", pages[i].full_name) ;
	gtk_window_set_title(GTK_WINDOW(window), buf) ;
      }
  
  gtk_notebook_set_page( GTK_NOTEBOOK(notebook), i) ;
}

void user_goto_page(GtkWidget *w, void* x, int page)
{
  gtk_notebook_set_page( GTK_NOTEBOOK(w->parent), page) ;
}

void goto_page(char *name)
{
  generic_goto_page(name, 1) ;
}

/*
 *
 */

void remove_end(int *id)
{
  gdk_input_remove(*id) ;
  *id = 0 ;
  if ( id_main==0 && id_error==0 && id_medium==0 )
    {
      waitpid(gfils, &exit_status, WUNTRACED);
      exit_status = WEXITSTATUS(exit_status) ;
      gtk_label_set(GTK_LABEL(pages[search_page("Bye Bye")].content),
		    (exit_status || global_there_is_error) ?
		    "*************************************************\n"
		    "*************************************************\n"
		    "*************************************************\n"
		    "Errors !\n"
		    "Look more closely the informations displayed.\n"
                    "It could come from a full disk, a permission denied, ...\n"
		    "*************************************************\n"
		    "*************************************************\n"
		    "*************************************************\n"
		    :
		    "Update completed without error.\n"
		    "Thank you for using NNFS2\n"
		    "You can click on `Quit' to terminate user interface") ;
      gtk_widget_set_name(wquit, "normal") ;
      goto_page("Bye Bye") ;
    }
}

/*
 *
 */

void error_function(gpointer data, gint source, GdkInputCondition condition)
{
  gchar *str ;
  GtkWidget *error ;
  int nb ;
  char buf[G_LINE_CHUNK] ;
  char *buf2, *name_utf8 ;
  gsize bytes_read, bytes_written ;

  error = (GtkWidget*)data ;

  nb = read(source, buf, G_LINE_CHUNK-1) ;
  if ( nb<=0 )
    {
      remove_end(&id_error) ;
    }
  else
    {
      buf[nb] = '\0' ;
      g_Printf("%s", buf) ;


      name_utf8 = g_locale_to_utf8(buf, -1, &bytes_read, &bytes_written, NULL);
      if ( name_utf8 == NULL )
	name_utf8 = strdup("Character translation failed, Check LC_ALL, LANG, ...") ;


      gtk_label_get (GTK_LABEL(error), &str) ;
      buf2 = g_Malloc( strlen(str) + strlen(name_utf8) + 1 ) ;
      sprintf(buf2, "%s%s", str, name_utf8) ;

      gtk_label_set (GTK_LABEL(error), buf2) ;
      g_Free(buf2) ;
      g_free(name_utf8) ;
    }
}

GtkWidget *error_page()
{
  GtkWidget *frame, *scrolled_window, *werror ;

  frame = gtk_frame_new("Error Messages") ;

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_border_width (GTK_CONTAINER (scrolled_window), 5);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  werror = gtk_label_new ("");
  gtk_label_set_justify(GTK_LABEL(werror), GTK_JUSTIFY_LEFT) ;
  /*  gtk_widget_set_usize(werror, 1000,100) ; */

  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), werror) ;
  

  gtk_widget_show (werror);

  id_error = gdk_input_add (gerror, GDK_INPUT_READ, error_function, (gpointer)werror) ;
  gtk_widget_show(scrolled_window) ;

  gtk_container_add (GTK_CONTAINER (frame), scrolled_window);
  gtk_widget_show(frame) ;
  

  return(frame) ;
}

/*
 *
 */

void unsensitivize_all()
{
  int i ;

  gtk_widget_set_sensitive(continue_button, FALSE) ;
  if ( format_button )
    {
      gtk_widget_destroy(format_button) ;
      format_button = NULL ;
    }

  if ( configure_button )
    {
      gtk_widget_destroy(configure_button) ;
      configure_button = NULL ;
    }

  for(i=0;i<NH;i++)
    add_nickname_but[i] = NULL ;
      

  gtk_label_set( GTK_LABEL(HOST.label), "") ;
}

void empty_table()
{
  GList *l, *ll ;

  ll = gtk_container_children(GTK_CONTAINER(HOST.table)) ;
  for(l = ll ; l ; l = l->next )
    gtk_widget_destroy( GTK_WIDGET(l->data) ) ; 
  g_list_free(ll) ;
  nb_hosts = 0 ;
}

void add_nickname(GtkWidget *widget, gpointer data)
{
  char c[3] ;

  c[0] = G_ADD_NICKNAME ;
  c[1] = (int)data+1 ;
  c[2] = 0 ;
  write(ganswer, c, 3) ;
  unsensitivize_all() ;
  empty_table() ;
}

/*
 *
 */

#define VO ((GtkAttachOptions)0)

void fill_td(GtkWidget *table, int l, int c, char *txt, int left, int nbc)
{
  GtkWidget *label;
  GtkWidget *box;

  label = gtk_label_new(txt) ;
  box = gtk_alignment_new(left?0:1,0.5,0.,0.) ;
  gtk_container_add (GTK_CONTAINER (box), label);

  gtk_widget_show(label) ;
  gtk_widget_show(box) ;

  gtk_table_attach (GTK_TABLE (table), box, c,c+nbc,l,l+1,GTK_FILL,VO,5,0);
}


void fill_table(GtkWidget *table,
		int l, char *host, char *dir, char *nb, char *date, char *ch)
{
  GtkWidget *but ;
  char tmp[G_LINE_CHUNK] ;

  fill_td(table, l,0,host, 0,1) ;
  fill_td(table, l,1,dir, 1,1) ;
  fill_td(table, l,2,nb, 0,1) ;
  fill_td(table, l,3,date, 1,1) ;
  fill_td(table, l,3,date, 1,1) ;

  if ( l<2 )
    return ;

  but = gtk_button_new_with_label("+Nickname") ;
  sprintf(tmp,
	  "To add the current host (%s) as a nickname "
	  "of the host: %s.\n"
	  "The two hosts must access the same files "
	  "by the same directory name (%s).",
	  ch, 
	  host,
	  dir
	  ) ;
  gtk_tooltips_set_tip(tips, but, tmp, NULL) ;
  gtk_signal_connect (GTK_OBJECT (but), "clicked",
		      GTK_SIGNAL_FUNC (add_nickname), (gpointer)(l-2));
  gtk_table_attach (GTK_TABLE (table),
		    but, 4,5,l,l+1,GTK_FILL,VO,0,0);
  add_nickname_but[l-2] = but ;
}


void configure_function()
{
}

void format_function()
{
  char c ;

  unsensitivize_all() ;
  c = G_FORMAT ;
  write(ganswer,&c,1) ;
}

GtkWidget *frame_label_button(char* frame_name, char *label_content,
			      char *button_name, void (*fct)(), int text,
			      char *widget_name)
{
  GtkWidget *frame, *vbox, *label, *alignment, *button ;

  label = gtk_label_new(label_content) ;
  /*  gtk_label_set_justify(label, GTK_JUSTIFY_FILL) ; */
  gtk_widget_show(label) ;

  if ( fct )
    {
      if ( text )
	{
	  button = gtk_entry_new() ;
	  gtk_entry_set_text (GTK_ENTRY (button), button_name) ;
	  gtk_signal_connect (GTK_OBJECT (button), "activate",
			      GTK_SIGNAL_FUNC (fct), NULL);
	}
      else
	{
	  button = gtk_button_new_with_label(button_name) ;
	  gtk_widget_set_name(button, widget_name) ;
	  gtk_signal_connect(GTK_OBJECT (button),"clicked",
			     GTK_SIGNAL_FUNC(fct),NULL);
	  gtk_widget_show(button) ;

	  alignment = gtk_alignment_new(0.5,0.,0.,0.) ;
	  gtk_container_add(GTK_CONTAINER(alignment), button) ;

	  button = alignment ;
	}
      gtk_widget_show(button) ;	  

      vbox  = gtk_vbox_new(0,0) ;
      gtk_box_pack_start(GTK_BOX(vbox), label, 0, 0, 0) ;
      gtk_box_pack_start(GTK_BOX(vbox), button, 0, 0, 5) ;
      gtk_widget_show(vbox) ;

      label = vbox ;
    }
    
  
  frame = gtk_frame_new(frame_name) ;
  gtk_container_border_width(GTK_CONTAINER(frame),5) ;
  gtk_container_add(GTK_CONTAINER(frame), label) ;
  gtk_widget_show(frame) ;

  return(frame) ;
}

/*
 *
 */

GtkWidget *notebook_label(const char *labeltext, int page, const char *tip)
{
  GtkWidget *label, *eb ;

  label = gtk_label_new (labeltext);
  gtk_widget_show(label) ;

  eb = gtk_event_box_new() ;
  gtk_signal_connect (GTK_OBJECT (eb), "button_press_event",
		      (GtkSignalFunc) user_goto_page,
		      (gpointer)page);   
  gtk_container_add( GTK_CONTAINER(eb), label);
  gtk_tooltips_set_tip(tips, eb, tip, NULL) ;
  return(eb) ;
}

/*
 *
 */

GtkWidget *host_page(int i)
{
  GtkWidget *vbox, *table, *label ;


  vbox = gtk_vbox_new(0,0) ;

  table = gtk_table_new (5, 2*NH+3, 0) ; /* 2 * nb_hosts */

  label = gtk_label_new("=") ;
  gtk_widget_show(label) ;
  pages[i].label = label ;

  gtk_box_pack_start(GTK_BOX(vbox), table, 0,0,0) ;
  gtk_box_pack_start(GTK_BOX(vbox), label, 0,0,0) ;


  format_button = frame_label_button
    ("Do not read medium"
     ,
     "If you press this button, "
     "NNFS2 will create a new file system on medium.\n"
     "It will overwrite existing data on medium."
     ,
     "Continue without reading the medium"
     ,
     format_function, 0, "dangerous"
     ) ;
  gtk_box_pack_start(GTK_BOX(vbox), format_button, 0,0,0) ;

  /*
  configure_button = frame_label_button
    ("Configure NNFS2"
     ,
     "If you press this button, "
     "you will be teleported on configuration page.\n"
     "You can come back here later."
     ,
     "Configure NNFS2"
     ,
     configure_function, 0
     ) ;
  gtk_box_pack_start(GTK_BOX(vbox), configure_button, 0,0,0) ;
  */

  gtk_widget_show(table) ;
  pages[i].table = table ;

  return(vbox) ;
}

/*
 *
 */

GtkWidget *preprocessing_page(int i)
{
  GtkWidget *table ;

  table = gtk_table_new (4, 10, 0) ;
  gtk_container_border_width( GTK_CONTAINER(table), 5) ;

  return(table) ;
}

/*
 *
 */

GtkWidget *welcome_page(int i)
{
  GtkWidget *label ;
  char *c ;

  c = g_Formatted_Strings("% %", /* the space should be here */
     "NNFS: Non-Networked File System.\n"
     NNFS_VERSION "\n"
     "Copyright (C) 1995-2005  Thierry EXCOFFIER (exco@liris.univ-lyon1.fr)\n"
     "\n"
     "This program is free software; you can redistribute it and/or modify\n"
     "it under the terms of the GNU General Public License as published by\n"
     "the Free Software Foundation; either version 2 of the License, or\n"
     "(at your option) any later version.\n"
     "\n"
			  ,
     "This program is distributed in the hope that it will be useful,\n"
     "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
     "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
     "GNU General Public License for more details.\n"
     "\n"
     "You should have received a copy of the GNU General Public License\n"
     "along with this program; if not, write to the Free Software\n"
     "Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."
			  ) ;

  label = gtk_label_new(c) ;
  g_Free(c) ;
  return(label) ;
}

/*
 *
 */

/*
 *
 */
static gint button_press (GtkWidget *widget, GdkEvent *event)
{

    if (event->type == GDK_BUTTON_PRESS) {
        GdkEventButton *bevent = (GdkEventButton *) event; 
        gtk_menu_popup (GTK_MENU(widget), NULL, NULL, NULL, NULL,
                        bevent->button, bevent->time);
        /* Tell calling code that we have handled this event; the buck
         * stops here. */
        return TRUE;
    }

    /* Tell calling code that we have not handled this event; pass it on. */
    return FALSE;
}

void set_config_widget(char *named) ;

void load_config(/*GtkWidget *widget, */gpointer data)
{
  set_config_widget(data) ;
}

void realize_text (GtkWidget *text, gpointer data)
{
  gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (text))
			    , data, -1);
  /*
  gtk_text_freeze (GTK_TEXT (text));
  gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL, data, -1) ;
  gtk_text_set_point(GTK_TEXT(text), 0) ;
  gtk_text_thaw (GTK_TEXT (text));
  */
}

GtkWidget* create_texts(char *t[10][2])
{
  GtkWidget *menu, *item ;
  int k ;

  menu = gtk_menu_new();
  for(k=0;t[k][0][0];k++)
    {
      item = gtk_menu_item_new_with_label(t[k][1]);
      gtk_menu_append(GTK_MENU (menu), item);
      gtk_widget_show(item);
    }
  return(menu) ;
}

void fill_a_case(GtkWidget *table, GtkWidget *w, int line, int column,
		 const char *tip)
{
  gtk_tooltips_set_tip(tips, w, tip, NULL) ;
  gtk_table_attach(GTK_TABLE(table), w
		   , column , column+1
		   , line   , line+1
		   ,(GtkAttachOptions)(GTK_FILL |( (column==7)?GTK_EXPAND:0))
		   ,VO,0,0
		   ) ;
  gtk_object_set_data(GTK_OBJECT(w), "nnfs2_l", (gpointer)line) ;
  gtk_object_set_data(GTK_OBJECT(w), "nnfs2_c", (gpointer)column) ;
  gtk_widget_show(w) ;
}


GtkWidget* output_filter(char *t)
{
  GtkWidget *table ;
  g_Filter *f, *a ;
  int i, j, line ;
  GtkWidget *w, *menu, *label ;
  char buf[G_LINE_CHUNK] ;
  guint width4 = 32 ;
  char *c ;
  static char *ar[][2] =
  {
    {"+", "+"},
    {"-", "-"},
    {""}
  } ;
  char *tmp_t ;
  char *types[g_Is_Any][2] ;
  static char *lab[] = { "what", "perm", "+perm", "-perm", "size <", "size >", "File Type", "Regex to compare to the full file name", NULL} ;
  static g_Type_Of_File tf[] = { 
		g_Is_A_Directory,
		g_Is_A_Link,
		g_Is_A_File,
                g_Is_A_FIFO,
		g_Is_Any
  	} ;


  for(i=0;i<sizeof(tf)/sizeof(tf[0]);i++)
    {
      types[i][0] = types[i][1] = g_Type_To_String(tf[i]) ;
    }
  types[i][0] = "" ;

  G_MALLOC(tmp_t, strlen(t) + 10) ;
  sprintf(tmp_t, "%s%s-\n-\n-\n", t, t[strlen(t)-1]=='\n' ? "" : "\n") ;
  f = g_Compile_Filter(tmp_t) ;
  g_Free(tmp_t) ;

  table = gtk_table_new(6,10,0) ;

  for(i=0;lab[i];i++)
    {
      label = gtk_label_new(lab[i]) ;
      gtk_widget_show(label) ;
      gtk_table_attach(GTK_TABLE(table), label, i,i+1, 0,1,VO,VO,4,0) ;
    }

  for( a=f, line=1 ; a ; a=a->next, line++)
    {
      w = gtk_option_menu_new() ;
      gtk_option_menu_set_menu(GTK_OPTION_MENU(w), create_texts(ar));
      gtk_option_menu_set_history(GTK_OPTION_MENU(w), a->what) ;
      c = g_Formatted_Strings("% %", /* the space should be here */
	 "The lines are evaluated from first to last. "
	 "On the first line all the files should be saved on medium."
	 "\n"
	 "The '-' indicates that the files verifying the conditions in "
	 "the line should not be saved on medium."
	 "\n"
			      ,
	 "The '+' indicates that the files verifying the conditions in "
	 "the line should be saved on medium."
	 "\n"
	 "The same file could be removed by a test on a line but "
	 "reinserted by a following line. In the default configuration, "
	 "executable file bigger than 50Kb are not saved but the next line "
	 "indicates that files named 'configure' should be saved."
			      ) ;
      fill_a_case(table, w, line, 0, c) ;
      g_Free(c) ;


      w = gtk_entry_new() ;
      if ( a->perm == -1 )
	buf[0] = '\0' ;
      else
	sprintf(buf, "%04o", a->perm) ;
      gtk_entry_set_text (GTK_ENTRY (w), buf) ;
      gtk_widget_set_usize (w, width4, -1) ;
      gtk_entry_set_max_length(GTK_ENTRY(w),4) ;
      fill_a_case(table, w, line, 1,
		  "The condition is verified if the file mode is "
		  "exactly equal to the value indicated in octal"
		  ) ;


      w = gtk_entry_new() ;
      if ( a->plus_perm != ~0 )
	sprintf(buf, "%04o", a->plus_perm) ;
      else
	buf[0] = '\0' ;
      gtk_entry_set_text (GTK_ENTRY (w), buf) ;
      gtk_widget_set_usize (w, width4, -1) ;
      gtk_entry_set_max_length(GTK_ENTRY(w),4) ;
      fill_a_case(table, w, line, 2,
		  "The condition is verified if any bit indicated in octal "
		  "are in the file mode. If you indicate 0111 the file mode "
		  "0100, 0111, 0750 are verified."
		  ) ;


      w = gtk_entry_new() ;
      if ( a->minus_perm != 0 )
	sprintf(buf, "%04o", a->minus_perm) ;
      else
	buf[0] = '\0' ;      
      gtk_entry_set_text (GTK_ENTRY (w), buf) ;
      gtk_widget_set_usize (w, width4, -1) ;
      gtk_entry_set_max_length(GTK_ENTRY(w),4) ;
      fill_a_case(table, w, line, 3,
		  "The condition is verified if all the bits indicated "
		  "in octal are in the file mode. If you indicate 0111 "
		  "the file mode 0755 is verified but not 0750."
		  ) ;


      w = gtk_entry_new() ;
      if ( a->t_minus_size )
	sprintf(buf, "%s", a->t_minus_size) ;
      else
	buf[0] = '\0' ;
      gtk_entry_set_text (GTK_ENTRY (w), buf) ;
      gtk_widget_set_usize (w, width4, -1) ;
      gtk_entry_set_max_length(GTK_ENTRY(w),5) ;
      fill_a_case(table, w, line, 4,
		  "The condition is verified if the file size is smaller "
		  "than the size indicated. The prefixes may be 'k' ou 'm' "
		  "to indicate kilo and mega bytes."
		  ) ;


      w = gtk_entry_new() ;
      if ( a->t_plus_size )
	sprintf(buf, "%s", a->t_plus_size) ;
      else
	buf[0] = '\0' ;
      gtk_entry_set_text (GTK_ENTRY (w), buf) ;
      gtk_widget_set_usize (w, width4, -1) ;
      gtk_entry_set_max_length(GTK_ENTRY(w),5) ;
      fill_a_case(table, w, line, 5,
		  "The condition is verified if the file size is bigger "
		  "than the size indicated. The prefixes may be 'k' ou 'm' "
		  "to indicate kilo and mega bytes."
		  ) ;


      w = gtk_option_menu_new() ;
      menu = create_texts(types) ;
      gtk_option_menu_set_menu(GTK_OPTION_MENU(w), menu);
      for(j=0; tf[j]!= a->type ; j++)
	{
	}
      gtk_option_menu_set_history(GTK_OPTION_MENU(w), j) ;
      fill_a_case(table, w, line, 6, "Choose a file type") ;


      w = gtk_entry_new() ;
      if ( strcmp(a->regex,".*") != 0 )
	sprintf(buf, "%s", a->regex) ;
      else
	buf[0] = '\0' ;
      gtk_widget_set_name(w, "program") ;      
      gtk_entry_set_text (GTK_ENTRY (w), buf) ;
      gtk_entry_set_position(GTK_ENTRY(w), 0) ;
      fill_a_case
	(table, w, line, 7,
	 "The condition is verified if the regular expression is verified.\n"
	 "File names start by ./"
	 ) ;
    }
  g_Free_Filter(f) ;
  return(table) ;
}

void retrieve_filter_values(GtkWidget *w, char *m[100][8])
{
  int line, column ;
  char *s ;

  line = (int)gtk_object_get_data(GTK_OBJECT(w), "nnfs2_l") ;
  if ( line < 1 )
    return ;
  column = (int)gtk_object_get_data(GTK_OBJECT(w), "nnfs2_c") ;

  if  ( GTK_IS_EDITABLE(w) )
    {
      s = gtk_editable_get_chars(GTK_EDITABLE(w), 0, -1) ;
      m[line-1][column] = g_Save_String(s) ;
      g_free(s) ;
      return ;
    }
  if ( GTK_IS_OPTION_MENU(w) )
    {
      gtk_label_get(GTK_LABEL(GTK_BIN(w)->child), &s) ; 
      m[line-1][column] = g_Save_String(s) ;
      return ;
    }
  g_Printf("Abort\n") ;
  abort() ;
}

void retrieve_values(GtkWidget *w, void *unused)
{
  int m ;
  g_Nnfsrc_Var *v ;
  int i ;
  char *s ;
  GtkTextBuffer *text_buffer ;
  GtkTextIter start_iter, end_iter ;

  m = (int)gtk_object_get_data(GTK_OBJECT(w), "nnfs2") - 1 ;
  if ( m < 0 )
    return ;

  v = g_Nnfsrc_Var_Table() ;

  switch( v[m].type )
    {
    case g_Var_Big_String:
      g_Free(v[m].content) ;
      text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w)) ;
      gtk_text_buffer_get_start_iter(text_buffer, &start_iter);
      gtk_text_buffer_get_end_iter(text_buffer, &end_iter);
      s = gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(w)),
				   &start_iter, &end_iter, TRUE) ;
      v[m].content = g_Save_String(s) ;
      g_free(s) ;
      break ;
    case g_Var_String:
    case g_Var_Integer:
      g_Free(v[m].content) ;
      s = gtk_editable_get_chars(GTK_EDITABLE(w), 0, -1) ;
      v[m].content = g_Save_String(s) ;
      g_free(s) ;
      break ;
    case g_Var_Choice:
    case g_Var_Boolean:
      g_Free(v[m].content) ;
      gtk_label_get(GTK_LABEL(GTK_BIN(w)->child), &v[m].content) ;
      for(i=0; v[m].choices[i][0][0]; i++)
	if ( strcmp(v[m].choices[i][1], v[m].content) == 0 )
	  {
	    v[m].content = g_Save_String(v[m].choices[i][0]) ;
	    break ;
	  }
      if ( v[m].choices[i][0][0] == '\0' )
	{
	  abort() ;
	}
      break ;
    case g_Var_Filter:
      {
	char *t[100][8] ;
	int j, n ;
	char *pc ;

	memset(t, 0, sizeof(t)) ;
	gtk_container_foreach (GTK_CONTAINER (w),
			       (GtkCallback) retrieve_filter_values,
			       (gpointer)t);
	n = 0 ;
	for(i=0;i<100;i++)
	  for(j=0;j<8;j++)
	    if ( t[i][j] )
	      {
		n += strlen(t[i][j]) + 10 ;
	      }
	G_REALLOC(v[m].content, n) ;
	pc = v[m].content ;
	
	for(i=0;t[i][0];i++)
	  {
	    if ( t[i][1][0]||t[i][2][0]||t[i][3][0]||t[i][4][0]||t[i][5][0]
		 || t[i][7][0] )
	      {
		*pc++ = t[i][0][0] ;
		if ( t[i][1][0] )
		  pc += sprintf(pc, " perm=%s", t[i][1]) ;
		if ( t[i][2][0] )
		  pc += sprintf(pc, " perm=+%s", t[i][2]) ;
		if ( t[i][3][0] )
		  pc += sprintf(pc, " perm=-%s", t[i][3]) ;
		if ( t[i][4][0] )
		  pc += sprintf(pc, " size=-%s", t[i][4]) ;
		if ( t[i][5][0] )
		  pc += sprintf(pc, " size=+%s", t[i][5]) ;
		switch( g_String_To_Type(t[i][6]) )
		  {
		  case g_Is_A_Directory:
		    pc += sprintf(pc, " type=d") ;
		    break ;
		  case g_Is_A_Link:
		    pc += sprintf(pc, " type=l") ;
		    break ;
		  case g_Is_A_FIFO:
		    pc += sprintf(pc, " type=p") ;
		    break ;
		  case g_Is_A_File:
		    pc += sprintf(pc, " type=f") ;
		    break ;
		  case g_Is_Any:
		    break ;
		  default:
		    abort() ;
		  }
		if ( t[i][7][0] )
		  pc += sprintf(pc, " regex=%s", t[i][7]) ;
		pc += sprintf(pc, "\n") ;
	      }
	    for(j=0;j<8;j++)
	      if ( t[i][j] )
		g_Free(t[i][j]) ;
	  }
      }
      break ;
    }
}

void save_config()
{
  FILE *g ;
  g_Nnfsrc_Var *v ;
  char **cases ;
  int nb_cases ;
  g_Boolean found ;
  GtkWidget **tables ;
  int k ;
  char *tmp ;
  char *new_file_name ;
  char *old_file_name ;

  v = g_Nnfsrc_Var_Table() ;

  G_MALLOC(new_file_name, strlen(gconfig->nnfsrc)+6) ;
  sprintf(new_file_name, "%s.new~", gconfig->nnfsrc) ;
  g = fopen(new_file_name, "w+") ;
  if ( g == NULL )
    {
      g_Printf("Can't write new configuration") ;
      perror(new_file_name) ;
      return ;
    }
  tables = pages[search_page("Setup")].tables ;

  for(k=0;tables[k];k++) /* 10 = More than the number of pages */
    {
      gtk_container_foreach (GTK_CONTAINER (tables[k]),
			     (GtkCallback) retrieve_values, NULL);
    }
  tmp = g_Case_Name(goptions->hostname) ;
  g_Nnfsrc_Action( gconfig->nnfsrc,v, tmp,&found, &cases,&nb_cases,g,
		   goptions->hostname) ;
  fclose(g) ;

  G_MALLOC(old_file_name, strlen(gconfig->nnfsrc)+6) ;
  sprintf(old_file_name, "%s~", gconfig->nnfsrc) ;
  if ( rename(gconfig->nnfsrc, old_file_name) )
    {
      g_Printf("Error renaming %s to %s\n", gconfig->nnfsrc, old_file_name) ;
      perror(old_file_name) ;
    }

  if ( rename(new_file_name, gconfig->nnfsrc) )
    {
      g_Printf("Error renaming %s to %s\n", new_file_name, gconfig->nnfsrc) ;
      perror(new_file_name) ;
    }

  gtk_main_quit ();
  /*

  set_config_widget(tmp) ;
  g_Free(tmp) ;
  */
}

void set_config_widget(char *named)
{
  char **cases ;
  int nb_cases ;
  int k, m, i ;
  GtkWidget **tables, *name, *entry, *menu, *item ;
  g_Nnfsrc_Var *v ;
  g_Boolean found ;

  v = g_Nnfsrc_Var_Table() ;
  g_Nnfsrc_Action( gconfig->nnfsrc,v, named,&found, &cases, &nb_cases, NULL,
		   goptions->hostname) ;

  tables = pages[search_page("Setup")].tables ;

  for(k=0;tables[k];k++) /* 10 = More than the number of pages */
    {
      gtk_widget_hide(tables[k]) ;

      gtk_container_foreach (GTK_CONTAINER (tables[k]),
			     (GtkCallback) gtk_widget_destroy, NULL);

      for(m=0; v[m].name ; m++)
	{
	  if ( v[m].page != k )
	    continue ;

	  if ( v[m].type != g_Var_Filter )
	    {
	      name = gtk_label_new( v[m].name ) ;
	      gtk_widget_show(name) ;
	      gtk_table_attach( GTK_TABLE(tables[k]), name,
				0,1, m,m+1, VO,VO,0,0) ; 
	    }

	  switch( v[m].type )
	    {
	    case g_Var_Big_String:
	      entry = gtk_text_view_new() ;
	      gtk_signal_connect (GTK_OBJECT (entry), "realize",
				  GTK_SIGNAL_FUNC (realize_text),
				  v[m].content);
	      gtk_text_view_set_editable(GTK_TEXT_VIEW(entry), TRUE) ;
	      break ;
	    case g_Var_Filter:
	      entry = output_filter(v[m].content) ;
	      break ;
	    case g_Var_Choice:
	    case g_Var_Boolean:
	      entry = gtk_option_menu_new() ;
	      v[m].menu = create_texts(v[m].choices) ;

	      gtk_option_menu_set_menu(GTK_OPTION_MENU(entry), v[m].menu);


	      for(i=0;v[m].choices[i][0][0];i++)
		if ( strcmp(v[m].choices[i][0], v[m].content) == 0 )
		  break ;
	      if ( v[m].choices[i][0][0]=='\0' ) i = 0 ;
	      gtk_option_menu_set_history(GTK_OPTION_MENU(entry), i) ;
	      break ;
	    default:
	      entry = gtk_entry_new() ;
	      if ( v[m].widget_name )
		gtk_widget_set_name(entry, v[m].widget_name) ;
	      gtk_entry_set_text (GTK_ENTRY (entry), v[m].content) ;
	      break ;
	    }

	  gtk_widget_show(entry) ;

	  gtk_tooltips_set_tip(tips, entry, v[m].tip, NULL) ;

	  gtk_object_set_data(GTK_OBJECT(entry), "nnfs2", (gpointer)(m+1)) ;
	  gtk_table_attach( GTK_TABLE(tables[k]), entry, 1,2, m,m+1,
			    (GtkAttachOptions)(GTK_FILL|GTK_EXPAND),VO,0,0) ; 
	}

      gtk_widget_show(tables[k]) ;
    }

  if ( pages[search_page("Setup")].menu )
    gtk_widget_destroy( pages[search_page("Setup")].menu ) ;

  menu = gtk_menu_new();
  pages[search_page("Setup")].menu = menu ;
  for(k=0;k<nb_cases;k++)
    {
      item = gtk_menu_item_new_with_label(cases[k]);
      gtk_menu_append(GTK_MENU (menu), item);
      gtk_signal_connect_object(GTK_OBJECT(item), "activate",
				GTK_SIGNAL_FUNC(load_config),
				(gpointer) cases[k]);
      gtk_widget_show(item);
    }

  if ( pages[search_page("Setup")].signal )
    {
      gtk_signal_disconnect(GTK_OBJECT(pages[search_page("Setup")].button),
			    pages[search_page("Setup")].signal
			    ) ;
    }

  pages[search_page("Setup")].signal =
    gtk_signal_connect_object(GTK_OBJECT(pages[search_page("Setup")].button),
			      "event",
			      GTK_SIGNAL_FUNC (button_press),
			      GTK_OBJECT(menu));

}


/*
 *
 */
GtkWidget *setup_page(int i)
{
  GtkWidget *vbox, *noteb, *frame ;
  GtkWidget *hbox, *b2, *eb ;
  int k ;
  static char *t[][2] =
  {
    {"Medium Access","How to read/write/eject the medium (host dependent)"},
    {"Basic options", "Simple options (host independent)"},
    {"Advanced options", "Advanced options (host independent)"},
    {"Output Filter", "The output filter allow to choose files no to be wrote on medium (host independent)"}
  } ;
  

  vbox = gtk_vbox_new(0,0) ;

  pages[i].label = gtk_label_new("NNFS2 Configuration pages") ;
  gtk_widget_show(pages[i].label) ;
  gtk_box_pack_start(GTK_BOX(vbox), pages[i].label, 0,0,0) ;

  noteb = gtk_notebook_new() ;
  gtk_container_border_width (GTK_CONTAINER (noteb), 5);
  gtk_box_pack_start(GTK_BOX(vbox), noteb, 0,0,0) ;

  for(k=0;k<sizeof(t)/sizeof(t[0]);k++)
    {
      pages[i].tables[k] = gtk_table_new(2, 20, 0) ;

      frame = gtk_frame_new (t[k][1]);
      gtk_container_border_width (GTK_CONTAINER (frame), 5);
      gtk_container_add (GTK_CONTAINER (frame), pages[i].tables[k]);
      gtk_widget_show(frame) ;

      eb = notebook_label(t[k][0], k, t[k][1]) ;

      gtk_notebook_append_page (GTK_NOTEBOOK (noteb), frame, eb);
    }
  gtk_widget_show(noteb) ;


  hbox = gtk_hbox_new(0,0) ;

  pages[i].button = gtk_button_new_with_label("Read a configuration");
  gtk_tooltips_set_tip(tips, pages[i].button,
		       "Use this button to load an example of configuration for various systems. The configuration is used only once the configuration saved.", NULL) ;
  gtk_widget_show(pages[i].button) ;
  gtk_box_pack_start(GTK_BOX(hbox), pages[i].button, TRUE,0,0) ;

  b2 = gtk_button_new_with_label ("Save this configuration and exit");
  gtk_signal_connect (GTK_OBJECT (b2), "clicked",
		      GTK_SIGNAL_FUNC (save_config), NULL);
  gtk_tooltips_set_tip(tips, b2,
		       "Use this button to save the configuration for this host, this will not affect the others. The edited configuration is used only once saved.", NULL) ;
  gtk_widget_show(b2) ;
  gtk_box_pack_start(GTK_BOX(hbox), b2, TRUE,0,0) ;

  gtk_widget_show(hbox) ;
  gtk_box_pack_start(GTK_BOX(vbox), hbox, 0,0,0) ;

  set_config_widget(gconfig->switchcase) ;

  return(vbox) ;
}

/*
 *
 */

GtkWidget *byebye_page(int i)
{
  GtkWidget *label ;

  label = gtk_label_new ("Program is running or asking you a question") ;
  return(label) ;
}

/*
 *
 */

void continue_update()
{
  char c ;

  c = G_CONTINUE ;
  write(ganswer,&c,1) ;
  unsensitivize_all() ;
  if ( can_not_write )
    {
      gtk_object_destroy(GTK_OBJECT(can_not_write)) ;
      can_not_write = NULL ;
    }
}

gint row_compare(GtkTreeModel *model,
		 GtkTreeIter *a,
		 GtkTreeIter *b,
		 gpointer user_data)
{
  GValue value_a = {0}, value_b = {0} ;

  gtk_tree_model_get_value(model, a, (int)user_data /* column */, &value_a) ;
  gtk_tree_model_get_value(model, b, (int)user_data /* column */, &value_b) ;

  return strcmp(value_a.data[0].v_pointer, value_b.data[0].v_pointer) ;
}

void column_clicked(GtkTreeViewColumn *tvc, gpointer data)
{
  static GtkTreeViewColumn *sorted_column = NULL ;
  static int direction = 0 ;
  static int dir[] = { GTK_SORT_ASCENDING,  GTK_SORT_DESCENDING } ;

  if ( sorted_column )
    {
      if ( sorted_column == tvc )
	{
	  direction = (direction + 1) % 2 ;
	}
      else
	{
	  direction = 0 ;
	  gtk_tree_view_column_set_sort_indicator(sorted_column, FALSE) ;
	}
    }

  sorted_column = tvc ;

  gtk_tree_sortable_set_sort_func
    ( GTK_TREE_SORTABLE(pages[search_page("Question")].list),
      (int)data,
      row_compare,
      data,
      NULL /* gtkDestroyNotify */
      );


  gtk_tree_view_column_set_sort_order(tvc, dir[direction]) ;


  gtk_tree_sortable_set_sort_column_id
    (GTK_TREE_SORTABLE(pages[search_page("Question")].list),
     (int)data,
     dir[direction]
     ) ;

  gtk_tree_view_column_set_sort_indicator(tvc, TRUE) ;

}


GtkWidget *question_page(int i)
{
  GtkWidget *list, *title, *scrolled_window ;
  GtkTreeViewColumn *tvc ;
  int c ;
  char *name[] = { "?", "Mode", "Size", "Modification date", "Filename" } ;

  pages[i].list = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING) ;
  list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(pages[i].list)) ;

  gtk_widget_show(list) ;

  gtk_tooltips_set_tip(tips, GTK_WIDGET(list),
		       "The first column indicates the action to do:\n"
		       "R: Read from medium,\n"
		       "W: Write to medium,\n"
		       "D: Delete,\n"
		       "F: File will not be written on medium,\n"
		       "f: File can not be read from medium,\n"
		       "C: There is an update conflict : Remote win,\n"
		       "\n"
		       "D in the file mode indicates a deleted file.\n"
		       "The other informations are about the up to date\n"
		       "information on the file."
		       , NULL) ;

  
  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_border_width (GTK_CONTAINER (scrolled_window), 5);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_container_add (GTK_CONTAINER (scrolled_window), list);
  gtk_widget_show(scrolled_window) ;
  
  gtk_widget_set_usize(list, -1, 100) ;
  

  gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(list), TRUE) ;

  for(c=0; c<5; c++)
    {
      tvc = gtk_tree_view_column_new_with_attributes(name[c],gtk_cell_renderer_text_new(), "text", c, NULL) ;

      title = gtk_label_new(name[c]) ;      
      gtk_widget_show(title) ;

      gtk_tree_view_column_set_widget(tvc, title) ;

      gtk_tree_view_column_set_clickable(tvc, TRUE) ;


      gtk_signal_connect (GTK_OBJECT (tvc), "clicked",
		      GTK_SIGNAL_FUNC (column_clicked), (gpointer)c);

      gtk_tree_view_append_column(GTK_TREE_VIEW(list),  tvc) ;


      if ( c == 0 )
	column_clicked(tvc, (gpointer)c) ;

    }


  return scrolled_window ;
}

/*
 *
 */

void delete_medium_page()
{
  gtk_widget_set_sensitive(skip_reading, FALSE) ;
  gtk_widget_set_sensitive(ok_medium, FALSE) ;
}

void answer_function1()
{
  char c ;
  c = G_CONTINUE ;
  write(ganswer, &c, 1) ;
  delete_medium_page() ;
}
void answer_function0()
{
  char c ;
  c = G_ABORT_READING ;
  write(ganswer, &c, 1) ;
  delete_medium_page() ;
}


void medium_function(gpointer data, gint source, GdkInputCondition condition)
{
  char buf[G_LINE_CHUNK] ;
  char tmp[G_LINE_CHUNK] ;
  int nb ;
  static char last_name[G_LINE_CHUNK] = "" ;
  GList *gl ;

  nb = read(source,buf,G_LINE_CHUNK) ;

  if ( nb<=0 )
    {
      remove_end(&id_medium) ;
      return ;
    }
  if ( nb!=4 )
    {
      fprintf(stderr, "Impossible case : nb=%d\n", nb) ;
      exit(46) ;
    }
  buf[4] = '\0' ;
  if ( strcmp(last_name, ((char*)buf)+1) )
    {
      sprintf(tmp, "Medium %s is ready for %s",
	      buf+1, buf[0]=='R' ? "reading" : "writing") ;
    }
  else
    {
      if ( buf[0]=='R' )
	{
	  sprintf(tmp,
		  "ERROR! Reinsert medium %s",
		  buf+1) ;
	}
      else
	{
	  sprintf(tmp,
		  "ERROR! Reinsert medium %s",
		  buf+1) ;
	}
    }
  gl = gtk_container_children(GTK_CONTAINER(ok_medium)) ;
  gtk_label_set(GTK_LABEL((GtkWidget*)gl->data), tmp) ;
  g_list_free(gl) ;

  gtk_widget_set_sensitive(ok_medium, TRUE) ;

  if ( buf[0] == 'R' )
    gtk_widget_set_sensitive(skip_reading, TRUE) ;

  strcpy(last_name, ((char*)buf)+1) ;
}




/*
 ******************************************************************************
 */


struct pages pages[] =
{
  { COPYRIGHT, "NNFS " NNFS_VERSION " Copyright",
    "Copyright page",
    welcome_page},
  { "Setup", "NNFS2 setup",
    "NNFS configuration and preferences",
    setup_page},
  { "Hosts", "Host Informations",
    "List of all the hosts participating into the file system",
    host_page},
  { "Preprocessing", "Preprocessing: Reading medium and local informations",
    "Work to be done before the start of the real update. There no file modification in this stage.",
    preprocessing_page},
  { "Question", "Do you want to continue the update ?",
    "What must be done for update, if you agree update can start",
    question_page },
  { "Postprocessing", "Postprocessing: Updating local and medium files",
    "Work done for the real updating",
    preprocessing_page},
  { "Bye Bye", "Good bye, the program had terminated",
    "The update is terminated, the final message",
    byebye_page}
} ;

/*
 * Search Page
 */

int search_page(char *name)
{
  int i ;

  for(i=0;i<sizeof(pages)/sizeof(pages[0]);i++)
    if ( strcmp(name, pages[i].name)==0 ) return( i ) ;
  g_Printf("Can't find page %s\n", name) ;
  exit(47) ;
  return(0) ;
}

  
/*
 *
 */


void delete (GtkWidget *widget, GdkEvent *event, gpointer *data)
{
  /* It was a bad idea because the shell script using "nnfs" is killed */
  /* g_Kill(0, g_Signal_Interrupt) ; */
  /* So we kill process by process */
  kill(gfils, g_Signal_Interrupt) ;
#ifdef exit
  exit(0) ;
#endif

  gtk_main_quit ();
}




char *unsign(g_Nb ui)
{
  static char t[G_LINE_CHUNK] ;
  sprintf(t, "%lu", ui) ;
  return(t) ;
}


void add_list(GtkListStore *l, char *name, char *c, g_File_Info *f, int nbh)
{
  char siz[30] ;		/* Could store a long long in decimal XXX */
  GtkTreeIter iter ;
  char date[4+1+2+1+2+1+2+1+2+1+2+1], *name_utf8 ;
  gsize bytes_read, bytes_written ;
  struct tm *tm ;
  int utd ;

  if ( f->nnfs_to_local )
    utd = f->up_to_date_on_medium ;
  else
    utd = nbh ;

  name_utf8 = g_locale_to_utf8(name, -1, &bytes_read, &bytes_written, NULL) ;
  if ( name_utf8 == NULL )
    name_utf8 = strdup("Character translation failed, Check LC_ALL, LANG, ...") ;
  /* %10ld in order to have a right justify and a working numerical sort. */
  sprintf(siz, "%10ld", f->hosts[utd].size) ;
  tm = localtime(&f->hosts[utd].date) ;
  sprintf(date, "%04d-%02d-%02d %02d:%02d:%02d",
	  1900 + tm->tm_year,
	  1 + tm->tm_mon,
	  tm->tm_mday,
	  tm->tm_hour,
	  tm->tm_min,
	  tm->tm_sec) ;
  /* date = ctime(&f->hosts[utd].date) ; */
  /* date[strlen(date)-1] = '\0' ; */
  gtk_list_store_append(l, &iter) ;
  gtk_list_store_set(l, &iter
		     , 0, c
		     , 1, g_file_info_mode(f,utd)
		     , 2, siz
		     , 3, date
		     , 4, name_utf8
		     , -1) ;
  if( name_utf8 )
    g_free(name_utf8) ;

}


void set_directory(GtkWidget *widget)
{
  const char *h ;
  char c ;

  c = G_ADD_HOST ;
  write(ganswer, &c, 1) ;
  h = gtk_entry_get_text( GTK_ENTRY(widget) ) ;
  write(ganswer,h, strlen(h)+1) ;
  unsensitivize_all() ;
  empty_table() ;
}



void g_Analyse2(char buf[G_LINE_CHUNK-3])
{
  static char aliases[G_LINE_CHUNK] = "" ;
  static char a_directory[G_LINE_CHUNK] ;
  static char an_host[G_LINE_CHUNK] ;
  static time_t mount_time ;
  static char nb_mount[G_LINE_CHUNK] ;
  static char *current_host ;
  static char display_bar_name[G_LINE_CHUNK] ;
  static int display_bar_max ;
  static int nbh=2, yet=0 ;
  static int line = 0 ;
  static enum state state = Reading ;
  static char *lt[] = {
    "local files",
    "files on hosts",
    "files from medium",
    "files to medium",
    "remote file change",
    "local file change",
    "update conflict",
    "file to delete"} ;

  static GtkWidget *pb, *box, *label1, *label, *table, *hs ;
  GtkListStore *list ;
  unsigned long ul ;
  int i, j ;
  g_Nb s[9] ;
  char command[G_LINE_CHUNK] ;
  int nb, nbe ;
  g_File_Info *f ;
  char *name ;
  int to_delete ;
  char action[20], *ptr_action ;

  switch(*buf++)
    {
    case G_HOST:
      strcpy(an_host,buf) ;
      break ;
    case G_NB_MOUNT:
      sprintf(nb_mount, "%d", atoi(buf)) ;
      break ;
    case G_DIRECTORY:
      strcpy(a_directory,buf) ;
      break ;
    case G_LAST_MOUNT:
      sscanf(buf, "%lu", &ul) ;
      mount_time = (time_t)ul ;
      break ;
    case G_ALIAS:
      strcat(aliases, "Alias ") ;
      strcat(aliases, buf) ;
      strcat(aliases, "\n") ;
      break ;
    case G_HOST_DONE:
      table = HOST.table ;

      if ( nb_hosts == 0 )
	{
	  fill_table(table, 0, "Host.Domain.User", "Directory", "#Mounts", "Last update time", current_host) ;
	  
	  hs = gtk_hseparator_new() ;
	  gtk_widget_show(hs) ;
	  gtk_table_attach (GTK_TABLE (table), hs, 0,5,1,2,GTK_FILL,VO,5,0);
	}  

      name = ctime(&mount_time) ;
      name[strlen(name)-1] = '\0' ;
      fill_table(table, nbh++, an_host, a_directory, nb_mount, name, current_host) ;
      
      if ( aliases[0] )
	{
	  fill_td(table, nbh++,1,aliases, 1,3) ;
	  aliases[0] = '\0' ;
	}
      nb_hosts++ ;
      goto_page("Hosts") ;
      break ;
    case G_CURRENT_HOST:
      table = HOST.table ;

      label = gtk_hseparator_new() ;
      gtk_widget_show(label) ;
      gtk_table_attach (GTK_TABLE (table), label,
			0,5,NH*2,NH*2+1,GTK_FILL,VO,5,0);

      sprintf(command,"Current host is %s", buf) ;
      fill_td(table, 2*NH+1, 0, command, 1, 4) ;
      current_host = strdup(buf) ;
      break ;
    case G_SET_DISPLAY_BAR:

      if ( state == Reading && yet==0 )
	{
	  goto_page("Preprocessing") ;
	  table = pages[search_page("Preprocessing")].content ;
	  yet = 1 ;
	  line = 0 ;
	}
      if ( state == Writing && yet==1 )
	{
	  goto_page("Postprocessing") ;
	  table = pages[search_page("Postprocessing")].content ;
	  yet = 0 ;
	  line = 0 ;
	  gtk_widget_set_name(wquit, "dangerous") ;
	}
      sscanf(buf, "%d", &display_bar_max) ;
      for(j=0;buf[j]!=' ';j++) /* empty */ ;
      strcpy(display_bar_name, &buf[j+1]) ;

      pb = gtk_progress_bar_new () ;
      gtk_widget_show(pb) ;
      gtk_table_attach (GTK_TABLE (table), pb, 0,1,line,line+1,GTK_FILL,VO,5,2);

      label1 = gtk_label_new(display_bar_name) ;
      box = gtk_alignment_new(0,0.5,0.,0.) ;
      gtk_container_add (GTK_CONTAINER (box), label1);
      gtk_widget_show(label1) ;
      gtk_widget_show(box) ;
      gtk_table_attach (GTK_TABLE (table), box, 3,4,line,line+1,GTK_FILL,VO,5,0);

      sprintf(command,"/%d",  display_bar_max) ;
      label = gtk_label_new(command) ;
      box = gtk_alignment_new(0,0.5,0.,0.) ;
      gtk_container_add (GTK_CONTAINER (box), label);
      gtk_widget_show(label) ;
      gtk_widget_show(box) ;
      gtk_table_attach (GTK_TABLE (table), box, 2,3,line,line+1,GTK_FILL,VO,0,0);

      label = gtk_label_new("0") ;
      box = gtk_alignment_new(1.,0.5,0.,0.) ;
      gtk_container_add (GTK_CONTAINER (box), label);
      gtk_widget_show(label) ;
      gtk_widget_show(box) ;
      gtk_table_attach (GTK_TABLE (table), box, 1,2,line,line+1,GTK_FILL,VO,0,0);
      line++ ;


      /* fall thru */
    case G_BAR:
      if ( buf[-1]==G_BAR )
	{
	  sscanf(buf, "%d%d", &nb, &nbe) ;
	  sprintf(command,"%d", nb) ;
	  gtk_label_set(GTK_LABEL(label),command) ;
	}
      else
	nb = nbe = 0 ;
      if ( display_bar_max )
	gtk_progress_bar_update (GTK_PROGRESS_BAR(pb), nb*1./display_bar_max) ;
      break ;
    case G_FINISH_BAR:
      sscanf(buf, "%d", &nbe) ;
      if ( nbe )
	{
	  global_there_is_error = g_True ;
	  gtk_widget_set_name(label1, "dangerous") ;
	  gtk_widget_set_name(label, "dangerous") ;
	  gtk_widget_set_name(pb, "dangerous") ;
	}
      gtk_progress_bar_update (GTK_PROGRESS_BAR(pb), 1.) ;
      sprintf(command,"%d", display_bar_max) ;
      gtk_label_set(GTK_LABEL(label),command) ; 
      break ;
    case G_FLUSHING_DATA:
      label = gtk_label_new("Flushing data on medium") ;
      gtk_widget_show(label) ;
      table = pages[search_page("Postprocessing")].content ;
      gtk_table_attach(GTK_TABLE (table), label, 0,4,line+1,line+2,GTK_FILL,VO,5,0);
      line++ ;

      break ;
    case G_CAN_NOT_WRITE:
      /*
       * Add the message in the window (postprocessing)
       */
      name = g_Formatted_Strings("%\nDo what is needed and continue the update", buf) ;
      label = gtk_label_new(name) ;
      can_not_write = label ;
      g_Free(name) ;
      gtk_widget_show(label) ;
      table = pages[search_page("Postprocessing")].content ;
      gtk_table_attach(GTK_TABLE (table), label, 0,4,line+1,line+2,GTK_FILL,VO,5,0);
      /*
       * Change button state
       */
      gtk_widget_set_sensitive(continue_button, TRUE) ;

      break ;
    case G_STATS:
      sscanf(buf, "%lu%lu%lu%lu%lu%lu%lu%lu%lu",
	     s,s+1,s+2,s+3,s+4,s+5,s+6,s+7,s+8) ;

      table = gtk_table_new(4,6,0) ;

      name = command ;
      strcpy(name,
	     "Use this button to continue the update. "
	     "Until you click on this button, nothing is done on your files. "
	     "\n\n"
	     ) ;
      name += strlen(name) ;
      for(j=0;j<8;j++)
	{
	  sprintf(name, "%ld %s\n", s[j], lt[j]) ;
	  name += strlen(name) ;
	}
      sprintf(name, "%lu bytes to compress&write", s[8]) ;

      gtk_tooltips_set_tip(tips, continue_button, command, NULL) ;


      /*      gtk_tree_sortable_sort_column_changed(GTK_TREE_SORTABLE(pages[search_page("Question")].list)) ; */
      

      

      goto_page("Question") ;
      state = Writing ;
      break ;
    case G_FILE:

      
      list = pages[search_page("Question")].list ;
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

      ptr_action = action ;
      if ( f->conflict )
	*ptr_action++ = 'C' ;
      if ( (f->to_delete && !to_delete) && f->nnfs_to_local )
	*ptr_action++ = 'D' ;
      if ( f->hosts[f->up_to_date].type != g_Is_A_Directory )
	{
	  if ( f->nnfs_to_local && !f->to_delete )
	    *ptr_action++ = 'R' ;
	  if ( f->local_to_nnfs && !f->filtered )
	    *ptr_action++ = 'W' ;
	}
      if ( ptr_action != action )
	{
	  if ( f->filtered )
	    *ptr_action++ = 'f' ;
	  if ( f->nnfs_filtered )
	    *ptr_action++ = 'F' ;

	  *ptr_action = '\0' ;

	  add_list(list, name, action , f, nb_hosts) ;
	}
      break ;

    case G_QUESTION:
      goto_page("Question") ;
      gtk_widget_set_sensitive(continue_button, TRUE) ;

	break ;

    case G_NNFSRC_CREATED:
      sprintf(command,
	      "Configuration file: "
	      "``%s'' "
	      "is created with default values.\n"
	      "So it is the first time you use NNFS2 on this connection directory.\n"
	      "Don't forget to configure NNFS2 (`setup' page) and format new medium\n"
	      "if it is the first host you add in NNFS file system."
	      ,buf) ;

    case G_NNFSRC_NOT_CREATED:
      if ( buf[-1] == G_NNFSRC_NOT_CREATED )
	{
	  strcpy(command,
		 "Your normal choice is to continue the update.\n"
		 "Nevertheless you can reconfigure NNFS2 or\n"
		 "reinitialise medium."
		 ) ;
	}
      gtk_label_set( GTK_LABEL(HOST.label), command) ;
      goto_page("Hosts") ;
      break ;



    case G_HOST_NOT_IN_NNFS:

      gtk_label_set( GTK_LABEL(HOST.label),
		     "\n"
		     "This host doesn't participate to NNFS\n"
		    ) ;

      if ( nb_hosts )
	{
	  table = HOST.table ;
	  for(i=0;i<NH;i++)
	    if ( add_nickname_but[i] )
	      {
		gtk_widget_show(add_nickname_but[i]) ;
	      }

	  format_button = frame_label_button
	    ("Current host is the nickname of a previous host"
	     ,
	     "If the current host has access to exactly the same files (via NFS, AFS, ...)\n"
	     "than an host in the previous table, you can indicate this by clicking\n"
	     "on the button +Nickname of the corresponding host"
	     ,
	     "",
	     NULL, 0, "") ;
	  gtk_box_pack_start(GTK_BOX(HOST.content),
			     format_button, 0,0,0) ;
	}
      else
	format_button = NULL ;

      configure_button = frame_label_button
	("Adding a new host in NNFS filesystem"
	 ,
	 "To add the current host in NNFS filesystem, you only need to indicate\n"
	 "the local directory name you want to synchronize and hit return."
	 ,
	 getenv("HOME")
	 ,
	 set_directory, 1, "") ;
      gtk_box_pack_start(GTK_BOX(HOST.content),
			 configure_button,0,0,0);
      break ;
      
    default:
      exit(48) ;
    }
}


void info_function(gpointer data, gint source, GdkInputCondition condition)
{
  unsigned char buf[G_LINE_CHUNK] ;
  int nb, nbt, i ;

  nb = 0 ;
  nbt = 0 ; /* To avoid a compiler warning */
  i = -1 ; /* To avoid a compiler warning */
  do
    {
      if ( nb>=3 ) nbt = buf[1]*256+buf[2] ;
      while( nb<3 || nb<nbt )
	{
	  i = read(source, &buf[nb], G_LINE_CHUNK-nb) ;
	  if ( i<=0 )
	    break ;
	  if ( buf[0]!='\0' )
	    {
	      g_Printf("(((%s)))",buf) ;
	      exit(49) ;
	    }
	  nb += i ;
	  if ( nb>=3 ) nbt = buf[1]*256+buf[2] ;
	}
      if ( i<=0 )
	break ;
      g_Analyse2((char*)&buf[3]) ;
      memmove(buf,&buf[nbt],nb-nbt) ;
      nb -= nbt ;
    }
  while( nb != 0 ) ;

  if ( i<=0 )
    {
      remove_end(&id_main) ;
    }
}


void g_Ui_GTK(int mainp, int medium, int error, int answer, pid_t fils,int *argc, char***argv, g_Config *c, g_Options *opt)
{
  GtkWidget *vbox, *hbox, *vbox2 ;
  int i ;
  char *gtkrc ;
  FILE *f ;
  char buf[G_LINE_CHUNK] ;
  g_Boolean has_treeview ;

  ganswer = answer ;
  gfils = fils ;
  gerror = error ;
  gconfig = c ;
  goptions = opt ;

  setlocale(LC_CTYPE, "" ) ;

  gtk_init (argc, argv);
  /*
   * GTKRC
   */
  gtkrc =  g_Formatted_Strings( "%" G_STRING_SEPARATOR "gtkrc", c->nnfs) ;
  f = fopen(gtkrc, "r") ;
  if ( f == NULL )
    {
      f = fopen(gtkrc, "w") ;
      if ( f == NULL )
	{
	  perror(gtkrc) ;
	  exit(50) ;
	}
#include "gtkrc"
      fclose(f) ;
    }
  else
    {
      /*
       * Add a new option in gtkrc if it isn't here
       */
      has_treeview = g_False ;
      for(;;)
	{
	  if ( fgets(buf, sizeof(buf), f) == NULL )
	    break ;
	  if ( strstr(buf, "GtkTreeView") )
	    {
	      has_treeview = g_True ;
	      break ;
	    }
	}
      fclose(f) ;

      
      if ( !has_treeview )
	{
	  f = fopen(gtkrc, "a") ;
	  if ( f )
	    {
	      fprintf(f,
		      "\n"
		      "style \"table\"\n"
		      "{\n"
		      "font_name = \"Courier 8\"\n"
		      "}\n"
		      "widget \"*GtkTreeView*\" style \"table\"\n"
		      ) ;
	      fclose(f) ;
	    }
	}
    }

  gtk_rc_parse(gtkrc);
  g_Free(gtkrc) ;


  tips = gtk_tooltips_new() ;

  if ( gconfig->auto_start )
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  else
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);


  gtk_signal_connect (GTK_OBJECT (window), "delete_event",
		      GTK_SIGNAL_FUNC (delete), NULL);


  vbox = gtk_vpaned_new() ;
  gtk_container_border_width (GTK_CONTAINER (vbox), 5);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  notebook = gtk_notebook_new ();
  gtk_paned_pack1 (GTK_PANED (vbox), notebook, TRUE, TRUE);

  for(i=0;i<sizeof(pages)/sizeof(pages[0]);i++)
    {
      pages[i].content = (*pages[i].create)(i) ;
      gtk_widget_show(pages[i].content) ;

      pages[i].frame = gtk_frame_new (pages[i].full_name);
      gtk_container_border_width (GTK_CONTAINER (pages[i].frame), 5);
      gtk_container_add (GTK_CONTAINER (pages[i].frame), pages[i].content);
      gtk_widget_show(pages[i].frame) ;

      pages[i].eb = notebook_label(pages[i].name, i, pages[i].tip) ;

      gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
				pages[i].frame,
				pages[i].eb
				) ;
    }

  vbox2 = gtk_vbox_new(0,0) ;
  gtk_paned_pack2 (GTK_PANED (vbox), vbox2,FALSE,TRUE);
  gtk_box_pack_start (GTK_BOX (vbox2), error_page(),TRUE,TRUE,0) ;

  /*
   * Buttons
   */

  hbox = gtk_hbox_new(FALSE,0) ;


  continue_button = gtk_button_new_with_label("Continue the update") ;
  gtk_widget_set_name(continue_button, "normal") ;
  gtk_signal_connect (GTK_OBJECT (continue_button), "clicked",
		      GTK_SIGNAL_FUNC (continue_update), NULL);
  gtk_widget_show(continue_button) ;
  gtk_box_pack_start( GTK_BOX(hbox), continue_button,FALSE,FALSE,0) ;


  ok_medium = gtk_button_new_with_label("No medium needed") ;
  gtk_widget_set_sensitive(ok_medium, FALSE) ;
  gtk_widget_set_name(ok_medium, "normal") ;
  gtk_signal_connect (GTK_OBJECT (ok_medium), "clicked",
		      GTK_SIGNAL_FUNC (answer_function1), NULL);
  gtk_widget_show(ok_medium) ;
  gtk_box_pack_start( GTK_BOX(hbox), ok_medium,TRUE,FALSE,0) ;
  gtk_tooltips_set_tip(tips, ok_medium,
		       "Use this button to indicate that the correct "
		       "medium is ready (for reading or writing).",
		       NULL) ;


  skip_reading = gtk_button_new_with_label("Skip medium") ;
  gtk_widget_set_name(skip_reading, "dangerous") ;
  gtk_widget_set_sensitive(skip_reading, FALSE) ;
  gtk_signal_connect (GTK_OBJECT (skip_reading), "clicked",
		      GTK_SIGNAL_FUNC (answer_function0), NULL);
  gtk_widget_show(skip_reading) ;
  gtk_tooltips_set_tip(tips, skip_reading,
		       "Use this button to indicate that you want to skip "
		       "the medium reading because the medium is not "
		       "readable or you have forgotted it.\n\n"
		       "The update will nevertheless continue, "
		       "some files will not be correctly updated. "
		       "Be careful with future update.",
		       NULL) ;
  gtk_box_pack_start( GTK_BOX(hbox), skip_reading,TRUE,FALSE,0) ;


  wquit = gtk_button_new_with_label ("Quit");
  gtk_widget_show(wquit) ;
  gtk_box_pack_start( GTK_BOX(hbox), wquit, FALSE, FALSE,0) ;
  gtk_widget_show(hbox) ;
  gtk_box_pack_start (GTK_BOX (vbox2), hbox,0,0,5) ;
  gtk_signal_connect (GTK_OBJECT (wquit), "clicked",
		      GTK_SIGNAL_FUNC (delete), NULL);
  gtk_tooltips_set_tip(tips, wquit,
		       "Use this button to stop the update "
		       "and close the window.",
		       NULL) ;


  /*
   *
   */

  goto_page(COPYRIGHT) ;
  /*
  gtk_widget_get_arg (page[0].frame, &i, ARG_WIDTH) ;
  gtk_widget_set_arg (page[0].content, &i, ARG_WIDTH) ;
  */


  gtk_widget_show (notebook);
  gtk_widget_show (vbox);
  gtk_widget_show (vbox2);
  gtk_widget_show (window);


  id_main = gdk_input_add (mainp, GDK_INPUT_READ, info_function, NULL) ;
  id_medium = gdk_input_add (medium, GDK_INPUT_READ, medium_function, NULL) ;

  if ( gconfig->auto_start )
    continue_update() ;

  gtk_main() ;
  exit(0) ;
}
