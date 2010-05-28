# the name of the project
project(ooh323c)

cmake_minimum_required(VERSION 2.6.0)

# find and setup threads library for the project
find_package(Threads)

# Set verbose output to make
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Set defines
ADD_DEFINITIONS(-DGNU -D_GNU_SOURCE -D_OOINFO -D_REENTRANT -D_COMPACT)

# tell cmake to process CMakeLists.txt in the subdirectories
add_subdirectory(src)
add_subdirectory(media)
add_subdirectory(tests)

