CFLAGS = @OOH323CFLAGS@

noinst_PROGRAMS = ooPlayer
ooPlayer_SOURCES = ooPlayer.c ooPlayer.h oosndrtp.c oosndrtp.h
INCLUDES = -I$(top_builddir)/src -I$(top_builddir)/src/h323
ooPlayer_LDADD = -loostk -ldl -lpthread
ooPlayer_LDFLAGS = -L$(top_builddir)/src

opt:
        $(MAKE) "CFLAGS = -DGNU -D_GNU_SOURCE -D_OOINFO -D_REENTRANT -D_COMPACT -O2" all

debug:
        $(MAKE) "CFLAGS = -g -DGNU -D_GNU_SOURCE -D_OODEBUGC -D_REENTRANT -D_COMPACT" all

EXTRA_DIST = ooPlayer.dsp space.raw states.wav space.wav
