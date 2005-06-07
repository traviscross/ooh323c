AUTOMAKE_OPTIONS = gnu
CFLAGS = @OOH323CFLAGS@

lib_LIBRARIES = libooh323c.a
#liboostk_la_LDFLAGS = -version-info 1:1:0
libooh323c_a_SOURCES = ooUtils.c ooUtils.h ooGkClient.h ooGkClient.c context.c ooCommon.h ooDateTime.h ooDateTime.c decode.c dlist.c encode.c errmgmt.c memheap.c memheap.h ooasn1.h  ootrace.h ootrace.c oochannels.c oochannels.h ooh245.c ooh245.h oohdr.h ooper.h ooports.c ooports.h ooq931.c ooq931.h ooCapability.c ooCapability.h ooSocket.c ooSocket.h ootypes.h perutil.c eventHandler.c eventHandler.h ooCalls.c ooCalls.h ooLogChan.c ooLogChan.h ooStackCmds.c ooStackCmds.h ooh323.c ooh323.h ooh323ep.c ooh323ep.h printHandler.c printHandler.h rtctype.c rtctype.h ooTimer.c ooTimer.h h323/H235-SECURITY-MESSAGESDec.c h323/H235-SECURITY-MESSAGESEnc.c h323/H235-SECURITY-MESSAGES.h h323/H323-MESSAGES.c h323/H323-MESSAGESDec.c h323/H323-MESSAGESEnc.c h323/H323-MESSAGES.h h323/MULTIMEDIA-SYSTEM-CONTROL.c h323/MULTIMEDIA-SYSTEM-CONTROLDec.c h323/MULTIMEDIA-SYSTEM-CONTROLEnc.c h323/MULTIMEDIA-SYSTEM-CONTROL.h

INCLUDES = -Ih323


opt:
        $(MAKE) "CFLAGS = -DGNU -D_GNU_SOURCE -D_REENTRANT -O2 -D_COMPACT" all

debug:
        $(MAKE) "CFLAGS = -g -DGNU -D_GNU_SOURCE -D_REENTRANT"  all

profile:
        $(MAKE) "CFLAGS = -g -pg -DGNU -D_GNU_SOURCE -D_REENTRANT" all

include_HEADERS = dlist.h ooUtils.h memheap.h ooCommon.h ooDateTime.h ooGkClient.h ooasn1.h oochannels.h ootrace.h ooh245.h ooports.h ooq931.h oohdr.h ooper.h ooSocket.h ooTimer.h ootypes.h eventHandler.h ooCapability.h ooCalls.h ooLogChan.h ooStackCmds.h ooh323.h ooh323ep.h printHandler.h rtctype.h h323/H235-SECURITY-MESSAGES.h h323/H323-MESSAGES.h h323/MULTIMEDIA-SYSTEM-CONTROL.h

EXTRA_DIST = oostk.dsp
