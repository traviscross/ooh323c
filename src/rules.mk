# C/C++ to object file compilation rules

context$(OBJ): ../src/context.c ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/context.c


decode$(OBJ): ../src/decode.c ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/decode.c


dlist$(OBJ): ../src/dlist.c ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/dlist.c


encode$(OBJ): ../src/encode.c ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/encode.c


errmgmt$(OBJ): ../src/errmgmt.c ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/errmgmt.c


eventHandler$(OBJ): ../src/eventHandler.c ../src/eventHandler.h \
  ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/eventHandler.c


memheap$(OBJ): ../src/memheap.c ../src/memheap.h ../src/ooasn1.h \
  ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/memheap.c


ooCalls$(OBJ): ../src/ooCalls.c ../src/ootrace.h ../src/ooCommon.h \
  ../src/ootypes.h ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooCalls.h ../src/ooLogChan.h ../src/ooCapability.h \
  ../src/ooUtils.h ../src/ooports.h ../src/oochannels.h ../src/ooh245.h \
  ../src/ooq931.h ../src/ooGkClient.h ../src/ooh323ep.h ../src/ooConfig.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooCalls.c


ooCapability$(OBJ): ../src/ooCapability.c ../src/ooCapability.h \
  ../src/ootypes.h ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooCalls.h ../src/ooLogChan.h ../src/ooh323ep.h ../src/ooConfig.h \
  ../src/ooGkClient.h ../src/ooports.h ../src/ooq931.h ../src/ooUtils.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooCapability.c


ooCmdChannel$(OBJ): ../src/ooCmdChannel.c ../src/ooStackCmds.h \
  ../src/ootypes.h ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooq931.h ../src/ooh245.h ../src/ooCapability.h \
  ../src/oochannels.h ../src/ooCalls.h ../src/ooLogChan.h \
  ../src/ooh323ep.h ../src/ooConfig.h ../src/ooGkClient.h \
  ../src/ooports.h ../src/ooCmdChannel.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooCmdChannel.c


ooConfig$(OBJ): ../src/ooConfig.c ../src/dlist.h ../src/ooasn1.h \
  ../src/ootrace.h ../src/ooCommon.h ../src/ooConfig.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooConfig.c


ooDateTime$(OBJ): ../src/ooDateTime.c ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooDateTime.c


ooGkClient$(OBJ): ../src/ooGkClient.c ../src/ooGkClient.h ../src/ooasn1.h \
  ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h ../src/ootypes.h \
  ../src/ooSocket.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooasn1.h ../src/h323_v6/H323-MESSAGES.h \
  ../src/h323/H235-SECURITY-MESSAGES.h ../src/ooasn1.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooports.h \
  ../src/oochannels.h ../src/ooCalls.h ../src/ooLogChan.h \
  ../src/ooCapability.h ../src/printHandler.h ../src/eventHandler.h \
  ../src/ooDateTime.h ../src/ooq931.h ../src/ooh323.h ../src/ooh323ep.h \
  ../src/ooConfig.h ../src/ooTimer.h ../src/ooUtils.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooGkClient.c


ooLogChan$(OBJ): ../src/ooLogChan.c ../src/ooCalls.h ../src/ooLogChan.h \
  ../src/ootypes.h ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooCapability.h ../src/ooh323ep.h ../src/ooConfig.h \
  ../src/ooGkClient.h ../src/ooports.h ../src/ooq931.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooLogChan.c


ooSocket$(OBJ): ../src/ooSocket.c ../src/ooSocket.h ../src/ooasn1.h \
  ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooSocket.c


ooStackCmds$(OBJ): ../src/ooStackCmds.c ../src/ooStackCmds.h ../src/ootypes.h \
  ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooCommon.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooasn1.h ../src/h323_v6/H323-MESSAGES.h \
  ../src/h323/H235-SECURITY-MESSAGES.h ../src/ooasn1.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooh323ep.h \
  ../src/ooCapability.h ../src/ooCalls.h ../src/ooLogChan.h \
  ../src/ooConfig.h ../src/ooGkClient.h ../src/ooports.h ../src/ooq931.h \
  ../src/ooCmdChannel.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooStackCmds.c


ooTimer$(OBJ): ../src/ooTimer.c ../src/ooDateTime.h ../src/ooCommon.h \
  ../src/ooTimer.h ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooSocket.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooTimer.c


ooUtils$(OBJ): ../src/ooUtils.c ../src/ooUtils.h ../src/ootypes.h \
  ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooCommon.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooasn1.h ../src/h323_v6/H323-MESSAGES.h \
  ../src/h323/H235-SECURITY-MESSAGES.h ../src/ooasn1.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooUtils.c


oochannels$(OBJ): ../src/oochannels.c ../src/ooports.h ../src/ootypes.h \
  ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooCommon.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooasn1.h ../src/h323_v6/H323-MESSAGES.h \
  ../src/h323/H235-SECURITY-MESSAGES.h ../src/ooasn1.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/oochannels.h \
  ../src/ooCalls.h ../src/ooLogChan.h ../src/ooCapability.h \
  ../src/ooq931.h ../src/ooh245.h ../src/ooh323.h ../src/printHandler.h \
  ../src/eventHandler.h ../src/ooGkClient.h ../src/ooTimer.h \
  ../src/ooh323ep.h ../src/ooConfig.h ../src/ooStackCmds.h \
  ../src/ooCmdChannel.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/oochannels.c


ooh245$(OBJ): ../src/ooh245.c ../src/ooh245.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h ../src/ooCapability.h \
  ../src/ootypes.h ../src/ooSocket.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/oochannels.h ../src/ooCalls.h ../src/ooLogChan.h ../src/ooq931.h \
  ../src/printHandler.h ../src/eventHandler.h ../src/ooh323ep.h \
  ../src/ooConfig.h ../src/ooGkClient.h ../src/ooports.h ../src/ooTimer.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooh245.c


ooh323$(OBJ): ../src/ooh323.c ../src/ootypes.h ../src/ooSocket.h \
  ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooq931.h ../src/oochannels.h ../src/ooCalls.h ../src/ooLogChan.h \
  ../src/ooCapability.h ../src/ooh245.h ../src/printHandler.h \
  ../src/eventHandler.h ../src/ooh323.h ../src/ooh323ep.h \
  ../src/ooConfig.h ../src/ooGkClient.h ../src/ooports.h ../src/ooTimer.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooh323.c


ooh323ep$(OBJ): ../src/ooh323ep.c ../src/ooh323ep.h ../src/ooCapability.h \
  ../src/ootypes.h ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooCalls.h ../src/ooLogChan.h ../src/ooConfig.h \
  ../src/ooGkClient.h ../src/ooports.h ../src/ooq931.h \
  ../src/ooStackCmds.h ../src/ooCmdChannel.h ../src/ooUtils.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooh323ep.c


ooports$(OBJ): ../src/ooports.c ../src/ooports.h ../src/ootypes.h \
  ../src/ooSocket.h ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooCommon.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooasn1.h ../src/h323_v6/H323-MESSAGES.h \
  ../src/h323/H235-SECURITY-MESSAGES.h ../src/ooasn1.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooh323ep.h \
  ../src/ooCapability.h ../src/ooCalls.h ../src/ooLogChan.h \
  ../src/ooConfig.h ../src/ooGkClient.h ../src/ooq931.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooports.c


ooq931$(OBJ): ../src/ooq931.c ../src/ooq931.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h ../src/ootypes.h ../src/ooSocket.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/oochannels.h ../src/ooCalls.h ../src/ooLogChan.h \
  ../src/ooCapability.h ../src/printHandler.h ../src/eventHandler.h \
  ../src/ooh323.h ../src/ooh245.h ../src/ooh323ep.h ../src/ooConfig.h \
  ../src/ooGkClient.h ../src/ooports.h ../src/ooUtils.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ooq931.c


ootrace$(OBJ): ../src/ootrace.c ../src/ootypes.h ../src/ooSocket.h \
  ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooCapability.h ../src/ooq931.h ../src/ooh245.h \
  ../src/oochannels.h ../src/ooCalls.h ../src/ooLogChan.h \
  ../src/ooh323ep.h ../src/ooConfig.h ../src/ooGkClient.h \
  ../src/ooports.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/ootrace.c


perutil$(OBJ): ../src/perutil.c ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h ../src/ooper.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/perutil.c


printHandler$(OBJ): ../src/printHandler.c ../src/printHandler.h \
  ../src/eventHandler.h ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h \
  ../src/ooCommon.h ../src/ootypes.h ../src/ooSocket.h \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/h323_v6/H323-MESSAGES.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/rtctype.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/printHandler.c


rtctype$(OBJ): ../src/rtctype.c ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h ../src/rtctype.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/rtctype.c


H323-MESSAGES$(OBJ): ../src/h323_v6/H323-MESSAGES.c \
  ../src/h323_v6/H323-MESSAGES.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/h323_v6/H323-MESSAGES.c


H323-MESSAGESDec$(OBJ): ../src/h323_v6/H323-MESSAGESDec.c \
  ../src/h323_v6/H323-MESSAGES.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/eventHandler.h ../src/ooasn1.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/h323_v6/H323-MESSAGESDec.c


H323-MESSAGESEnc$(OBJ): ../src/h323_v6/H323-MESSAGESEnc.c \
  ../src/h323_v6/H323-MESSAGES.h ../src/ooasn1.h ../src/dlist.h \
  ../src/ootrace.h ../src/ooCommon.h ../src/h323/H235-SECURITY-MESSAGES.h \
  ../src/ooasn1.h ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h \
  ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/h323_v6/H323-MESSAGESEnc.c


MULTIMEDIA-SYSTEM-CONTROL$(OBJ): ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.c \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.c


MULTIMEDIA-SYSTEM-CONTROLDec$(OBJ):  \
 ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROLDec.c \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h ../src/eventHandler.h \
  ../src/ooasn1.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROLDec.c


MULTIMEDIA-SYSTEM-CONTROLEnc$(OBJ):  \
 ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROLEnc.c \
  ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROL.h ../src/ooasn1.h \
  ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h ../src/ooCommon.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/h323_v6/MULTIMEDIA-SYSTEM-CONTROLEnc.c


H235-SECURITY-MESSAGESDec$(OBJ): ../src/h323/H235-SECURITY-MESSAGESDec.c \
  ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h \
  ../src/h323/H235-SECURITY-MESSAGES.h ../src/eventHandler.h \
  ../src/ooasn1.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/h323/H235-SECURITY-MESSAGESDec.c


H235-SECURITY-MESSAGESEnc$(OBJ): ../src/h323/H235-SECURITY-MESSAGESEnc.c \
  ../src/ooasn1.h ../src/dlist.h ../src/ootrace.h ../src/ooCommon.h \
  ../src/h323/H235-SECURITY-MESSAGES.h
        $(CC) -c $(CFLAGS) $(IPATHS) ../src/h323/H235-SECURITY-MESSAGESEnc.c

