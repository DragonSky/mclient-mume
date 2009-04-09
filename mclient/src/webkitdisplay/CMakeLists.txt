include_directories(${plugin_skeleton_SOURCE_DIR}/src)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)


# webkitdisplay
set(webkitdisplay_SRCS
    WebKitDisplay.cpp)

set(webkitdisplay_MOC_HDRS
    WebKitDisplay.h)

QT4_WRAP_CPP(webkitdisplay_MOC_SRCS ${webkitdisplay_MOC_HDRS})
add_library(webkitdisplay SHARED ${webkitdisplay_SRCS}
    ${webkitdisplay_MOC_SRCS})
target_link_libraries(webkitdisplay mclientplugin ${QT_LIBRARIES})

INSTALL(TARGETS webkitdisplay
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
