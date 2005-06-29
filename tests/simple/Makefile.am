CFLAGS= @OOH323CFLAGS@
noinst_PROGRAMS = simple
simple_SOURCES = simple.c oosndrtp.c oosndrtp.h
INCLUDES = -I$(top_builddir)/src -I$(top_builddir)/src/h323
simple_LDADD = -looh323c -ldl -lpthread
simple_LDFLAGS = -L$(top_builddir)/src

opt:
        $(MAKE) all

debug:
        $(MAKE) all

profile:
        $(MAKE) all

EXTRA_DIST = simple.dsp
