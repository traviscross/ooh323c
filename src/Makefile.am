AUTOMKAE_OPTIONS = gnu
CFLAGS = @OOH323CFLAGS@

lib_LIBRARIES = liboostk.a
#liboostk_la_LDFLAGS = -version-info 1:1:0
liboostk_a_SOURCES = context.c decode.c dlist.c encode.c errmgmt.c memheap.c memheap.h ooasn1.h oo.c oochannels.c oochannels.h oo.h ooh245.c ooh245.h oohdr.h ooper.h ooports.c ooports.h ooq931.c ooq931.h oosndrtp.c oosndrtp.h ooSocket.c ooSocket.h ootypes.h perutil.c SList.c SList.h asn1CEvtHndlr.c asn1CEvtHndlr.h ooCalls.c ooCalls.h ooStackCmds.c ooStackCmds.h ooh323.c ooh323.h ooh323ep.c ooh323ep.h printHandler.c printHandler.h rtconv.c rtctype.c rtctype.h h323/H235-SECURITY-MESSAGES.c h323/H235-SECURITY-MESSAGESDec.c h323/H235-SECURITY-MESSAGESEnc.c h323/H235-SECURITY-MESSAGES.h h323/H323-MESSAGES.c h323/H323-MESSAGESDec.c h323/H323-MESSAGESEnc.c h323/H323-MESSAGES.h h323/MULTIMEDIA-SYSTEM-CONTROL.c h323/MULTIMEDIA-SYSTEM-CONTROLDec.c h323/MULTIMEDIA-SYSTEM-CONTROLEnc.c h323/MULTIMEDIA-SYSTEM-CONTROL.h

INCLUDES = -Ih323


opt:
        $(MAKE) "CFLAGS = -DGNU -D_GNU_SOURCE -D_OOINFO -D_REENTRANT -O2 -D_COMPACT" all

debug:
        $(MAKE) "CFLAGS = -g -DGNU -D_GNU_SOURCE -D_OODEBUGC -D_REENTRANT -D_COMPACT"  all

#include_HEADERS = memheap.h ooasn1.h oochannels.h oo.h ooh245.h oohdr.h ooper.h ooports.h ooq931.h oosndrtp.h ooSocket.h ootypes.h SList.h asn1CEvtHndlr.h ooCalls.h ooStackCmds.h ooh323.h ooh323ep.h printHandler.h rtctype.h h323/H235-SECURITY-MESSAGES.h h323/H323-MESSAGES.h h323/MULTIMEDIA-SYSTEM-CONTROL.h h450/Addressing-Data-Elements.h h450/Call-Completion-Operations.h h450/Call-Diversion-Operations.h h450/Call-Hold-Operations.h h450/Call-Intrusion-Operations.h h450/Call-Offer-Operations.h h450/Call-Park-Pickup-Operations.h h450/Call-Transfer-Operations.h h450/Call-Waiting-Operations.h h450/H225-generic-parameters-definition.h h450/H4501-General-Error-List.h h450/H4501-Supplementary-ServiceAPDU-Structure.h h450/Manufacturer-specific-service-extension-definition.h h450/Message-Waiting-Indication-Operations.h h450/Name-Operations.h h450/Remote-Operations-Apdus.h h450/Remote-Operations-Generic-ROS-PDUs.h h450/Remote-Operations-Information-Objects.h h450/Remote-Operations-Useful-Definitions.h

EXTRA_DIST = oostk.dsp
