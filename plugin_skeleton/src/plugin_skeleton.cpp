#include "MClientEvent.h"
#include "MClientEventData.h"
#include "ConfigManager.h"
#include "PluginManager.h"

#include <QApplication>
#include <QByteArray>
#include <QDebug>


int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QApplication::setOrganizationName("MUME");
    QApplication::setOrganizationDomain("mume.org");
    QApplication::setApplicationName("mclient");

    ConfigManager* cm = ConfigManager::instance();
    
    PluginManager* pm = PluginManager::instance();
    pm->start(QThread::TimeCriticalPriority);

//    while(!pm->doneLoading()) sleep(.1);
    sleep(1);
    pm->configureTest();

    pm->initSession("test");
    
    int retval = app.exec();

    pm->destroy();
    cm->destroy();

    return retval;
}
