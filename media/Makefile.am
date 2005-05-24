AUTOMAKE_OPTIONS = gnu
CFLAGS = @OOH323CFLAGS@

lib_LTLIBRARIES = liboomedia.la
liboomedia_la_LDFLAGS = -version-info 1:1:0
liboomedia_la_SOURCES = g711.c g711.h ooCommon.c ooCommon.h oomedialx.c oomedialx.h oortp.c oortp.h ooSock.c ooSock.h
liboomedia_la_LIBADD = -lpthread

EXTRA_DIST= doc oomedia.dsp ooWave.c ooWave.h

opt:
        $(MAKE) "CFLAGS = -DGNU -D_GNU_SOURCE -D_OOINFO -D_REENTRANT -O2" all

debug:
        $(MAKE) "CFLAGS = -g -DGNU -D_GNU_SOURCE -D_OODEBUGC -D_REENTRANT" all

profile:
        $(MAKE) "CFLAGS = -g -pg -DGNU -D_GNU_SOURCE -D_REENTRANT" all
