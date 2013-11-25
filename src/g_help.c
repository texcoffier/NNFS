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

#include "g_help.h"
#include "g_host.h"
#include "g_debug.h"
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

/*
 * Problem with UID
 */

void g_Uid_Help(int a, int b)
{
g_Printf("\
NNFS: Are you sure to have inserted your own NNFS medium and not\n\
NNFS: the NNFS medium of another user?\n\
NNFS: Current UID=%d     Medium UID=%d\n\
NNFS:\n\
NNFS: Another case is that your UID had been modified by the administrator\n\
NNFS: If it is the case use '--uid #' option to update your NNFS UID\n\
NNFS:\n",a,b);

exit(13) ;
}

/*
 * Normal help message
 */

void g_Help()
{
g_Printf(
"NNFS:    --add Local_Name Add the current host in NNFS.\n"
"NNFS:                     The parameter is the LOCAL name of the NNFS.\n"
"NNFS:                     If you want an identical home directory on all the\n"
"NNFS:                     host, use $HOME as local name.\n"
"NNFS:    --add-nickname host.domain.user\n"
"NNFS: 	                   Add a nickname (alias) to an existing NNFS host\n"
"NNFS:                     (you have several hosts with floppy drive and NFS)\n"
);
g_Printf(
"NNFS:    --add-first      Format medium (don't read it).\n"
) ;
g_Printf(
"NNFS:    --licence        Print license information.\n"
"NNFS:    --no-x11         Text interface.\n"
"NNFS:------------------------------------------------------------------------\n"
"NNFS: For all the messages we call NNFS (Non-Networked File System)\n"
"NNFS: a virtual hierarchy of files. The ``nnfs'' command try\n"
"NNFS: to update this virtual hierarchy on all the hosts participating to NNFS\n"
"NNFS:\n"
) ;
g_Printf(
"NNFS: The NNFS medium (defined by a script shell) could be whatever you want.\n"
"NNFS: The NNFS medium only contains the files needed to update NNFS\n"
"NNFS: on the differents host. So it could be a small size medium\n"
"NNFS:\n"
"NNFS: For more information: run ``nnfs'' without parameter.\n"
"NNFS:------------------------------------------------------------------------\n"
"NNFS: Last informations: <URL:http://perso.univ-lyon1.fr/thierry.excoffier/nnfs.html>\n"
) ;
}


