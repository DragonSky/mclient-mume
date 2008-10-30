#include "MClientPlugin.h"

#include <QEvent>
#include <QString>


MClientPlugin::MClientPlugin(QObject* parent) : QThread(parent) {
    _shortName = "mclientplugin";
    _longName = "The Original MClientPlugin";
    _description = "If you see this text, the plugin author did not replace the default description.";
    _configurable = false;
}


MClientPlugin::~MClientPlugin() {
}


const QString& MClientPlugin::libName() const {
    return _libName;
}


const QString& MClientPlugin::shortName() const {
    return _shortName;
}


const QString& MClientPlugin::longName() const {
    return _longName;
}


const QString& MClientPlugin::description() const {
    return _description;
}


const QString& MClientPlugin::version() const {
    return _version;
}


const QHash<QString, int> MClientPlugin::implemented() const {
    return _implemented;
}


const QHash<QString, int> MClientPlugin::dependencies() const {
    return _dependencies;
}


const QStringList& MClientPlugin::dataTypes() const {
    return _dataTypes;
}


const bool MClientPlugin::configurable() const {
    return _configurable;
}


// This is needed in Qt 4.3 because run is pure virtual -- not in 4.4
void MClientPlugin::run() {
}

/*
void MClientPlugin::configure() {

}


const bool MClientPlugin::loadSettings() {
    return true;
}

        
const bool MClientPlugin::saveSettings() const {
    return true;
}


void MClientPlugin::customEvent(QEvent* e) {

}
*/
