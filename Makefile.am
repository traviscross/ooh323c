AUTOMAKE_OPTIONS = gnu
CFLAGS=@OOH323CFLAGS@

opt:
        $(MAKE) "CFLAGS = $(CFLAGS) -O2 -D_COMPACT" all

debug:
        $(MAKE) "CFLAGS = $(CFLAGS) -g" all

profile:
        $(MAKE) "CFLAGS = $(CFLAGS) -g -pg" all

SUBDIRS = . src @OOMEDIADIR@ tests
EXTRA_DIST = doc ooh323c.dsw
