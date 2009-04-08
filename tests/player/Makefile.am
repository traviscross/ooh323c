AM_CFLAGS = @OOH323CFLAGS@

noinst_PROGRAMS = ooPlayer
ooPlayer_SOURCES = ooPlayer.c ooPlayer.h oosndrtp.c oosndrtp.h
INCLUDES = -I$(top_builddir)/src -I$(top_builddir)/src/h323
ooPlayer_LDADD = -looh323c -ldl -lpthread
ooPlayer_LDFLAGS = -L$(top_builddir)/src

opt:
        $(MAKE) all

debug:
        $(MAKE) all

EXTRA_DIST = ooPlayer.dsp space.raw states.wav space.wav
