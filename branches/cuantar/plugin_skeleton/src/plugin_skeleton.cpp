#include "MClientEvent.h"
#include "MClientEventData.h"
#include "PluginManager.h"

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

    //while(!pm->doneLoading()) ; // sleep(.1);
    sleep(.5);
    pm->configureTest();

    pm->initSession("test");
    
    int retval = app.exec();
    pm->destroy();
    return retval;
}
