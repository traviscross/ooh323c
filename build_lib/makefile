# Build static run-time libraries

include ../platform.mk

COMFLAGS = $(CVARS_) $(MCFLAGS) $(CDEV_) -D_TRACE
CFLAGS   = $(COMFLAGS) $(CFLAGS_)
IPATHS   = -I../src -I../src/h323_v6 -I../src/h323
DESTDIR  = ..$(PS)lib

all : \
$(DESTDIR)$(PS)$(LIBPFX)ooh323c$(A)

include ../src/objects.mk
include ../src/h323_v6/objects.mk
include ../src/h323/objects.mk

$(DESTDIR)$(PS)$(LIBPFX)ooh323c$(A) : \
$(COMMON_OBJECTS) $(H323V6_OBJECTS) $(H323V4_OBJECTS)
        $(LIBCMD) $(COMMON_OBJECTS) $(H323V6_OBJECTS) $(H323V4_OBJECTS)


include ../src/rules.mk

outdirs :
        perl -e "mkdir ('../lib', 0777) if (! -e '../lib')"

clean :
        $(RM) *$(OBJ)

realclean :
        $(MAKE) clean
        $(RM) $(DESTDIR)$(PS)$(LIBPFX)ooh323c$(A)
