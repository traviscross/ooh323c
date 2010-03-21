# the name of the project
project(ooh323c)

cmake_minimum_required(VERSION 2.6.0)

# find and setup threads library for the project
find_package(Threads)

# Set verbose output to make
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Set defines
ADD_DEFINITIONS(-DGNU)
ADD_DEFINITIONS(-D_GNU_SOURCE)
ADD_DEFINITIONS(-D_OOINFO)
ADD_DEFINITIONS(-D_REENTRANT)
ADD_DEFINITIONS(-D_COMPACT)

# tell cmake to process CMakeLists.txt in the subdirectories
SUBDIRS(
    src
    media
    tests
    )

