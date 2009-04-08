AM_CFLAGS= @OOH323CFLAGS@
noinst_PROGRAMS = h323peer
h323peer_SOURCES = h323peer.c
INCLUDES = -I$(top_builddir)/src -I$(top_builddir)/src/h323
h323peer_LDADD = -looh323c -ldl -lpthread
h323peer_LDFLAGS = -L$(top_builddir)/src

opt:
        $(MAKE) all

debug:
        $(MAKE) all
