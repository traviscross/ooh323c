AUTOMKAE_OPTIONS = gnu


opt:
        $(MAKE) "CFLAGS = -DGNU -D_GNU_SOURCE -D_OOINFO -D_REENTRANT -D_COMPACT -O2" all

debug:
        $(MAKE) "CFLAGS = -g -DGNU -D_GNU_SOURCE -D_OODEBUGC -D_REENTRANT -D_COMPACT" all

SUBDIRS = . src media tests
EXTRA_DIST = doc ooh323c.dsw
