project(plugin_skeleton)

# Set up Qt4
find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK TRUE)
include(${QT_USE_FILE})

subdirs(src)
