CFLAGS= @OOH323CFLAGS@
noinst_PROGRAMS = h323peer
h323peer_SOURCES = h323peer.c
INCLUDES = -I$(top_builddir)/src -I$(top_builddir)/src/h323
h323peer_LDADD = -loostk -ldl -lpthread
h323peer_LDFLAGS = -L$(top_builddir)/src

opt:
        $(MAKE) "CFLAGS = -DGNU -D_GNU_SOURCE -D_REENTRANT -D_COMPACT -O2" all

debug:
        $(MAKE) "CFLAGS = -g -DGNU -D_GNU_SOURCE -D_REENTRANT" all
