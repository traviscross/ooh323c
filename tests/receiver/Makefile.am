CFLAGS = @OOH323CFLAGS@
noinst_PROGRAMS = ooReceiver
ooReceiver_SOURCES = ooReceiver.c ooReceiver.h oosndrtp.h oosndrtp.c
INCLUDES = -I$(top_builddir)/src -I$(top_builddir)/src/h323
ooReceiver_LDADD = -looh323c -ldl -lpthread
ooReceiver_LDFLAGS = -L$(top_builddir)/src

opt:
        $(MAKE) "CFLAGS = -DGNU -D_GNU_SOURCE -D_REENTRANT -D_COMPACT -O2" all

debug:
        $(MAKE) "CFLAGS = -g -DGNU -D_GNU_SOURCE -D_REENTRANT" all

EXTRA_DIST = ooReceiver.dsp
