
default:
	@echo "Read the README Please."

include Version

tar:
	@echo "The tar will be created, untarred, compiled and tested"
	@echo "press return"
	@read A
	-rm *~
	cd src ; $(MAKE) clean ; autoconf ; rm -fR Makefile auto*.cache || true
	cd tst ; $(MAKE) clean
	cd doc ; $(MAKE) clean nnfs.html
	ln -s `pwd` ../nnfs-$(V)
	cd .. ; tar -cvf - nnfs-$(V)/* | gzip -9 >$(V).tar.gz
	rm ../nnfs-$(V)
	cp ChangeLog ..
	cd .. && \
	P=`pwd` && \
	cd /tmp && \
	zcat <$$P/$(V).tar.gz | tar -xvf - && \
	cd nnfs-$(V)/src && \
	configure ; \
	$(MAKE) ; \
	cd ../tst ; \
	tests

