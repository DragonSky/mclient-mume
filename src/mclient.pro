SOURCES += ./main.cpp \
           ./mainwindow/MainWindow.cpp \
           ./mainwindow/ClientManager.cpp \
           ./mainwindow/MapperManager.cpp \
           ./mainwindow/GroupManager.cpp \
           ./mainwindow/ActionManager.cpp \
           ./client/ClientTextEdit.cpp \
           ./client/ClientLineEdit.cpp \
           ./client/PowwowWrapper.cpp \
           ./client/ClientSocket.cpp \
           ./client/ClientProcess.cpp \
           ./client/wrapper_tcp.cpp \
           ./client/wrapper_process.cpp \
           ./client/wrapper_tty.cpp \
           ./client/wrapper_cmd.cpp \
           ./client/wrapper_profile.cpp \
           ./dialogs/ObjectEditor.cpp \
           ./dialogs/KeyBinder.cpp \
           ./dialogs/InternalEditor.cpp \
           ./dialogs/ConnectDialog.cpp \
           ./dialogs/ProfileEditDialog.cpp \
           ./dialogs/ProfileManagerDialog.cpp \
           ./profile/cprofilemanager.cpp \
           ./profile/cprofilesettings.cpp \
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
           ./configuration/configuration.cpp \
           ./configuration/configdialog.cpp \
           ./configuration/generalpage.cpp \
           ./configuration/ansicombo.cpp \
           ./configuration/colorfontpage.cpp \
           ./mapper/proxy/telnetfilter.cpp \
           ./mapper/parser/patterns.cpp \
           ./mapper/mapdata/mapdata.cpp \
          ./mapper/mapdata/mmapper2room.cpp \
          ./mapper/mapdata/mmapper2exit.cpp \
          ./mapper/mapdata/roomfactory.cpp \
          ./mapper/mapdata/drawstream.cpp \
          ./mapper/mapdata/roomselection.cpp \
          ./mapper/mapdata/customaction.cpp \
          ./mapper/mainwindow/roomeditattrdlg.cpp \
          ./mapper/mainwindow/infomarkseditdlg.cpp \
          ./mapper/mainwindow/findroomsdlg.cpp \
          ./mapper/display/mapwindow.cpp \
          ./mapper/display/mapcanvas.cpp \
          ./mapper/display/connectionselection.cpp \
          ./mapper/display/prespammedpath.cpp \
          ./mapper/proxy/connectionlistener.cpp \
          ./mapper/proxy/proxy.cpp \
          ./mapper/parser/parser.cpp \
          ./mapper/parser/mumexmlparser.cpp \
          ./mapper/parser/abstractparser.cpp \
          ./mapper/parser/roompropertysetter.cpp \
          ./mapper/expandoracommon/component.cpp \
          ./mapper/expandoracommon/coordinate.cpp \
          ./mapper/expandoracommon/frustum.cpp \
          ./mapper/expandoracommon/abstractroomfactory.cpp \
          ./mapper/expandoracommon/init.cpp \
          ./mapper/expandoracommon/property.cpp \
          ./mapper/expandoracommon/mmapper2event.cpp \
          ./mapper/expandoracommon/parseevent.cpp \
          ./mapper/mapfrontend/intermediatenode.cpp \
          ./mapper/mapfrontend/mmappermap.cpp \
          ./mapper/mapfrontend/mapaction.cpp \
          ./mapper/mapfrontend/mapfrontend.cpp \
          ./mapper/mapfrontend/roomcollection.cpp \
          ./mapper/mapfrontend/roomlocker.cpp \
          ./mapper/mapfrontend/searchtreenode.cpp \
          ./mapper/pathmachine/approved.cpp \
          ./mapper/pathmachine/experimenting.cpp \
          ./mapper/pathmachine/pathmachine.cpp \
          ./mapper/pathmachine/mmapper2pathmachine.cpp \
          ./mapper/pathmachine/roomsignalhandler.cpp \
          ./mapper/pathmachine/path.cpp \
          ./mapper/pathmachine/pathparameters.cpp \
          ./mapper/pathmachine/syncing.cpp \
          ./mapper/pathmachine/onebyone.cpp \
          ./mapper/pathmachine/crossover.cpp \
          ./mapper/mapstorage/roomsaver.cpp \
          ./mapper/mapstorage/abstractmapstorage.cpp \
          ./mapper/mapstorage/mapstorage.cpp \
          ./mapper/mapstorage/oldconnection.cpp \
          ./group/CGroup.cpp \
          ./group/CGroupChar.cpp \
          ./group/CGroupClient.cpp \
          ./group/CGroupCommunicator.cpp \
          ./group/CGroupServer.cpp \
          ./group/CGroupStatus.cpp \
          ./mapper/preferences/groupmanagerpage.cpp
HEADERS += ./mainwindow/MainWindow.h \
           ./mainwindow/ClientManager.h \
           ./mainwindow/MapperManager.h \
           ./mainwindow/GroupManager.h \
           ./mainwindow/ActionManager.h \
           ./client/ClientTextEdit.h \
           ./client/ClientLineEdit.h \
           ./client/PowwowWrapper.h \
           ./client/ClientSocket.h \
           ./client/ClientProcess.h \
           ./client/wrapper_tcp.h \
           ./client/wrapper_process.h \
           ./client/wrapper_tty.h \
           ./client/wrapper_cmd.h \
           ./client/wrapper_profile.h \
           ./dialogs/ObjectEditor.h \
           ./dialogs/KeyBinder.h \
           ./dialogs/InternalEditor.h \
           ./dialogs/ConnectDialog.h \
           ./dialogs/ProfileEditDialog.h \
           ./dialogs/ProfileManagerDialog.h \
           ./profile/cprofilemanager.h \
           ./profile/cprofilesettings.h \
           ./powwow/beam.h \
           ./powwow/cmd2.h \
           ./powwow/cmd.h \
           ./powwow/edit.h \
           ./powwow/eval.h \
           ./powwow/list.h \
           ./powwow/log.h \
           ./powwow/map.h \
           ./powwow/ptr.h \
           ./powwow/tcp.h \
           ./powwow/utils.h \
           ./powwow/main.h \
           ./configuration/configuration.h \
           ./configuration/configdialog.h \
           ./configuration/generalpage.h \
           ./configuration/ansicombo.h \
           ./configuration/colorfontpage.h \
           ./mapper/proxy/telnetfilter.h \
           ./mapper/parser/patterns.h \
           ./mapper/global/defs.h \
          ./mapper/mapdata/roomselection.h \
          ./mapper/mapdata/mapdata.h \
          ./mapper/mapdata/mmapper2room.h \
          ./mapper/mapdata/mmapper2exit.h \
          ./mapper/mapdata/infomark.h \
          ./mapper/mapdata/customaction.h \
          ./mapper/mapdata/roomfactory.h \
          ./mapper/mapdata/drawstream.h \
          ./mapper/proxy/proxy.h \
          ./mapper/proxy/connectionlistener.h \
          ./mapper/mainwindow/roomeditattrdlg.h \
          ./mapper/mainwindow/infomarkseditdlg.h \
          ./mapper/mainwindow/findroomsdlg.h \
          ./mapper/display/mapcanvas.h \
          ./mapper/display/mapwindow.h \
          ./mapper/display/connectionselection.h \
          ./mapper/display/prespammedpath.h \
          ./mapper/expandoracommon/room.h \
          ./mapper/expandoracommon/frustum.h \
          ./mapper/expandoracommon/component.h \
          ./mapper/expandoracommon/coordinate.h \
          ./mapper/expandoracommon/listcycler.h \
          ./mapper/expandoracommon/abstractroomfactory.h \
          ./mapper/expandoracommon/mmapper2event.h \
          ./mapper/expandoracommon/parseevent.h \
          ./mapper/expandoracommon/property.h \
          ./mapper/expandoracommon/roomadmin.h \
          ./mapper/expandoracommon/roomrecipient.h \
          ./mapper/expandoracommon/exit.h \
          ./mapper/pathmachine/approved.h \
          ./mapper/pathmachine/experimenting.h \
          ./mapper/pathmachine/pathmachine.h \
          ./mapper/pathmachine/path.h \
          ./mapper/pathmachine/pathparameters.h \
          ./mapper/pathmachine/mmapper2pathmachine.h \
          ./mapper/pathmachine/roomsignalhandler.h \
          ./mapper/pathmachine/onebyone.h \
          ./mapper/pathmachine/crossover.h \
          ./mapper/pathmachine/syncing.h \
          ./mapper/mapfrontend/intermediatenode.h \
          ./mapper/mapfrontend/mmappermap.h \
          ./mapper/mapfrontend/mapaction.h \
          ./mapper/mapfrontend/mapfrontend.h \
          ./mapper/mapfrontend/roomcollection.h \
          ./mapper/mapfrontend/roomlocker.h \
          ./mapper/mapfrontend/roomoutstream.h \
          ./mapper/mapfrontend/searchtreenode.h \
          ./mapper/mapstorage/abstractmapstorage.h \
          ./mapper/mapstorage/mapstorage.h \
          ./mapper/mapstorage/oldroom.h \
          ./mapper/mapstorage/olddoor.h \
          ./mapper/mapstorage/roomsaver.h \
          ./mapper/mapstorage/oldconnection.h \
          ./mapper/mapfrontend/tinylist.h \
          ./mapper/parser/parser.h \
          ./mapper/parser/mumexmlparser.h \
          ./mapper/parser/abstractparser.h \
          ./mapper/parser/roompropertysetter.h \
          ./group/CGroup.h \
          ./group/CGroupChar.h \
          ./group/CGroupClient.h \
          ./group/CGroupCommunicator.h \
          ./group/CGroupServer.h \
          ./group/CGroupStatus.h \
          ./mapper/preferences/groupmanagerpage.h
TEMPLATE = app
RC_FILE = ./resources/mclient.rc
FORMS += ./dialogs/ObjectEditor.ui \
         ./configuration/generalpage.ui \
         ./configuration/colorfontpage.ui \
         ./dialogs/KeyBinder.ui \
         ./dialogs/ConnectDialog.ui \
         ./dialogs/ProfileManagerDialog.ui \
         ./dialogs/ProfileEditDialog.ui \
         ./mapper/preferences/parserpage.ui \
         ./mapper/preferences/pathmachinepage.ui \
         ./mapper/mainwindow/roomeditattrdlg.ui \
         ./mapper/mainwindow/infomarkseditdlg.ui \
         ./mapper/mainwindow/findroomsdlg.ui \
         ./mapper/preferences/groupmanagerpage.ui
QT += network gui xml opengl
CONFIG += warn_on \
          thread \
          qt \
          network \
          xml \
          opengl
CONFIG -= release
CONFIG += debug
TARGET = mclient
INCLUDEPATH += . ./mainwindow ./client ./powwow ./win32 ./configuration ./profile ./dialogs ./group ./mapper/global ./mapper/mapstorage \
./mapper/mapdata ./mapper/proxy ./mapper/parser ./mapper/preferences ./mapper/configuration ./mapper/display ./mapper/mainwindow \
./mapper/expandoracommon ./mapper/pathmachine ./mapper/mapfrontend
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

