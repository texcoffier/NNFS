=====
nnfs2
=====

---------------------------------------------------
Synchronise filesystems between Non-Networked hosts
---------------------------------------------------

.. :Author: Thierry.EXCOFFIER@univ-lyon1.fr
.. :Date:   2013-11-28
.. :Copyright: public domain
.. :Version: 1
.. :Manual section: 1

SYNOPSIS
========

nnfs2 [--no-x11] {--add-first dir-name | --add dir-name | --add-nickname dir-name}

nnfs2 {--help | --licence}

INTRODUCTION
============

The NNFS goal is to provide to *one* user a consistent file system
over all the hosts used by the user.

With hosts connected on a LAN you must use NFS instead of NNFS. In the
case of hosts not connected by a network or connected by a slow network
(modem) NFS is not the answer.

With NNFS, the files are duplicated on all the computers. The up to date
files are on the computer on which the user works. The FS is stored on
the MEDIUM. Usually the MEDIUM is a floppy disk, it could be whatever
you want (easily customisable), if there is much data, several floppies
will be asked.

When the user stops his work on a computer, he runs as last command:
NNFS. Then all his modifications (change, creation, deletion) are
recorded on the MEDIUM.

When the user starts to work on a computer, he runs as first command:
NNFS. Then all modifications (change, creation, deletion) stored on the
MEDIUM are retrieved on current system.

If the user forgot to run an update, NNFS will nevertheless optimally
work. The only drawback is that some files will not be up to date on the
host used by the user.

It is also possible to have a numbered backup of modified/deleted files.
So you cannot lose a file when you run NNFS (because the file is
modified/deleted on another host)

USAGE
=====

NNFS has a text and a graphical user interface. If you run the command
for the first time or with a MEDIUM formatted or not, the command will
provide contextual informations and help and will ask contextual
questions. The graphical user interface allows you to edit the
configuration.

The interactive help is extensive and guide the user. This documentation
is here to give general principles.

By default NNFS works with standards tools: floppy disk and gzip.
The floppy disk access is done via system dependent commands
(IRIX, AIX, HPUX and mcopy in the other cases).
If you want to change these defaults, see the configuration section.

As the default configurations use floppy disk, the NNFS default filter
does not synchronise big files or garbage files. If you want to
change these defaults, see the configuration section.

It is not recommended to continue to modify files while NNFS is running.
It is not dangerous but the result is unpredictable.

Be carefull, some applications wrote their data on disk only when you
stop them. So you must stop these applications before running NNFS. For
example firefox wrote some folder information only when you quit
the program, so you must run NNFS only if firefox is not running.
To avoid this you can create a shell script killing these applications
and cleaning some file before launching NNFS.

To do before using NNFS
-----------------------

If the file hierarchy you want to synchronise is yet identical on all
the hosts you can go to the next section. The root of the file hierarchy
may have a different name on each host. To avoid problems you should use
only relative internal symbolic links, so the links are correct on all
the hosts.

If the files hierarchy are not identical, you have two solutions:

* You create yourself an identical hierarchy on all the hosts.
  The file modification date should be identical on all the hosts.
* Or, you let NNFS create the identical hierarchy on all the hosts. In
  this case, a hierarchy made of the union of all the files is created,
  conflicts are solved using the modification date. It is the only case
  where the modification date is used to solve conflicts.

Initialisations
---------------

When you run the NNFS command on an host not known by the FS it will
explain you how to add the host in the FS.

If the FS does not fit in one medium, you will be asked to introduce
a new medium.

Now, the initialisation for 2 hosts named A and B will be detailed for
the 2 user interfaces.

Initialisations with X11 interface
..................................

First run on host A.

* You click on: Continue without reading the medium
* you enter the local name of the hierarchy to synchronise,
* you click on: Continue the update,
* you click on: Medium AAA is ready for writing,
* you click on: Quit

Second run on host B, assuming medium is ready.

* You click on: Continue the update
* you enter the local name of the hierarchy to synchronise,
* you click on: Continue the update,
* you click on: Quit

Normal run when initialisations are done,

* assuming medium is ready.
* You click on: Continue the update
* you click on: Continue the update,
* you click on: Quit

Initialisations with text interface
...................................

First run on host A.

* You type: **nnfs2 --add-first dirname**
* You answer: **c return** to continue the update
* You hit : return when medium is ready

Second run on host B, assuming medium is ready.

* You type: **nnfs2 --add dirname**
* You answer: **c return** to continue the update

Normal run when initialisations are done, assuming medium is ready.

* You type: **nnfs2**
* You answer: **c return** to continue the update


How to recover from MEDIUM read failure
---------------------------------------

If there is a read error you can rerun NNFS a few times with the hope
that the error will vanish. If it is not the case, put the bad medium in
a trash, take a new one and create immediately a new FS with the current
host.

If the read error is before you start to work on an host. Try to not
work on non-synchronized file in order to avoid future conflicts. Run
NNFS as always before stopping to work on the host.

If the read error is before you leave an host. You will want to copy the
file modified by your session on the other host. To do so, modify all
the file you want to synchronise and rerun NNFS on the host. For example
by running **find . -mtime -1 -print | xargs touch**. If you do not do
this, these files will be synchronised but only after adding the other
host on FS, a run on local the host and a run on the other host.

My experience about floppies is that there is floppies usable only 2 or
3 times and some usable hundreds of time. *Even if they are from the
same computer*.

Historised backups
------------------

Each time you run NNFS the files deleted or modified are moved in a
directory. This history directory is not mirrored!

Archiving modified files in default directory
.............................................

Current date: Sun Sep 16 14:57:47 CEST 2001

Modified file: **foo/bar/Makefile**

Historised file: **.nnfs/history/2001_09/16_14:57.47/foo/bar/Makefile**

So you can easily remove old historised files by date.

Conflicts
---------

If you don't run NNFS before and after working, some update conflicts
may be raised if you work on the same files.

* You run NNFS on host A

* You modify 'foo' on host A

* You run NNFS on host A

* You modify 'foo' on host B

* You run NNFS on host B (you should have done this before modifying the
  file)

The conflict solving method is straightforward, the remote conflicting
file is always copied on local host. If you don't agree, you can
retrieve the local file content from the **.nnfs/history** directory.

This method is not used to solve conflict when adding a new host. In
this case, the most recent file is copied over the old ones

ADVANCED USAGE
==============

Alias
-----

If you run NNFS on several hosts using NNFS,
you must indicate to NNFS that the hosts use the same file system.

The mirrored directory must have the same name on all aliased host
Assuming that the medium is ready, adding an alias with X11 interface.

* You click on: Continue the update
* you click on: +Nickname,
* You click on: Continue the update
* you click on: Quit

Assuming that the medium is ready, adding an alias with text interface.

* You type: **nnfs2 --add-nickname hostname-yet-known-by-nnfs**
* You answer: **c return** to continue the update

Configuration
-------------

The configuration file is **.nnfs/nnfsrc**,
it is created the first time you run NNFS.
It is a hugely commented shell script you can edit.
It is a shell script to allow some default values
to be computed from the system.

If you find this configuration file too complex to edit, you can use the
NNFS X11 interface to edit the file. There is many tips to help you configure.

The more useful options are :

* READ_MEDIUM shell script called by NNFS to read from the medium
* WRITE_MEDIUM shell script called by NNFS to write on the medium
* COMPRESS_FILTER shell script filter called by NNFS to compress
* UNCOMPRESS_FILTER shell script filter called by NNFS to uncompress
* MEDIUM_SIZE size of the medium in bytes, if the medium if bigger than
  2Gb the data will be splitted in multiple files AAA AAB ...
* OUTPUT_FILTER indicate which files should not be mirrored
* CONFIRMATION choose which questions NNFS will ask you
* AUTO_START if true, NNFS start update without user intervention.
* TMP_FILE: set this to *none* if you use NNFS with a big medium
* CROSS_MOUNT_POINT if true NNFS will synchronise all the files even if
  they are on several file systems. The default is false so the mount
  points are not crossed.
* COPY_HARD_LINK if true NNFS will copy hard links as several files not
  hard linked, this is dangerous because an update can break the link, so
  the default is false and the hard linked files are not copied at all.
* MODE_CHANGE_SENSITIVE if true NNFS will copy the file even if only the
  mode change. If false the file will not be synchronized, but if it is,
  its mode will be synchronized. The default is true in order to have a
  perfect synchronization of the mode.


Required minimal filter
.........................

The option hard to configure is OUTPUT_FILTER.
The default value is fine for me but certainly not for you. The
minimal filter must stop the copy of NNFS history and all backup files.

::

    -regex=~$
    -regex=^\.nnfs/history/

Filter files modified by the user connection
........................................................

But, to run NNFS you must be connected, and the connection modify some
file you don't want to copy on other hosts because it is a security
breach or it is a non-sense. For example, any dot file in connection
directory ending by history or authority

::

    -regex=^\.[^/]*(history|authority)$

Filter garbage files
........................................................

If the medium is small, you need to filter the files that are garbage as core,
the executable files or the results of compilation as *.o* files.
I assume here that executables biggers than 50k are not scripts shell or perl.


::

    -type=f size=+50k perm=+111
    -regex=(\.(o|a|so|sl|aux|log|dvi|summary|old)|/(core|a\.out))$


Do not filter scripts (notice the + on line start)
........................................................

The big shell or perl script are filtered, if you don't want this. Tell
NNFS to not filter them.
::

    +regex=\.(pl|sh|tcl)$
    +regex=/configure$

Reference of the filter description
........................................................
    
Each line starts by **-** or **+**, to remove or add to the file set the
files verifying all the conditions in the line.

The conditions are:

* **perm=+xxxx**: The condition is verified if any
  bit indicated in octal are in the file mode. If you indicate ``0111``
  the file mode ``0100``, ``0111``, ``0750`` are verified.
* **perm=-xxxx**: The condition is verified if all the bits indicated in
  octal are in the file mode. If you indicate ``0111`` the file mode
  ``0755`` is verified but not ``0750``.
* **perm=xxxx**: The condition is verified if the file mode is exactly equal
  to the value indicated in octal.
* **size=+xxxx**: The condition is verified if the file size if
  bigger than the size specified. A size in kilo-bytes may be specified as
  ``56k`` and in mega-bytes as ``2m``
* **size=-xxxx**: same as above, but the file size must be smaller.
  The two conditions may appear on the same line.
* **type=dflp**: The type is regular file (``f``), directory (``d``),
  FIFO (``p``) or symbolic link (``l``)
* **regex=aregularexpression**: If you want the expression to match the
  full name, add **^** at the begin and **$** at the end. This condition
  must be at the end of the line.

Synchronisation of UID and GID
------------------------------

If you want to synchonise the files with there UID and GID you need to be
root and to run **rootnnfs2**.


FILES
=====

**~/.nnfs/nnfsrc**
  NNFS configuration

**~/.nnfs/gtkrc**
  NNFS GTK configuration

**~/.nnfs/history**
  Directory where modified or deleted files are saved

**~/.nnfs/log~**
  Some log about NNFS last work

BUGS
====

Limitations:

* Directories must contains less than 16777216 files (need to recompile).
* Less than 128 hosts to synchronise (need to recompile).
* No mirroring of 000 mode file (it is more a feature than a bug).
* No mirroring of hard links (will not be corrected).
* A file may be not mirrored if it is modified in the same second that
  NNFS modify it (will not be corrected).
* If A, B and C are 3 hosts, B filter some files. NNFS runs on A, B, C, A,
  B, C, A, B, C, A, B, C, ... The modified files on A are filtered by B
  and are not copied on C. (very difficult to correct)
* When adding an host to NNFS, symbolics links are copied because the
  MEDIUM doesn't contain the content of the link. The real problem is that
  you can't create a symbolic link with all the good attributes
  (modification time, mode and on some systems UID and GID). So it is
  impossible to fully mirror symbolic links
* In ``g_file.h`` should use ``off_t`` in place of ``long`` but it doesn't
  compile on IRIX MIPS IP30 gcc. So files are limited to 2GB.
* On Mac OS X the symbolic links are often copied because the file system
  does not store modification date of symbolic links.
* On Mac OS X NNFS is unpredictable if only the case is different between
  two file names as with ``README`` and ``Readme``

Bugs:

* In some cases, the GTK progress bar go over 100%.

NO WARRANTY
===========

**This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.**

It is nearly impossible that NNFS do an irrecoverable error. In the
worst case, NNFS will stop. The returned value (error) of all the system
calls are verified and the program tries to analyse the error.
The parameters of the more stressing test on NNFS are the following:
4 hosts

On each host a program create/delete/modify file/links/directories at
the maximum speed.

NNFS runs continuously on each of the host randomly. So, while NNFS
runs, files are modified while it is reading/writing them. Or worstly,
directories are deleted while NNFS analyse the content, or files are
created in directories that NNFS tries to delete

Incredibly, NNFS works in this case thousands of time on a Linux 386
Debian host.

NNFS had been compiled and tested on HPPA 1.1 HPUX 10.20 and MIPS IP30
IRIX 6.5

On Mac OS X it does not work optimaly with symbolic links (see BUGS).

AUTHOR
======

Author: Thierry EXCOFFIER, Author Home Page:
http://perso.univ-lyon1.fr/thierry.excoffier/

NNFS home page: http://perso.univ-lyon1.fr/thierry.excoffier/nnfs.html
