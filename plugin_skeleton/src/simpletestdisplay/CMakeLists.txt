include_directories(${plugin_skeleton_SOURCE_DIR}/src)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)


# simpletestdisplay
set(simpletestdisplay_SRCS
    SimpleTestDisplay.cpp
    SimpleTest.cpp
    ClientWidget.cpp
    ClientTextEdit.cpp
    ClientLineEdit.cpp
    ClientSocket.cpp
    PowwowWrapper.cpp)

set(simpletestdisplay_MOC_HDRS
    SimpleTestDisplay.h
    SimpleTest.h
    ClientWidget.h
    ClientTextEdit.h
    ClientLineEdit.h
    ClientSocket.h
    PowwowWrapper.h)

QT4_WRAP_CPP(simpletestdisplay_MOC_SRCS ${simpletestdisplay_MOC_HDRS})
add_library(simpletestdisplay SHARED ${simpletestdisplay_SRCS}
    ${simpletestdisplay_MOC_SRCS})
target_link_libraries(simpletestdisplay mclientplugin ${QT_LIBRARIES})

INSTALL(TARGETS simpletestdisplay
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)