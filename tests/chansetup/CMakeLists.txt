set(chansetup_SOURCES
    h323peer.c
    )
   
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/h323_v6)
include_directories(${CMAKE_SOURCE_DIR}/src/h323)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)   
endif(COMMAND cmake_policy)

ADD_EXECUTABLE(chansetup
    ${chansetup_SOURCES}
    )

target_link_libraries(chansetup
    ooh323c
    ${CMAKE_THREAD_LIBS_INIT}
    )
