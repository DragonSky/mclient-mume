#include "ConfigManager.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QtXml>


ConfigManager* ConfigManager::_pinstance = 0;

ConfigManager* ConfigManager::instance() {
    if(!_pinstance) {
        _pinstance = new ConfigManager();
    }

    return _pinstance;
}


void ConfigManager::destroy() {
    delete this;
}


ConfigManager::ConfigManager(QObject* parent) : QObject(parent) {
    readApplicationSettings();
}


ConfigManager::~ConfigManager() {
}


const bool ConfigManager::readApplicationSettings() {
    QSettings s;
    _configPath = s.value("general/config_path", "./config").toString();
    return true;
}


const bool ConfigManager::writeApplicationSettings() const {
    QSettings s;
    s.setValue("general/config_path", _configPath);
    return true;
}


const bool ConfigManager::readPluginSettings() {
    QDir d(_configPath);
    QStringList filters;
    filters << "*.xml";
    d.setNameFilters(filters);
    foreach(QString fn, d.entryList(QDir::Files)) {
        qDebug() << "* examining config file" << fn;
        QIODevice* device = new QFile(_configPath+"/"+fn);
        if(!device->open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file for reading!" << fn;
            continue;
        }

        QXmlStreamReader* xml = new QXmlStreamReader(device);

    }
    return true;
}


const bool ConfigManager::writePluginSettings() const {
    return true;
}
