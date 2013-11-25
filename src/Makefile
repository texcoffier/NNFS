#######################################################################
# DO NOT EDIT
#######################################################################
# I think you should not edit this file.
# Run "./configure" script, and, may be, patch the Makefile
#######################################################################

include ../Version


# Uncomment to include all debugging informations and memory checkers
# DEBUG="-DG_DEBUG"

CFLAGS=-O $(OTHER_CFLAGS) -Wall -g -I/home/exco/include -Wall -pedantic -DHAVE_GETOPT_H -DXTHREADS -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/X11R6/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include   $(DEBUG) -DNNFS_VERSION=\"Version:$(V)\" -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_DIRENT_H=1 -DHAVE_SYS_WAIT_H=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_UNISTD_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_STRINGS_H=1 -DHAVE_STDARG_H=1 -DHAVE_LIMITS_H=1 -DHAVE_TIME_H=1 -DHAVE_UTIME_H=1 -DHAVE_ERRNO_H=1 -DHAVE_REGEX_H=1 -DHAVE_FCNTL_H=1 -DHAVE_SIGNAL_H=1 -DHAVE_PWD_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_VFS_H=1 -DHAVE_SYS_STATFS_H=1 -DHAVE_DIRENT_H=1 -DHAVE_STRFTIME=1 -DHAVE_ALLOCA_H=1 -DHAVE_ALLOCA=1 -DRETSIGTYPE=void -DHAVE_UTIME_NULL=1 -DHAVE_VPRINTF=1 -DHAVE_GETDOMAINNAME=1 -DHAVE_REGCOMP=1 -DHAVE_STRCHR=1 -DHAVE_MEMCPY=1 -DHAVE_GETHOSTNAME=1 -DHAVE_MKDIR=1 -DHAVE_RE_COMP=1 -DHAVE_RMDIR=1 -DHAVE_SELECT=1 -DHAVE_STRDUP=1 -DHAVE_STRTOL=1 -DHAVE_LCHOWN=1 -DDO_NOT_NEED_INCLUDE_POSIX_SOURCE=1 -DHAVE_ALPHASORT=1 -DHAVE_GTK_INIT=1 -DHAVE_F_BAVAIL=1 -DHAVE_F_BAVAIL=1 -DHAVE_STATFS2=1 -DHAVE_STATFS2=1 -DHAVE_DIRENT_CONST=1 -DHAVE_LINK_MTIME=1 

OBJ=g_nnfs.o g_update.o g_merge.o g_file.o g_dir.o g_option.o \
	g_init.o g_header.o g_tree.o g_ui.o g_ui_txt.o \
	g_openp.o g_filter.o \
	g_host.o g_help.o g_unix.o g_finish.o g_histo.o\
	g_read.o g_state.o g_trim.o g_dspbar.o g_report.o \
        g_secure.o g_debug.o g_malloc.o g_nnfsrc.o g_copy.o g_split.o  g_ui_gtk.o


nnfs2:$(OBJ)
	$(CC) $(CFLAGS) $(OBJ)  -L/usr/X11R6/lib -lgtk-x11-2.0 -lgdk-x11-2.0 -lXrandr -lXi -lXinerama -lXext -latk-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lfontconfig -lXcursor -lpango-1.0 -lcairo -lXrender -lX11 -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0   -lm -o $@
	@echo
	@echo 'To verify if NNFS2 is working you can go in 'tst' and run "tests"'
	@echo
	@echo 'Copy "nnfs2" (or "rootnnfs2") executable where you want'

rootnnfs:
	-rm *.o
	$(MAKE) rootnnfs2.real
	-rm *.o

rootnnfs2.real:
	OTHER_CFLAGS="-DROOTNNFS" ; export OTHER_CFLAGS ; $(MAKE) nnfs2 && mv nnfs2 rootnnfs2


ftp:pgp
	echo   "exco\n\
		umask 022\n\
		cd /vol/ftp/nnfs\n\
		bin\n\
		prompt no\n\
		hash\n\
		mdel *\n\
		put ../nnfs-$(V).tar.gz nnfs-$(V).tar.gz\n\
		put ../nnfs-$(V).tar.gz nnfs.tar.gz\n\
		quit" | \
	ftp ftp710.univ-lyon1.fr

pgp:tar
	cd .. ; \
	pgp -sb nnfs-$(V).tar ; \
	( \
	echo "If you don't use PGP, untar the nnfs-$(V).tar" ; \
	echo "If you use PGP, retrieve my Public key and type:" ; \
	echo "	pgp nnfs-$(V).tar.asc nnfs-$(V).tar" ; \
	echo "To verify the archive." ; \
	) >>READMEpgp ; \
	tar -cvf - READMEpgp nnfs-$(V).tar nnfs-$(V).tar.asc | \
		gzip -9 >nnfs-$(V).tar.gz ;\
	rm -f nnfs-$(V).tar nnfs-$(V).tar.asc READMEpgp ;

create_defaults: nnfsrc_ gtkrc_

nnfsrc_:/home/exco/.nnfs/nnfsrc
	sed \
	-e 's/CONFIRMATION=".*"/CONFIRMATION="always"/' \
	-e 's/AUTO_START=".*"/AUTO_START="no"/' \
	-e 's/[\\%]/&&/g' -e 's/"/\\"/g' \
	-e 's/^/g_fprintf(f,"/' -e 's/$$/\\n") ;/' \
	<$? >nnfsrc
gtkrc_:/home/exco/.nnfs/gtkrc
	sed -e 's/[\\%]/&&/g' -e 's/"/\\"/g' \
	    -e 's/^/g_fprintf(f,"/' -e 's/$$/\\n") ;/' \
         <$? >gtkrc

clean:
	-rm -f ../*~
	-rm -rf *~ *.o nnfs2 rootnnfs2 TAGS config.* \
	       nnfs.info nnfs.ps nnfs.dvi nnfs-$(V) *.bak
	cd ../tst ; make clean

depend:TAGS
	makedepend $(CFLAGS) $(OBJ:.o=.c)
TAGS:*.c *.h
	-etags *.[ch]

# DO NOT DELETE

edanticg_nnfs.o: g_nnfs.h g_header.h g_type.h /usr/include/unistd.h
edanticg_nnfs.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_nnfs.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_nnfs.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_nnfs.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_nnfs.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_nnfs.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_nnfs.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_nnfs.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_nnfs.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_nnfs.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_nnfs.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_nnfs.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_nnfs.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_nnfs.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_nnfs.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_nnfs.o: /usr/include/bits/stdio_lim.h
edanticg_nnfs.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_nnfs.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_nnfs.o: /usr/include/bits/stat.h g_debug.h g_debugP.h g_malloc.h
edanticg_nnfs.o: /usr/include/stdlib.h g_init.h g_unix.h g_nnfsrc.h g_merge.h
edanticg_nnfs.o: g_update.h g_host.h g_finish.h g_tree.h g_secure.h g_histo.h
edanticg_nnfs.o: g_dspbar.h g_report.h g_read.h g_trim.h g_dir.h g_ui.h
edanticg_nnfs.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
edanticg_nnfs.o: /usr/include/ctype.h /usr/include/endian.h
edanticg_nnfs.o: /usr/include/bits/endian.h g_filter.h /usr/include/regex.h
edanticg_update.o: g_const.h /usr/include/string.h /usr/include/features.h
edanticg_update.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_update.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_update.o: /usr/include/sys/types.h /usr/include/bits/types.h
edanticg_update.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
edanticg_update.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_update.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_update.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_update.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_update.o: /usr/include/bits/waitflags.h
edanticg_update.o: /usr/include/bits/waitstatus.h g_update.h g_nnfs.h
edanticg_update.o: g_header.h g_type.h /usr/include/unistd.h
edanticg_update.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
edanticg_update.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_update.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_update.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_update.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_update.o: /usr/include/bits/stdio_lim.h
edanticg_update.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_update.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_update.o: /usr/include/bits/stat.h g_debug.h g_debugP.h g_malloc.h
edanticg_update.o: g_dspbar.h g_filter.h /usr/include/regex.h g_tree.h
edanticg_update.o: /usr/include/stdlib.h
edanticg_merge.o: g_merge.h g_nnfs.h g_header.h g_type.h
edanticg_merge.o: /usr/include/unistd.h /usr/include/features.h
edanticg_merge.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_merge.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
edanticg_merge.o: /usr/include/bits/wordsize.h
edanticg_merge.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_merge.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_merge.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_merge.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_merge.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_merge.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_merge.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_merge.o: /usr/include/bits/waitflags.h
edanticg_merge.o: /usr/include/bits/waitstatus.h /usr/include/stdio.h
edanticg_merge.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_merge.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_merge.o: /usr/include/gconv.h
edanticg_merge.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_merge.o: /usr/include/bits/stdio_lim.h
edanticg_merge.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_merge.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_merge.o: /usr/include/bits/stat.h g_update.h g_debug.h g_debugP.h
edanticg_merge.o: g_dir.h g_malloc.h g_dspbar.h g_secure.h
edanticg_file.o: g_file.h /usr/include/stdio.h /usr/include/features.h
edanticg_file.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_file.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_file.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_file.o: /usr/include/bits/typesizes.h /usr/include/libio.h
edanticg_file.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_file.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_file.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_file.o: /usr/include/bits/stdio_lim.h
edanticg_file.o: /usr/include/bits/sys_errlist.h /usr/include/sys/stat.h
edanticg_file.o: /usr/include/bits/stat.h g_type.h /usr/include/unistd.h
edanticg_file.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
edanticg_file.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_file.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_file.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_file.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_file.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_file.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_file.o: g_debug.h g_debugP.h g_malloc.h g_secure.h g_merge.h
edanticg_file.o: g_nnfs.h g_header.h g_option.h g_config.h g_state.h
edanticg_file.o: g_dspbar.h /usr/include/errno.h /usr/include/bits/errno.h
edanticg_file.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
edanticg_file.o: /usr/include/asm-i486/errno.h
edanticg_file.o: /usr/include/asm-generic/errno.h
edanticg_file.o: /usr/include/asm-generic/errno-base.h /usr/include/stdlib.h
edanticg_dir.o: g_dir.h g_type.h /usr/include/unistd.h
edanticg_dir.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_dir.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_dir.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_dir.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_dir.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_dir.o: g_file.h /usr/include/stdio.h /usr/include/libio.h
edanticg_dir.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_dir.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_dir.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_dir.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
edanticg_dir.o: /usr/include/sys/stat.h /usr/include/bits/stat.h g_const.h
edanticg_dir.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_dir.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_dir.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_dir.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_dir.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_dir.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_dir.o: g_config.h g_malloc.h g_secure.h g_debug.h g_debugP.h
edanticg_dir.o: /usr/include/stdlib.h /usr/include/dirent.h
edanticg_dir.o: /usr/include/bits/dirent.h
edanticg_option.o: g_option.h g_type.h /usr/include/unistd.h
edanticg_option.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_option.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_option.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_option.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_option.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_option.o: g_debug.h g_debugP.h g_help.h g_header.h g_const.h
edanticg_option.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_option.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_option.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_option.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_option.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_option.o: /usr/include/bits/waitflags.h
edanticg_option.o: /usr/include/bits/waitstatus.h /usr/include/stdio.h
edanticg_option.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_option.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_option.o: /usr/include/gconv.h
edanticg_option.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_option.o: /usr/include/bits/stdio_lim.h
edanticg_option.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
edanticg_option.o: /usr/include/getopt.h
edanticg_init.o: g_init.h g_nnfs.h g_header.h g_type.h /usr/include/unistd.h
edanticg_init.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_init.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_init.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_init.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_init.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_init.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_init.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_init.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_init.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_init.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_init.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_init.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_init.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_init.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_init.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_init.o: /usr/include/bits/stdio_lim.h
edanticg_init.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_init.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_init.o: /usr/include/bits/stat.h g_malloc.h g_debug.h g_debugP.h
edanticg_init.o: g_unix.h g_nnfsrc.h g_histo.h
edanticg_header.o: g_header.h g_type.h /usr/include/unistd.h
edanticg_header.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_header.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_header.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_header.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_header.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_header.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_header.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_header.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_header.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_header.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_header.o: /usr/include/bits/waitflags.h
edanticg_header.o: /usr/include/bits/waitstatus.h /usr/include/stdio.h
edanticg_header.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_header.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_header.o: /usr/include/gconv.h
edanticg_header.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_header.o: /usr/include/bits/stdio_lim.h
edanticg_header.o: /usr/include/bits/sys_errlist.h g_debug.h g_debugP.h
edanticg_header.o: g_malloc.h g_secure.h g_help.h g_host.h g_option.h
edanticg_header.o: /usr/include/stdlib.h
edanticg_tree.o: g_debug.h g_type.h /usr/include/unistd.h
edanticg_tree.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_tree.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_tree.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_tree.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_tree.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_tree.o: g_debugP.h g_malloc.h g_nnfs.h g_header.h g_const.h
edanticg_tree.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_tree.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_tree.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_tree.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_tree.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_tree.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_tree.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_tree.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_tree.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_tree.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_tree.o: /usr/include/bits/stdio_lim.h
edanticg_tree.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_tree.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_tree.o: /usr/include/bits/stat.h g_read.h g_tree.h g_dir.h
edanticg_tree.o: g_secure.h g_copy.h g_dspbar.h g_report.h g_histo.h
edanticg_tree.o: g_update.h g_filter.h /usr/include/regex.h
edanticg_tree.o: /usr/include/stdlib.h /usr/include/errno.h
edanticg_tree.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
edanticg_tree.o: /usr/include/asm/errno.h /usr/include/asm-i486/errno.h
edanticg_tree.o: /usr/include/asm-generic/errno.h
edanticg_tree.o: /usr/include/asm-generic/errno-base.h /usr/include/utime.h
edanticg_ui.o: g_ui.h /usr/include/sys/types.h /usr/include/features.h
edanticg_ui.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_ui.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_ui.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_ui.o: /usr/include/bits/typesizes.h /usr/include/time.h
edanticg_ui.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
edanticg_ui.o: /usr/include/ctype.h /usr/include/endian.h
edanticg_ui.o: /usr/include/bits/endian.h g_unix.h g_config.h g_type.h
edanticg_ui.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
edanticg_ui.o: /usr/include/bits/confname.h g_option.h g_state.h
edanticg_ui.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_ui.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_ui.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_ui.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_ui.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
edanticg_ui.o: g_file.h /usr/include/sys/stat.h /usr/include/bits/stat.h
edanticg_ui.o: g_const.h /usr/include/string.h /usr/include/sys/wait.h
edanticg_ui.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_ui.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_ui.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_ui.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_ui.o: g_nnfsrc.h /usr/include/stdlib.h g_secure.h
edanticg_ui_txt.o: g_const.h /usr/include/string.h /usr/include/features.h
edanticg_ui_txt.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_ui_txt.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_ui_txt.o: /usr/include/sys/types.h /usr/include/bits/types.h
edanticg_ui_txt.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
edanticg_ui_txt.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_ui_txt.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_ui_txt.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_ui_txt.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_ui_txt.o: /usr/include/bits/waitflags.h
edanticg_ui_txt.o: /usr/include/bits/waitstatus.h g_debug.h g_type.h
edanticg_ui_txt.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
edanticg_ui_txt.o: /usr/include/bits/confname.h g_debugP.h g_ui.h
edanticg_ui_txt.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
edanticg_ui_txt.o: /usr/include/ctype.h /usr/include/endian.h
edanticg_ui_txt.o: /usr/include/bits/endian.h g_unix.h g_config.h g_option.h
edanticg_ui_txt.o: g_state.h /usr/include/stdio.h /usr/include/libio.h
edanticg_ui_txt.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_ui_txt.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_ui_txt.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_ui_txt.o: /usr/include/bits/stdio_lim.h
edanticg_ui_txt.o: /usr/include/bits/sys_errlist.h g_file.h
edanticg_ui_txt.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
edanticg_ui_txt.o: g_nnfsrc.h /usr/include/stdlib.h g_report.h g_secure.h
edanticg_ui_txt.o: g_malloc.h
edanticg_openp.o: g_openp.h g_type.h /usr/include/unistd.h
edanticg_openp.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_openp.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_openp.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_openp.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_openp.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_openp.o: g_secure.h /usr/include/stdio.h /usr/include/libio.h
edanticg_openp.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_openp.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_openp.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_openp.o: /usr/include/bits/stdio_lim.h
edanticg_openp.o: /usr/include/bits/sys_errlist.h g_const.h
edanticg_openp.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_openp.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_openp.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_openp.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_openp.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_openp.o: /usr/include/bits/waitflags.h
edanticg_openp.o: /usr/include/bits/waitstatus.h g_malloc.h g_debug.h
edanticg_openp.o: g_debugP.h /usr/include/sys/stat.h /usr/include/bits/stat.h
edanticg_filter.o: g_filter.h /usr/include/sys/types.h
edanticg_filter.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_filter.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
edanticg_filter.o: /usr/include/bits/wordsize.h
edanticg_filter.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_filter.o: /usr/include/bits/typesizes.h /usr/include/time.h
edanticg_filter.o: /usr/include/regex.h g_file.h /usr/include/stdio.h
edanticg_filter.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_filter.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_filter.o: /usr/include/gconv.h
edanticg_filter.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_filter.o: /usr/include/bits/stdio_lim.h
edanticg_filter.o: /usr/include/bits/sys_errlist.h /usr/include/sys/stat.h
edanticg_filter.o: /usr/include/bits/stat.h g_type.h /usr/include/unistd.h
edanticg_filter.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
edanticg_filter.o: g_const.h /usr/include/string.h /usr/include/sys/wait.h
edanticg_filter.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_filter.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_filter.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_filter.o: /usr/include/bits/waitflags.h
edanticg_filter.o: /usr/include/bits/waitstatus.h g_nnfs.h g_header.h
edanticg_filter.o: g_option.h g_config.h g_state.h g_debug.h g_debugP.h
edanticg_filter.o: g_malloc.h g_update.h /usr/include/limits.h
edanticg_filter.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/limits.h
edanticg_filter.o: /usr/include/stdlib.h
edanticg_host.o: /usr/include/unistd.h /usr/include/features.h
edanticg_host.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_host.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
edanticg_host.o: /usr/include/bits/wordsize.h
edanticg_host.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_host.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_host.o: /usr/include/string.h /usr/include/stdlib.h g_host.h
edanticg_host.o: g_header.h g_type.h g_const.h /usr/include/sys/types.h
edanticg_host.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_host.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_host.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_host.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_host.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_host.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_host.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_host.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_host.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_host.o: /usr/include/bits/stdio_lim.h
edanticg_host.o: /usr/include/bits/sys_errlist.h g_option.h g_debug.h
edanticg_host.o: g_debugP.h g_malloc.h g_unix.h g_config.h g_state.h g_file.h
edanticg_host.o: /usr/include/sys/stat.h /usr/include/bits/stat.h g_nnfsrc.h
edanticg_host.o: g_help.h g_nnfs.h g_secure.h
edanticg_help.o: g_help.h g_header.h g_type.h /usr/include/unistd.h
edanticg_help.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_help.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_help.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_help.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_help.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_help.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_help.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_help.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_help.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_help.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_help.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_help.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_help.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_help.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_help.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_help.o: /usr/include/bits/stdio_lim.h
edanticg_help.o: /usr/include/bits/sys_errlist.h g_host.h g_option.h
edanticg_help.o: g_debug.h g_debugP.h /usr/include/stdlib.h
edanticg_unix.o: g_unix.h g_config.h g_type.h /usr/include/unistd.h
edanticg_unix.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_unix.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_unix.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_unix.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_unix.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_unix.o: g_option.h g_state.h /usr/include/stdio.h
edanticg_unix.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_unix.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_unix.o: /usr/include/gconv.h
edanticg_unix.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_unix.o: /usr/include/bits/stdio_lim.h
edanticg_unix.o: /usr/include/bits/sys_errlist.h g_file.h
edanticg_unix.o: /usr/include/sys/stat.h /usr/include/bits/stat.h g_const.h
edanticg_unix.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_unix.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_unix.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_unix.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_unix.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_unix.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_unix.o: g_nnfsrc.h g_debug.h g_debugP.h g_secure.h g_malloc.h
edanticg_unix.o: g_copy.h g_split.h /usr/include/math.h
edanticg_unix.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
edanticg_unix.o: /usr/include/bits/mathcalls.h /usr/include/stdlib.h
edanticg_unix.o: /usr/include/sys/vfs.h /usr/include/sys/statfs.h
edanticg_unix.o: /usr/include/bits/statfs.h /usr/include/limits.h
edanticg_unix.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/limits.h
edanticg_unix.o: /usr/include/pwd.h
edanticg_finish.o: g_finish.h g_nnfs.h g_header.h g_type.h
edanticg_finish.o: /usr/include/unistd.h /usr/include/features.h
edanticg_finish.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_finish.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
edanticg_finish.o: /usr/include/bits/wordsize.h
edanticg_finish.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_finish.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_finish.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_finish.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_finish.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_finish.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_finish.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_finish.o: /usr/include/bits/waitflags.h
edanticg_finish.o: /usr/include/bits/waitstatus.h /usr/include/stdio.h
edanticg_finish.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_finish.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_finish.o: /usr/include/gconv.h
edanticg_finish.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_finish.o: /usr/include/bits/stdio_lim.h
edanticg_finish.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_finish.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_finish.o: /usr/include/bits/stat.h g_debug.h g_debugP.h g_secure.h
edanticg_finish.o: g_malloc.h g_filter.h /usr/include/regex.h g_nnfsrc.h
edanticg_histo.o: g_debug.h g_type.h /usr/include/unistd.h
edanticg_histo.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_histo.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_histo.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_histo.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_histo.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_histo.o: g_debugP.h g_malloc.h g_copy.h /usr/include/stdio.h
edanticg_histo.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_histo.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_histo.o: /usr/include/gconv.h
edanticg_histo.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_histo.o: /usr/include/bits/stdio_lim.h
edanticg_histo.o: /usr/include/bits/sys_errlist.h g_nnfs.h g_header.h
edanticg_histo.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_histo.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_histo.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_histo.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_histo.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_histo.o: /usr/include/bits/waitflags.h
edanticg_histo.o: /usr/include/bits/waitstatus.h g_option.h g_config.h
edanticg_histo.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_histo.o: /usr/include/bits/stat.h g_openp.h g_secure.h
edanticg_histo.o: /usr/include/errno.h /usr/include/bits/errno.h
edanticg_histo.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
edanticg_histo.o: /usr/include/asm-i486/errno.h
edanticg_histo.o: /usr/include/asm-generic/errno.h
edanticg_histo.o: /usr/include/asm-generic/errno-base.h
edanticg_read.o: g_nnfs.h g_header.h g_type.h /usr/include/unistd.h
edanticg_read.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_read.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_read.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_read.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_read.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_read.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_read.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_read.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_read.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_read.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_read.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_read.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_read.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_read.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_read.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_read.o: /usr/include/bits/stdio_lim.h
edanticg_read.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_read.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_read.o: /usr/include/bits/stat.h g_read.h g_secure.h g_debug.h
edanticg_read.o: g_debugP.h g_dspbar.h g_malloc.h g_tree.h g_histo.h
edanticg_read.o: /usr/include/stdlib.h /usr/include/errno.h
edanticg_read.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
edanticg_read.o: /usr/include/asm/errno.h /usr/include/asm-i486/errno.h
edanticg_read.o: /usr/include/asm-generic/errno.h
edanticg_read.o: /usr/include/asm-generic/errno-base.h g_copy.h
edanticg_state.o: g_state.h /usr/include/stdio.h /usr/include/features.h
edanticg_state.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_state.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_state.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_state.o: /usr/include/bits/typesizes.h /usr/include/libio.h
edanticg_state.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_state.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_state.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_state.o: /usr/include/bits/stdio_lim.h
edanticg_state.o: /usr/include/bits/sys_errlist.h g_file.h
edanticg_state.o: /usr/include/sys/stat.h /usr/include/bits/stat.h g_type.h
edanticg_state.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
edanticg_state.o: /usr/include/bits/confname.h g_const.h
edanticg_state.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_state.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_state.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_state.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_state.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_state.o: /usr/include/bits/waitflags.h
edanticg_state.o: /usr/include/bits/waitstatus.h g_debug.h g_debugP.h
edanticg_state.o: g_malloc.h /usr/include/ctype.h /usr/include/endian.h
edanticg_state.o: /usr/include/bits/endian.h /usr/include/stdlib.h
edanticg_trim.o: g_debug.h g_type.h /usr/include/unistd.h
edanticg_trim.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_trim.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_trim.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_trim.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_trim.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_trim.o: g_debugP.h g_malloc.h g_nnfs.h g_header.h g_const.h
edanticg_trim.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_trim.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_trim.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_trim.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_trim.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_trim.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_trim.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_trim.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_trim.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_trim.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_trim.o: /usr/include/bits/stdio_lim.h
edanticg_trim.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_trim.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_trim.o: /usr/include/bits/stat.h g_trim.h g_finish.h g_dspbar.h
edanticg_trim.o: /usr/include/utime.h
edanticg_dspbar.o: g_nnfs.h g_header.h g_type.h /usr/include/unistd.h
edanticg_dspbar.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_dspbar.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_dspbar.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_dspbar.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_dspbar.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_dspbar.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_dspbar.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_dspbar.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_dspbar.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_dspbar.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_dspbar.o: /usr/include/bits/waitflags.h
edanticg_dspbar.o: /usr/include/bits/waitstatus.h /usr/include/stdio.h
edanticg_dspbar.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_dspbar.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_dspbar.o: /usr/include/gconv.h
edanticg_dspbar.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_dspbar.o: /usr/include/bits/stdio_lim.h
edanticg_dspbar.o: /usr/include/bits/sys_errlist.h g_option.h g_config.h
edanticg_dspbar.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_dspbar.o: /usr/include/bits/stat.h g_dspbar.h g_unix.h g_nnfsrc.h
edanticg_dspbar.o: /usr/include/limits.h
edanticg_dspbar.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/limits.h
edanticg_report.o: g_report.h /usr/include/stdio.h /usr/include/features.h
edanticg_report.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_report.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_report.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_report.o: /usr/include/bits/typesizes.h /usr/include/libio.h
edanticg_report.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_report.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_report.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_report.o: /usr/include/bits/stdio_lim.h
edanticg_report.o: /usr/include/bits/sys_errlist.h g_nnfs.h g_header.h
edanticg_report.o: g_type.h /usr/include/unistd.h
edanticg_report.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
edanticg_report.o: g_const.h /usr/include/string.h /usr/include/sys/types.h
edanticg_report.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_report.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_report.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_report.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_report.o: /usr/include/bits/waitflags.h
edanticg_report.o: /usr/include/bits/waitstatus.h g_option.h g_config.h
edanticg_report.o: g_state.h g_file.h /usr/include/sys/stat.h
edanticg_report.o: /usr/include/bits/stat.h g_malloc.h g_debug.h g_debugP.h
edanticg_secure.o: g_secure.h /usr/include/stdio.h /usr/include/features.h
edanticg_secure.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_secure.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_secure.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_secure.o: /usr/include/bits/typesizes.h /usr/include/libio.h
edanticg_secure.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_secure.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_secure.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_secure.o: /usr/include/bits/stdio_lim.h
edanticg_secure.o: /usr/include/bits/sys_errlist.h g_const.h
edanticg_secure.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_secure.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_secure.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_secure.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_secure.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_secure.o: /usr/include/bits/waitflags.h
edanticg_secure.o: /usr/include/bits/waitstatus.h g_malloc.h g_type.h
edanticg_secure.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
edanticg_secure.o: /usr/include/bits/confname.h g_debug.h g_debugP.h g_unix.h
edanticg_secure.o: g_config.h g_option.h g_state.h g_file.h
edanticg_secure.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
edanticg_secure.o: g_nnfsrc.h /usr/include/errno.h /usr/include/bits/errno.h
edanticg_secure.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
edanticg_secure.o: /usr/include/asm-i486/errno.h
edanticg_secure.o: /usr/include/asm-generic/errno.h
edanticg_secure.o: /usr/include/asm-generic/errno-base.h
edanticg_secure.o: /usr/include/dirent.h /usr/include/bits/dirent.h
edanticg_secure.o: /usr/include/stdlib.h /usr/include/fcntl.h
edanticg_secure.o: /usr/include/bits/fcntl.h
edanticg_debug.o: g_debug.h g_type.h /usr/include/unistd.h
edanticg_debug.o: /usr/include/features.h /usr/include/sys/cdefs.h
edanticg_debug.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
edanticg_debug.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_debug.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_debug.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
edanticg_debug.o: g_debugP.h g_malloc.h g_unix.h g_config.h g_option.h
edanticg_debug.o: g_state.h /usr/include/stdio.h /usr/include/libio.h
edanticg_debug.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_debug.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_debug.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_debug.o: /usr/include/bits/stdio_lim.h
edanticg_debug.o: /usr/include/bits/sys_errlist.h g_file.h
edanticg_debug.o: /usr/include/sys/stat.h /usr/include/bits/stat.h g_const.h
edanticg_debug.o: /usr/include/string.h /usr/include/sys/types.h
edanticg_debug.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_debug.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_debug.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_debug.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_debug.o: /usr/include/bits/waitflags.h
edanticg_debug.o: /usr/include/bits/waitstatus.h g_nnfsrc.h
edanticg_debug.o: /usr/include/stdlib.h
edanticg_malloc.o: /usr/include/stdio.h /usr/include/features.h
edanticg_malloc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_malloc.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_malloc.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_malloc.o: /usr/include/bits/typesizes.h /usr/include/libio.h
edanticg_malloc.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_malloc.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_malloc.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_malloc.o: /usr/include/bits/stdio_lim.h
edanticg_malloc.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
edanticg_malloc.o: /usr/include/string.h g_malloc.h g_type.h
edanticg_malloc.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
edanticg_malloc.o: /usr/include/bits/confname.h g_debug.h g_debugP.h
edanticg_nnfsrc.o: g_nnfsrc.h /usr/include/stdio.h /usr/include/features.h
edanticg_nnfsrc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_nnfsrc.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_nnfsrc.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
edanticg_nnfsrc.o: /usr/include/bits/typesizes.h /usr/include/libio.h
edanticg_nnfsrc.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_nnfsrc.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_nnfsrc.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_nnfsrc.o: /usr/include/bits/stdio_lim.h
edanticg_nnfsrc.o: /usr/include/bits/sys_errlist.h g_type.h
edanticg_nnfsrc.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
edanticg_nnfsrc.o: /usr/include/bits/confname.h g_config.h g_help.h
edanticg_nnfsrc.o: g_header.h g_const.h /usr/include/string.h
edanticg_nnfsrc.o: /usr/include/sys/types.h /usr/include/time.h
edanticg_nnfsrc.o: /usr/include/sys/wait.h /usr/include/signal.h
edanticg_nnfsrc.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
edanticg_nnfsrc.o: /usr/include/sys/resource.h /usr/include/bits/resource.h
edanticg_nnfsrc.o: /usr/include/bits/time.h /usr/include/bits/waitflags.h
edanticg_nnfsrc.o: /usr/include/bits/waitstatus.h g_malloc.h g_debug.h
edanticg_nnfsrc.o: g_debugP.h g_secure.h g_unix.h g_option.h g_state.h
edanticg_nnfsrc.o: g_file.h /usr/include/sys/stat.h /usr/include/bits/stat.h
edanticg_nnfsrc.o: /usr/include/errno.h /usr/include/bits/errno.h
edanticg_nnfsrc.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
edanticg_nnfsrc.o: /usr/include/asm-i486/errno.h
edanticg_nnfsrc.o: /usr/include/asm-generic/errno.h
edanticg_nnfsrc.o: /usr/include/asm-generic/errno-base.h
edanticg_nnfsrc.o: /usr/include/stdlib.h nnfsrc
edanticg_copy.o: g_const.h /usr/include/string.h /usr/include/features.h
edanticg_copy.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_copy.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_copy.o: /usr/include/sys/types.h /usr/include/bits/types.h
edanticg_copy.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
edanticg_copy.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_copy.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_copy.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_copy.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_copy.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
edanticg_copy.o: g_debug.h g_type.h /usr/include/unistd.h
edanticg_copy.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
edanticg_copy.o: g_debugP.h g_copy.h /usr/include/stdio.h
edanticg_copy.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_copy.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_copy.o: /usr/include/gconv.h
edanticg_copy.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_copy.o: /usr/include/bits/stdio_lim.h
edanticg_copy.o: /usr/include/bits/sys_errlist.h g_secure.h g_malloc.h
edanticg_copy.o: /usr/include/stdlib.h /usr/include/sys/stat.h
edanticg_copy.o: /usr/include/bits/stat.h /usr/include/fcntl.h
edanticg_copy.o: /usr/include/bits/fcntl.h
edanticg_split.o: g_const.h /usr/include/string.h /usr/include/features.h
edanticg_split.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_split.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_split.o: /usr/include/sys/types.h /usr/include/bits/types.h
edanticg_split.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
edanticg_split.o: /usr/include/time.h /usr/include/sys/wait.h
edanticg_split.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_split.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_split.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_split.o: /usr/include/bits/waitflags.h
edanticg_split.o: /usr/include/bits/waitstatus.h g_debug.h g_type.h
edanticg_split.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
edanticg_split.o: /usr/include/bits/confname.h g_debugP.h g_secure.h
edanticg_split.o: /usr/include/stdio.h /usr/include/libio.h
edanticg_split.o: /usr/include/_G_config.h /usr/include/wchar.h
edanticg_split.o: /usr/include/bits/wchar.h /usr/include/gconv.h
edanticg_split.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_split.o: /usr/include/bits/stdio_lim.h
edanticg_split.o: /usr/include/bits/sys_errlist.h g_malloc.h g_split.h
edanticg_split.o: g_config.h g_unix.h g_option.h g_state.h g_file.h
edanticg_split.o: /usr/include/sys/stat.h /usr/include/bits/stat.h g_nnfsrc.h
edanticg_split.o: /usr/include/stdlib.h
edanticg_ui_gtk.o: /usr/include/locale.h /usr/include/features.h
edanticg_ui_gtk.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
edanticg_ui_gtk.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stddef.h
edanticg_ui_gtk.o: /usr/include/bits/locale.h /usr/include/gtk-2.0/gtk/gtk.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdk.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkcairo.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkcolor.h
edanticg_ui_gtk.o: /usr/include/cairo/cairo.h
edanticg_ui_gtk.o: /usr/include/cairo/cairo-features.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdktypes.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/galloca.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gtypes.h
edanticg_ui_gtk.o: /usr/lib/glib-2.0/include/glibconfig.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gmacros.h /usr/include/limits.h
edanticg_ui_gtk.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/limits.h
edanticg_ui_gtk.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/float.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/garray.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gasyncqueue.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gthread.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gerror.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gquark.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gatomic.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gbacktrace.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gcache.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/glist.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gmem.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gcompletion.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gconvert.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gdataset.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gdate.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gdir.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gfileutils.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/ghash.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/ghook.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/giochannel.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gmain.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gslist.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gstring.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gunicode.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gutils.h
edanticg_ui_gtk.o: /usr/lib/gcc/i486-linux-gnu/4.0.3/include/stdarg.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gkeyfile.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gmappedfile.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gmarkup.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gmessages.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gnode.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/goption.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gpattern.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gprimes.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gqsort.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gqueue.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/grand.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/grel.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gscanner.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gshell.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gspawn.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gstrfuncs.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gthreadpool.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gtimer.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib/gtree.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-attributes.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-font.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-coverage.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-types.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/glib-object.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gboxed.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gtype.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/genums.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gobject.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gvalue.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gparam.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gclosure.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gsignal.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gmarshal.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gparamspecs.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gsourceclosure.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gtypemodule.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gtypeplugin.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gvaluearray.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gobject/gvaluetypes.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-break.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-item.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-context.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-fontmap.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-fontset.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-engine.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-glyph.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-script.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-enum-types.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-layout.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-glyph-item.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-tabs.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pango-renderer.h
edanticg_ui_gtk.o: /usr/lib/gtk-2.0/include/gdkconfig.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkpixbuf.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkrgb.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf-features.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf-core.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf-transform.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf-animation.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf-simple-anim.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf-io.h
edanticg_ui_gtk.o: /usr/include/stdio.h /usr/include/bits/types.h
edanticg_ui_gtk.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
edanticg_ui_gtk.o: /usr/include/libio.h /usr/include/_G_config.h
edanticg_ui_gtk.o: /usr/include/wchar.h /usr/include/bits/wchar.h
edanticg_ui_gtk.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
edanticg_ui_gtk.o: /usr/include/bits/sys_errlist.h
edanticg_ui_gtk.o: /usr/include/glib-2.0/gmodule.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf-loader.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk-pixbuf/gdk-pixbuf-enum-types.h
edanticg_ui_gtk.o: /usr/include/pango-1.0/pango/pangocairo.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkcursor.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkdisplay.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkevents.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkdnd.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkinput.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkdrawable.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkgc.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkenumtypes.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkfont.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkimage.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkkeys.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkdisplaymanager.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkpango.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkpixmap.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkproperty.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkregion.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkscreen.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkselection.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkspawn.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkvisual.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gdk/gdkwindow.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkaboutdialog.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkdialog.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkwindow.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkaccelgroup.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkenums.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkbin.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcontainer.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkwidget.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkobject.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktypeutils.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktypebuiltins.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkdebug.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkadjustment.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkstyle.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtksettings.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkrc.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkobject.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkstate.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkrelationtype.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkaccellabel.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtklabel.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmisc.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmenu.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmenushell.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkaccelmap.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkaccessible.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atk.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkaction.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkcomponent.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkutil.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkdocument.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkeditabletext.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atktext.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkgobjectaccessible.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkhyperlink.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkhypertext.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkimage.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atknoopobject.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atknoopobjectfactory.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkobjectfactory.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkregistry.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkobjectfactory.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkrelation.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkrelationset.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkselection.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkstateset.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkstreamablecontent.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atktable.h
edanticg_ui_gtk.o: /usr/include/atk-1.0/atk/atkvalue.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkaction.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkactiongroup.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkitemfactory.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkalignment.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkarrow.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkaspectframe.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkframe.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkbbox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkbox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkbindings.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkimage.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcalendar.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtksignal.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmarshal.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcelllayout.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcellrenderer.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcelleditable.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreeviewcolumn.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreemodel.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreesortable.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcellrenderercombo.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcellrenderertext.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcellrendererpixbuf.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcellrendererprogress.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcellrenderertoggle.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcellview.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcheckbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktogglebutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcheckmenuitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmenuitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkclipboard.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkselection.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkclist.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkhscrollbar.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkscrollbar.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkrange.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkvscrollbar.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcolorbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcolorsel.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkvbox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcolorseldialog.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcombo.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkhbox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcombobox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreeview.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkdnd.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcomboboxentry.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkctree.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkcurve.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkdrawingarea.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkeditable.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkentry.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkimcontext.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkentrycompletion.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkliststore.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreemodelfilter.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkeventbox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkexpander.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfilesel.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfixed.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfilechooserbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfilechooser.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfilefilter.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfilechooserdialog.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfilechooser.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfilechooserwidget.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfontbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkfontsel.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkgamma.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkgc.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkhandlebox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkhbbox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkhpaned.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkpaned.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkhruler.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkruler.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkhscale.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkscale.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkhseparator.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkseparator.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkiconfactory.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkicontheme.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkiconview.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkimagemenuitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkimcontextsimple.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkimmulticontext.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkinputdialog.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkinvisible.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtklayout.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtklist.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtklistitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmain.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmenubar.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmenutoolbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmenu.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktoolbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktoolitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktooltips.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmessagedialog.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkmodules.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtknotebook.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkoldeditable.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkoptionmenu.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkpixmap.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkplug.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtksocket.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkpreview.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkprogress.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkprogressbar.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkradioaction.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktoggleaction.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkradiobutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkradiomenuitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkradiotoolbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktoggletoolbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktoolbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkscrolledwindow.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkviewport.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkseparatormenuitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkseparatortoolitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtksizegroup.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkspinbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkstatusbar.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkstock.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktable.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktearoffmenuitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktext.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktextbuffer.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktexttagtable.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktexttag.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktextiter.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktextchild.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktextmark.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktextview.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktipsquery.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktoggletoolbutton.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktoolbar.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktoolitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktree.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreednd.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreeitem.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreemodelsort.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreeselection.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtktreestore.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkuimanager.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkvbbox.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkversion.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkvpaned.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkvruler.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkvscale.h
edanticg_ui_gtk.o: /usr/include/gtk-2.0/gtk/gtkvseparator.h g_ui.h
edanticg_ui_gtk.o: /usr/include/sys/types.h /usr/include/time.h
edanticg_ui_gtk.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
edanticg_ui_gtk.o: /usr/include/ctype.h /usr/include/endian.h
edanticg_ui_gtk.o: /usr/include/bits/endian.h g_unix.h g_config.h g_type.h
edanticg_ui_gtk.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
edanticg_ui_gtk.o: /usr/include/bits/confname.h g_option.h g_state.h g_file.h
edanticg_ui_gtk.o: /usr/include/sys/stat.h /usr/include/bits/stat.h g_const.h
edanticg_ui_gtk.o: /usr/include/string.h /usr/include/sys/wait.h
edanticg_ui_gtk.o: /usr/include/signal.h /usr/include/bits/sigset.h
edanticg_ui_gtk.o: /usr/include/bits/signum.h /usr/include/sys/resource.h
edanticg_ui_gtk.o: /usr/include/bits/resource.h /usr/include/bits/time.h
edanticg_ui_gtk.o: /usr/include/bits/waitflags.h
edanticg_ui_gtk.o: /usr/include/bits/waitstatus.h g_nnfsrc.h
edanticg_ui_gtk.o: /usr/include/stdlib.h g_debug.h g_debugP.h g_malloc.h
edanticg_ui_gtk.o: g_filter.h /usr/include/regex.h g_nnfs.h g_header.h
edanticg_ui_gtk.o: g_update.h g_secure.h gtkrc
