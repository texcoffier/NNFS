d:
	interactive

clean:
	-rm -f TMP/* lll xxx* yyy* zzz* `id -nu`.AAA *~ number a.out >/dev/null 2>&1
	-rm -rf .nnfs TMP/.??* >/dev/null 2>&1
	# chmod -R 777   doesn't work on HPUX for directory with mode 000
	-chmod -R 777 /tmp/NNFS*
	-rm -rf /tmp/NNFS* >/dev/null 2>&1

