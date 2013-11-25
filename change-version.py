#!/usr/bin/python

old = "2.1.8"
new = "2.1.9"

for i in [ \
           ["Version"            , old         , new         ], \
           ["Welcome.html"       , old         , new         ], \
           ["src/Makefile"       , old         , new         ], \
           ]:

    try:
       f = open(i[0], "r")
    except IOError:
       continue
    print "Read", i[0],
    content = f.read()
    f.close()

    print "\tReplace", i[1], "by", i[2],
    content = content.replace(i[1], i[2])

    print "\tWrite", i[0]
    f = open(i[0], "w")
    f.write(content)
    f.close()
