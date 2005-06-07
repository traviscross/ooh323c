CFLAGS= @OOH323CFLAGS@
noinst_PROGRAMS = simple
simple_SOURCES = simple.c oosndrtp.c oosndrtp.h
INCLUDES = -I$(top_builddir)/src -I$(top_builddir)/src/h323
simple_LDADD = -looh323c -ldl -lpthread
simple_LDFLAGS = -L$(top_builddir)/src

opt:
        $(MAKE) "CFLAGS = -DGNU -D_GNU_SOURCE -D_REENTRANT -D_COMPACT -O2" all

debug:
        $(MAKE) "CFLAGS = -g -DGNU -D_GNU_SOURCE -D_REENTRANT" all

profile:
        $(MAKE) "CFLAGS = -g -pg -DGNU -D_GNU_SOURCE -D_REENTRANT" all

EXTRA_DIST = simple.dsp
