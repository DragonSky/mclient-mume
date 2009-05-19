add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

# Core Source Files
foreach(dir ${CMAKE_CURRENT_SOURCE_DIR})
    file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${dir}/*.cpp")
    set(mclient_SRCS ${mclient_SRCS} ${files})
endforeach()

# Core Header Files
foreach(dir ${CMAKE_CURRENT_SOURCE_DIR})
    file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${dir}/*.h")
    set(mclient_MOC_HDRS ${mclient_MOC_HDRS} ${files})
endforeach()

# Resource Files
foreach(dir mainwindow mclientplugin)
    file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${dir}/*.qrc")
    set(mclient_RCS ${mclient_RCS} ${files})
endforeach()

# UI Files
foreach(dir mainwindow mclientplugin)
    file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${dir}/*.ui")
    set(mclient_UIS ${mclient_UIS} ${files})
endforeach()

# Add Windows Resource File
if(WIN32)
    set(mclient_SRCS ${mclient_SRCS} mclient.rc)
endif(WIN32)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    mainwindow
    mclientplugin
)
  
QT4_WRAP_UI(mclient_UIS_H ${mclient_UIS})
QT4_WRAP_CPP(mclient_MOC_SRCS ${mclient_MOC_HDRS})
QT4_ADD_RESOURCES(mclient_RCC_SRCS ${mclient_RCS})

add_executable(mclient
    ${mclient_SRCS}
    ${mclient_MOC_SRCS}
    ${mclient_UIS_H}
    ${mclient_RCC_SRCS}
)

target_link_libraries(mclient
    ${QT_QTMAIN_LIBRARY}
    ${QT_LIBRARIES}
    mclientplugin
    mainwindow
)

# Installation Location
install(TARGETS mclient RUNTIME DESTINATION bin)

# mClient's Core Engine
add_subdirectory(mclientplugin)
add_subdirectory(mainwindow)

# Various Dynamic Plugins
add_subdirectory(socketmanagerio)
add_subdirectory(simplelineinput)
add_subdirectory(telnet)
add_subdirectory(mumexml)
add_subdirectory(webkitdisplay)
add_subdirectory(commandmanager)
#add_subdirectory(simpletestdisplay)
