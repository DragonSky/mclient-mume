#include "MainWindow.h"
#include "ConfigManager.h"
#include "PluginManager.h"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QApplication::setOrganizationName("MUME");
    QApplication::setOrganizationDomain("mume.org");
    QApplication::setApplicationName("mclient");

    ConfigManager* cm = ConfigManager::instance();
    PluginManager* pm = PluginManager::instance();
    MainWindow *mw = new MainWindow();
    pm->start(QThread::TimeCriticalPriority);

    return app.exec();
}
