#include "MClientEvent.h"
#include "MClientEventData.h"
#include "PluginManager.h"
#include "SocketData.h"

#include <QApplication>
#include <QByteArray>
#include <QDebug>


int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QApplication::setOrganizationName("MUME");
    QApplication::setOrganizationDomain("mume.org");
    QApplication::setApplicationName("mclient");

    
    PluginManager* pm = PluginManager::instance();
    pm->start(QThread::LowPriority);
    //pm->loadAllPlugins();

    /*
    SocketData* sd; 
    SocketData* sd2; 
    MClientEvent* me;
    MClientEvent* me2; 
    
    QStringList sl;
    sl << "SocketData";
    sd = new SocketData(QByteArray("yar"));
    me = new MClientEvent(new MClientEventData(sd), sl);
    me->session("Session1");
    QApplication::postEvent(pm, me);

    QStringList sl2;
    sl2 << "NotASocketData";
    sd2 = new SocketData(QByteArray("yar2"));
    me2 = new MClientEvent(new MClientEventData(sd2), sl2);
    me->session("Session1");
    QApplication::postEvent(pm, me2);
    */

    while(!pm->doneLoading()) sleep(.1);
    pm->configureTest();
    //pm->configureTest(); // testing whether I can save things

    pm->initDisplays();
    
    return app.exec();
}
