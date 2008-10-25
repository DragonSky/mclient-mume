#include "MClientEvent.h"
#include "MClientEventData.h"
#include "PluginManager.h"
#include "SocketData.h"

#include <QApplication>
#include <QByteArray>
#include <QDebug>


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    
    PluginManager* pm = PluginManager::instance();
    pm->start(QThread::LowPriority);
    //pm->loadAllPlugins();

    SocketData* sd; 
    SocketData* sd2; 
    MClientEvent* me;
    MClientEvent* me2; 
    
    sd = new SocketData(QByteArray("yar"));
    me = new MClientEvent(new MClientEventData(sd), "SocketDataEvent");
    QApplication::postEvent(pm, me);

    /*
    sd2 = new SocketData(QByteArray("yar2"));
    me2 = new MClientEvent(new MClientEventData(sd2), "NotASocketDataEvent");
    QApplication::postEvent(pm, me2);
    */

    while(!pm->doneLoading()) sleep(.1);
    pm->configureTest();
    pm->configureTest(); // testing whether I can save things
    
    return app.exec();
}
