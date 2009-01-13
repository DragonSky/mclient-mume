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

MESSAGE( "\n\n\tStarting to compile mClient...")
# Resource Allocation
file(GLOB sub-dir RELATIVE ${mclient_SOURCE_DIR} "*")
set(mclient_resources "")
foreach(dir ${sub-dir})
  if(IS_DIRECTORY ${mclient_SOURCE_DIR}/${dir})
    file(GLOB sub-res RELATIVE ${mclient_SOURCE_DIR} "${mclient_SOURCE_DIR}/${dir}/*.qrc")
    if (sub-res)
      string(REGEX REPLACE "src/" "" sub-res ${sub-res})
      MESSAGE("\tFound Qt4 resource:\t" ${sub-res})
      set(mclient_resources ${mclient_resources} ${sub-res})
    endif()
  endif()
endforeach()
MESSAGE("\n")

QT4_ADD_RESOURCES(mclient_RCC_SRCS ${mclient_resources})
QT4_WRAP_CPP(mclient_MOC_SRCS ${mclient_MOC_HDRS})

add_executable(mclient ${mclient_SRCS} ${mclient_MOC_SRCS} ${mclient_RCC_SRCS})
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