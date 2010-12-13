set(ooPlayer_SOURCES
    ooPlayer.c
    ../../media/ooMediaLibIF.c
    )
   
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/h323_v6)
include_directories(${CMAKE_SOURCE_DIR}/src/h323)
include_directories(${CMAKE_SOURCE_DIR}/media)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)   
endif(COMMAND cmake_policy)

ADD_EXECUTABLE(ooPlayer
    ${ooPlayer_SOURCES}
    )

target_link_libraries(ooPlayer
    ooh323c
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    )
