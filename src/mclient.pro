SOURCES += main.cpp \
           ./mainwindow/mainwindow.cpp \
           ./mainwindow/textview.cpp \
           ./wrapper/wrapper.cpp \
           ./wrapper/wrapper_tcp.cpp \
           ./wrapper/wrapper_process.cpp \
           ./wrapper/wrapper_tty.cpp \
           ./powwow/beam.c \
           ./powwow/cmd2.c \
           ./powwow/cmd.c \
           ./powwow/edit.c \
           ./powwow/eval.c \
           ./powwow/list.c \
           ./powwow/log.c \
           ./powwow/powmain.c \
           ./powwow/map.c \
           ./powwow/ptr.c \
           ./powwow/tcp.c \
           ./powwow/utils.c \
           ./mainwindow/inputbar.cpp \
           ./wrapper/wrapper_cmd.cpp \
           ./mainwindow/objecteditor.cpp \
           ./configuration/configuration.cpp \
           ./configuration/configdialog.cpp \
           ./configuration/generalpage.cpp \
           ./configuration/ansicombo.cpp \
           ./configuration/colorfontpage.cpp \
           ./mainwindow/keybinder.cpp \
           ./mainwindow/profiledialog.cpp \
           ./mainwindow/profilemanagerdialog.cpp \
           ./mainwindow/profileeditdialog.cpp \
           ./kmuddy/cprofilemanager.cpp \
           ./kmuddy/cprofilesettings.cpp \
 wrapper/wrapper_profile.cpp
HEADERS += ./mainwindow/mainwindow.h \
           ./mainwindow/textview.h \
           ./wrapper/wrapper.h \
           ./wrapper/wrapper_tty.h \
           ./wrapper/wrapper_tcp.h \
           ./wrapper/wrapper_process.h \
           ./powwow/beam.h \
           ./powwow/cmd2.h \
           ./powwow/cmd.h \
           ./powwow/defines.h \
           ./powwow/edit.h \
           ./powwow/eval.h \
           ./powwow/list.h \
           ./powwow/log.h \
           ./powwow/main.h \
           ./powwow/map.h \
           ./powwow/ptr.h \
           ./powwow/tcp.h \
           ./powwow/tty.h \
           ./powwow/utils.h \
           ./win32/timeval.h \
           ./mainwindow/inputbar.h \
           ./wrapper/wrapper_cmd.h \
           ./mainwindow/objecteditor.h \
           ./configuration/configuration.h \
           ./configuration/configdialog.h \
           ./configuration/generalpage.h \
           ./configuration/ansicombo.h \
           ./configuration/colorfontpage.h \
           ./mainwindow/keybinder.h \
           ./mainwindow/profiledialog.h \
           ./mainwindow/profilemanagerdialog.h \
           ./mainwindow/profileeditdialog.h \
           ./kmuddy/cprofilemanager.cpp \
           ./kmuddy/cprofilesettings.cpp \
 wrapper/wrapper_profile.h
TEMPLATE = app
FORMS += ./mainwindow/objecteditor.ui \
         ./configuration/generalpage.ui \
         ./configuration/colorfontpage.ui \
         ./mainwindow/keybinder.ui \
         ./mainwindow/profiledialog.ui \
         ./mainwindow/profilemanagerdialog.ui \
         ./mainwindow/profileeditdialog.ui
QT += network gui
CONFIG += warn_on \
          thread \
          qt \
          network
CONFIG -= release
CONFIG += debug
TARGET = mclient
INCLUDEPATH += . ./mainwindow ./powwow ./win32 ./wrapper ./configuration ./kmuddy
macx : LIBS += 
win32 : LIBS += -llibgnurx
unix : LIBS += -lm -W -Wall -O -pedantic
DEPENDPATH += .
DEFINES += USE_REGEXP=1 MCLIENT=1 VERSION=\\\"1.2.13\\\"
RESOURCES = resources/application.qrc
UI_DIR = ../build/ui
MOC_DIR = ../build/moc
debug {
    DESTDIR = ../bin/debug
    OBJECTS_DIR = ../build/debug-obj
}
release {
    DESTDIR = ../bin/release
    OBJECTS_DIR = ../build/release-obj
}

