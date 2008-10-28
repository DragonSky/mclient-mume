cmake_minimum_required (VERSION 2.6)
project(plugin_skeleton)

# Make sure we know where the executable is
SET(EXECUTABLE_OUTPUT_PATH "${plugin_skeleton_BINARY_DIR}/bin"
    CACHE INTERNAL "")
SET(LIBRARY_OUTPUT_PATH "${plugin_skeleton_BINARY_DIR}/bin/plugins"
    CACHE INTERNAL "")

# Set up Qt4
find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK TRUE)
include(${QT_USE_FILE})

subdirs(src)

IF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC"  )
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC"  )
    ENDIF("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")



#CONFIGURATIONS debug
