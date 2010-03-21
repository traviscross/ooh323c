set(ooh323c_SOURCES
    ooCmdChannel.c
    ooCmdChannel.h
    ooUtils.c
    ooUtils.h
    ooGkClient.h
    ooGkClient.c
    context.c
    ooCommon.h
    ooDateTime.h
    ooDateTime.c
    decode.c
    dlist.c
    encode.c
    errmgmt.c
    memheap.c
    memheap.h
    ooasn1.h
    ootrace.h
    ootrace.c
    oochannels.c
    oochannels.h
    ooh245.c
    ooh245.h
    oohdr.h
    ooper.h
    ooports.c
    ooports.h
    ooq931.c
    ooq931.h
    ooCapability.c
    ooCapability.h
    ooConfig.c
    ooConfig.h
    ooSocket.c
    ooSocket.h
    ootypes.h
    perutil.c
    eventHandler.c
    eventHandler.h
    ooCalls.c
    ooCalls.h
    ooLogChan.c
    ooLogChan.h
    ooStackCmds.c
    ooStackCmds.h
    ooh323.c
    ooh323.h
    ooh323ep.c
    ooh323ep.h
    printHandler.c
    printHandler.h
    rtctype.c
    rtctype.h
    ooTimer.c
    ooTimer.h
    h323/H235-SECURITY-MESSAGESDec.c
    h323/H235-SECURITY-MESSAGESEnc.c
    h323/H235-SECURITY-MESSAGES.h
    h323_v6/H323-MESSAGES.c
    h323_v6/H323-MESSAGESDec.c
    h323_v6/H323-MESSAGESEnc.c
    h323_v6/H323-MESSAGES.h
    h323_v6/MULTIMEDIA-SYSTEM-CONTROL.c
    h323_v6/MULTIMEDIA-SYSTEM-CONTROLDec.c
    h323_v6/MULTIMEDIA-SYSTEM-CONTROLEnc.c
    )

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/h323_v6)
include_directories(${CMAKE_SOURCE_DIR}/src/h323)

ADD_LIBRARY(ooh323c STATIC
    ${ooh323c_SOURCES}
    )
ADD_LIBRARY(ooh323c_SHARED SHARED
    ${ooh323c_SOURCES}
    )

# The static library target "ooh323c" already has a default OUTPUT_NAME of
# "ooh323c", so we don't need to change it.
# The library target "ooh323c_SHARED" has a default OUTPUT_NAME of
# "ooh323c_SHARED", so change it.

set_target_properties(ooh323c_SHARED
    PROPERTIES OUTPUT_NAME "ooh323c")

# Now the library target "ooh323c" will be named "ooh323c.lib" with MS tools.
# This conflicts with the "ooh323c.lib" import library corresponding to
# "ooh323c.dll", so we add a "_a" suffix:

if (WIN32)
    SET_TARGET_PROPERTIES(ooh323c PROPERTIES SUFFIX "_a.lib")
endif (WIN32)

# Set output directory
set_target_properties(ooh323c ooh323c_SHARED
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

