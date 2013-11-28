
default:
	@echo "Read the README Please."

include Version

clean:
	find . -name "*~" -exec rm {} \;
	cd src ; $(MAKE) clean ; autoconf ; rm -fR Makefile auto*.cache || true
	cd tst ; $(MAKE) clean
	cd doc ; $(MAKE) clean nnfs.html

create_tar:
	ln -s `pwd` ../nnfs-$(V)
	# /* to not copy GIT repository
	cd .. ; tar -cf - nnfs-$(V)/* | gzip -9 >$(V).tar.gz
	rm -r ../nnfs-$(V)

check_tar:
	cd .. && \
	P=`pwd` && \
	cd /tmp && \
	zcat <$$P/$(V).tar.gz | tar -xvf - && \
	cd nnfs-$(V)/src && \
	./configure ; \
	$(MAKE) ; \
	cd ../tst ; \
	./tests

tar:clean create_tar check_tar
	cp ChangeLog doc/nnfs.html /home/exco/public_html/NNFS
	cp Welcome.html /home/exco/public_html/nnfs.html
	mv ../$(V).tar.gz /home/exco/public_html/NNFS
