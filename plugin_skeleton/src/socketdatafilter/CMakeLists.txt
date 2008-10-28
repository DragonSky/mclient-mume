include_directories(${plugin_skeleton_SOURCE_DIR}/src)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)


# socketdatafilter
set(socketdatafilter_SRCS
    SocketDataFilter.cpp
    SocketData.cpp)

set(socketdatafilter_MOC_HDRS
    SocketDataFilter.h
    SocketData.h)

QT4_WRAP_CPP(socketdatafilter_MOC_SRCS ${socketdatafilter_MOC_HDRS})
add_library(socketdatafilter SHARED ${socketdatafilter_SRCS}
    ${socketdatafilter_MOC_SRCS})
target_link_libraries(socketdatafilter mclientplugin ${QT_LIBRARIES})

INSTALL(TARGETS socketdatafilter
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
