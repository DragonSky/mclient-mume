INCLUDE_DIRECTORIES(${mclient_SOURCE_DIR}/ui 
    mclientplugin
    mainwindow)

ADD_DEFINITIONS(${QT_DEFINITIONS})
#ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


set(mclient_SRCS
    main.cpp)

set(mclient_MOC_HDRS
    )

QT4_WRAP_CPP(mclient_MOC_SRCS ${mclient_MOC_HDRS})

add_executable(mclient ${mclient_SRCS}
    ${mclient_MOC_SRCS})
target_link_libraries(mclient mclientplugin mainwindow ${QT_LIBRARIES})


install(TARGETS 
    mclient 
    RUNTIME DESTINATION bin)

add_subdirectory(mainwindow)
add_subdirectory(mclientplugin)
add_subdirectory(socketmanagerio)
add_subdirectory(simpletestdisplay)
add_subdirectory(telnet)
add_subdirectory(mumexml)
