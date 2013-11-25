nnfs.html:nnfs.sgml
	db2html nnfs.sgml
	mv nnfs/index.html nnfs.html
	rm -r nnfs

# Not nice (too much space above the item list)
nnfs.ps:nnfs.sgml
	db2ps nnfs.sgml


nnfs2.1:nnfs.sgml
	docbook2man $?


clean:
	-rm -f *~ nnfs.junk nnfs.tex nnfs.log nnfs.aux nnfs.dvi nnfs.ps nnfs2.1 manpage.links manpage.refs 2>/dev/null



