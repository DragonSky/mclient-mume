INCLUDE_DIRECTORIES(${plugin_skeleton_SOURCE_DIR}/ui 
    mclientplugin
    socketdatafilter)

ADD_DEFINITIONS(${QT_DEFINITIONS})
#ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


set(plugin_skeleton_SRCS
    plugin_skeleton.cpp)

set(plugin_skeleton_MOC_HDRS
    )

QT4_WRAP_CPP(plugin_skeleton_MOC_SRCS ${plugin_skeleton_MOC_HDRS})

add_executable(plugin_skeleton ${plugin_skeleton_SRCS}
    ${plugin_skeleton_MOC_SRCS})
target_link_libraries(plugin_skeleton mclientplugin ${QT_LIBRARIES})


install(TARGETS 
    plugin_skeleton 
    RUNTIME DESTINATION bin)

add_subdirectory(mumexml)
add_subdirectory(mclientplugin)
#add_subdirectory(socketdatafilter)
add_subdirectory(socketmanagerio)
add_subdirectory(simpletestdisplay)
add_subdirectory(telnet)