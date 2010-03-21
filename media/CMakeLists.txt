include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(oomedia_SOURCES
    g711.c
    g711.h
    ooCommon.c
    ooCommon.h
    oomedialx.c
    oomedialx.h
    oortp.c
    oortp.h
    ooSock.c
    ooSock.h
    )
   
ADD_LIBRARY(oomedia STATIC
    ${oomedia_SOURCES}
    )
ADD_LIBRARY(oomedia_SHARED SHARED
    ${oomedia_SOURCES}
    )

# The static library target "oomedia" already has a default OUTPUT_NAME of
# "oomedia", so we don't need to change it.
# The library target "oomedia_SHARED" has a default OUTPUT_NAME of
# "oomedia_SHARED", so change it.

set_target_properties(oomedia_SHARED
    PROPERTIES OUTPUT_NAME "oomedia")

# Now the library target "oomedia" will be named "oomedia.lib" with MS tools.
# This conflicts with the "oomedia.lib" import library corresponding to
# "oomedia.dll", so we add a "_a" suffix:

if (WIN32)
    SET_TARGET_PROPERTIES(oomedia PROPERTIES SUFFIX "_a.lib")
endif (WIN32)

# Set output directory
set_target_properties(oomedia oomedia_SHARED
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

