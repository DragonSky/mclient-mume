SOURCES += ./main.cpp \
           ./mainwindow/mainwindow.cpp \
           ./mainwindow/textview.cpp \
           ./wrapper/wrapper.cpp \
           ./wrapper/wrapper_tcp.cpp \
           ./wrapper/wrapper_process.cpp \
           ./wrapper/wrapper_tty.cpp \
           ./wrapper/WrapperSocket.cpp \
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
           ./objecteditor/objecteditor.cpp \
           ./configuration/configuration.cpp \
           ./configuration/configdialog.cpp \
           ./configuration/generalpage.cpp \
           ./configuration/ansicombo.cpp \
           ./configuration/colorfontpage.cpp \
           ./mainwindow/keybinder.cpp \
           ./profilemanager/profiledialog.cpp \
           ./profilemanager/profilemanagerdialog.cpp \
           ./profilemanager/profileeditdialog.cpp \
           ./profilemanager/cprofilemanager.cpp \
           ./profilemanager/cprofilesettings.cpp \
           ./wrapper/wrapper_profile.cpp \
           ./mainwindow/internaleditor.cpp \
           ./proxy/telnetfilter.cpp \
           ./parser/patterns.cpp \
           ./mmapper2/mapdata/mapdata.cpp \
          ./mmapper2/mapdata/mmapper2room.cpp \
          ./mmapper2/mapdata/mmapper2exit.cpp \
          ./mmapper2/mapdata/roomfactory.cpp \
          ./mmapper2/mapdata/drawstream.cpp \
          ./mmapper2/mapdata/roomselection.cpp \
          ./mmapper2/mapdata/customaction.cpp \
          ./mmapper2/mainwindow/roomeditattrdlg.cpp \
          ./mmapper2/mainwindow/infomarkseditdlg.cpp \
          ./mmapper2/mainwindow/findroomsdlg.cpp \
          ./mmapper2/display/mapwindow.cpp \
          ./mmapper2/display/mapcanvas.cpp \
          ./mmapper2/display/connectionselection.cpp \
          ./mmapper2/display/prespammedpath.cpp \
          ./mmapper2/proxy/connectionlistener.cpp \
          ./mmapper2/proxy/proxy.cpp \
          ./mmapper2/parser/parser.cpp \
          ./mmapper2/parser/mumexmlparser.cpp \
          ./mmapper2/parser/abstractparser.cpp \
          ./mmapper2/parser/roompropertysetter.cpp \
          ./mmapper2/expandoracommon/component.cpp \
          ./mmapper2/expandoracommon/coordinate.cpp \
          ./mmapper2/expandoracommon/frustum.cpp \
          ./mmapper2/expandoracommon/abstractroomfactory.cpp \
          ./mmapper2/expandoracommon/init.cpp \
          ./mmapper2/expandoracommon/property.cpp \
          ./mmapper2/expandoracommon/mmapper2event.cpp \
          ./mmapper2/expandoracommon/parseevent.cpp \
          ./mmapper2/mapfrontend/intermediatenode.cpp \
          ./mmapper2/mapfrontend/mmappermap.cpp \
          ./mmapper2/mapfrontend/mapaction.cpp \
          ./mmapper2/mapfrontend/mapfrontend.cpp \
          ./mmapper2/mapfrontend/roomcollection.cpp \
          ./mmapper2/mapfrontend/roomlocker.cpp \
          ./mmapper2/mapfrontend/searchtreenode.cpp \
          ./mmapper2/pathmachine/approved.cpp \
          ./mmapper2/pathmachine/experimenting.cpp \
          ./mmapper2/pathmachine/pathmachine.cpp \
          ./mmapper2/pathmachine/mmapper2pathmachine.cpp \
          ./mmapper2/pathmachine/roomsignalhandler.cpp \
          ./mmapper2/pathmachine/path.cpp \
          ./mmapper2/pathmachine/pathparameters.cpp \
          ./mmapper2/pathmachine/syncing.cpp \
          ./mmapper2/pathmachine/onebyone.cpp \
          ./mmapper2/pathmachine/crossover.cpp \
          ./mmapper2/mapstorage/roomsaver.cpp \
          ./mmapper2/mapstorage/abstractmapstorage.cpp \
          ./mmapper2/mapstorage/mapstorage.cpp \
          ./mmapper2/mapstorage/oldconnection.cpp \
          ./mmapper2/pandoragroup/CGroup.cpp \
          ./mmapper2/pandoragroup/CGroupChar.cpp \
          ./mmapper2/pandoragroup/CGroupClient.cpp \
          ./mmapper2/pandoragroup/CGroupCommunicator.cpp \
          ./mmapper2/pandoragroup/CGroupServer.cpp \
          ./mmapper2/pandoragroup/CGroupStatus.cpp \
          ./mmapper2/preferences/groupmanagerpage.cpp \
 mainwindow/clientmanager.cpp \
 mainwindow/mappermanager.cpp \
 mainwindow/groupmanager.cpp \
 mainwindow/actionmanager.cpp
HEADERS += ./mainwindow/mainwindow.h \
           ./mainwindow/textview.h \
           ./wrapper/wrapper.h \
           ./wrapper/wrapper_tty.h \
           ./wrapper/wrapper_tcp.h \
           ./wrapper/wrapper_process.h \
           ./wrapper/WrapperSocket.h \
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
           ./objecteditor/objecteditor.h \
           ./configuration/configuration.h \
           ./configuration/configdialog.h \
           ./configuration/generalpage.h \
           ./configuration/ansicombo.h \
           ./configuration/colorfontpage.h \
           ./mainwindow/keybinder.h \
           ./profilemanager/profiledialog.h \
           ./profilemanager/profilemanagerdialog.h \
           ./profilemanager/profileeditdialog.h \
           ./profilemanager/cprofilemanager.cpp \
           ./profilemanager/cprofilesettings.cpp \
           ./wrapper/wrapper_profile.h \
           ./mainwindow/internaleditor.h \
           ./proxy/telnetfilter.h \
           ./parser/patterns.h \
           ./mmapper2/global/defs.h \
          ./mmapper2/mapdata/roomselection.h \
          ./mmapper2/mapdata/mapdata.h \
          ./mmapper2/mapdata/mmapper2room.h \
          ./mmapper2/mapdata/mmapper2exit.h \
          ./mmapper2/mapdata/infomark.h \
          ./mmapper2/mapdata/customaction.h \
          ./mmapper2/mapdata/roomfactory.h \
          ./mmapper2/mapdata/drawstream.h \
          ./mmapper2/proxy/proxy.h \
          ./mmapper2/proxy/connectionlistener.h \
          ./mmapper2/mainwindow/roomeditattrdlg.h \
          ./mmapper2/mainwindow/infomarkseditdlg.h \
          ./mmapper2/mainwindow/findroomsdlg.h \
          ./mmapper2/display/mapcanvas.h \
          ./mmapper2/display/mapwindow.h \
          ./mmapper2/display/connectionselection.h \
          ./mmapper2/display/prespammedpath.h \
          ./mmapper2/expandoracommon/room.h \
          ./mmapper2/expandoracommon/frustum.h \
          ./mmapper2/expandoracommon/component.h \
          ./mmapper2/expandoracommon/coordinate.h \
          ./mmapper2/expandoracommon/listcycler.h \
          ./mmapper2/expandoracommon/abstractroomfactory.h \
          ./mmapper2/expandoracommon/mmapper2event.h \
          ./mmapper2/expandoracommon/parseevent.h \
          ./mmapper2/expandoracommon/property.h \
          ./mmapper2/expandoracommon/roomadmin.h \
          ./mmapper2/expandoracommon/roomrecipient.h \
          ./mmapper2/expandoracommon/exit.h \
          ./mmapper2/pathmachine/approved.h \
          ./mmapper2/pathmachine/experimenting.h \
          ./mmapper2/pathmachine/pathmachine.h \
          ./mmapper2/pathmachine/path.h \
          ./mmapper2/pathmachine/pathparameters.h \
          ./mmapper2/pathmachine/mmapper2pathmachine.h \
          ./mmapper2/pathmachine/roomsignalhandler.h \
          ./mmapper2/pathmachine/onebyone.h \
          ./mmapper2/pathmachine/crossover.h \
          ./mmapper2/pathmachine/syncing.h \
          ./mmapper2/mapfrontend/intermediatenode.h \
          ./mmapper2/mapfrontend/mmappermap.h \
          ./mmapper2/mapfrontend/mapaction.h \
          ./mmapper2/mapfrontend/mapfrontend.h \
          ./mmapper2/mapfrontend/roomcollection.h \
          ./mmapper2/mapfrontend/roomlocker.h \
          ./mmapper2/mapfrontend/roomoutstream.h \
          ./mmapper2/mapfrontend/searchtreenode.h \
          ./mmapper2/mapstorage/abstractmapstorage.h \
          ./mmapper2/mapstorage/mapstorage.h \
          ./mmapper2/mapstorage/oldroom.h \
          ./mmapper2/mapstorage/olddoor.h \
          ./mmapper2/mapstorage/roomsaver.h \
          ./mmapper2/mapstorage/oldconnection.h \
          ./mmapper2/mapfrontend/tinylist.h \
          ./mmapper2/parser/parser.h \
          ./mmapper2/parser/mumexmlparser.h \
          ./mmapper2/parser/abstractparser.h \
          ./mmapper2/parser/roompropertysetter.h \
          ./mmapper2/pandoragroup/CGroup.h \
          ./mmapper2/pandoragroup/CGroupChar.h \
          ./mmapper2/pandoragroup/CGroupClient.h \
          ./mmapper2/pandoragroup/CGroupCommunicator.h \
          ./mmapper2/pandoragroup/CGroupServer.h \
          ./mmapper2/pandoragroup/CGroupStatus.h \
          ./mmapper2/preferences/groupmanagerpage.h \
 mainwindow/clientmanager.h \
 mainwindow/mappermanager.h \
 mainwindow/groupmanager.h \
 mainwindow/actionmanager.h
TEMPLATE = app
RC_FILE = ./resources/mclient.rc
FORMS += ./objecteditor/objecteditor.ui \
         ./configuration/generalpage.ui \
         ./configuration/colorfontpage.ui \
         ./mainwindow/keybinder.ui \
         ./profilemanager/profiledialog.ui \
         ./profilemanager/profilemanagerdialog.ui \
         ./profilemanager/profileeditdialog.ui \
         ./mmapper2/preferences/generalpage.ui \
         ./mmapper2/preferences/parserpage.ui \
         ./mmapper2/preferences/pathmachinepage.ui \
         ./mmapper2/mainwindow/roomeditattrdlg.ui \
         ./mmapper2/mainwindow/infomarkseditdlg.ui \
         ./mmapper2/mainwindow/findroomsdlg.ui \
         ./mmapper2/preferences/groupmanagerpage.ui
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
INCLUDEPATH += . ./mainwindow ./powwow ./win32 ./wrapper ./configuration ./profilemanager ./objecteditor ./proxy ./parser \
./mmapper2/global ./mmapper2/mapstorage ./mmapper2/mapdata ./mmapper2/proxy ./mmapper2/parser ./mmapper2/preferences ./mmapper2/configuration \
./mmapper2/display ./mmapper2/mainwindow ./mmapper2/expandoracommon ./mmapper2/pathmachine ./mmapper2/mapfrontend ./mmapper2/pandoragroup
macx : LIBS += 
win32 : LIBS += -llibgnurx
unix : LIBS += -lm -W -Wall -O -pedantic
DEPENDPATH += .
DEFINES += USE_REGEXP=1 MCLIENT=1 MMAPPER=1 VERSION=\\\"1.2.13\\\"
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

