#include "MClientPlugin.h"

#include "PluginManager.h"
#include "PluginSession.h"
#include "MClientEvent.h"
#include "MClientEngineEvent.h"
#include "MClientEventData.h"

#include <QApplication>
#include <QEvent>
#include <QString>
#include <QStringList>
#include <QVariant>

MClientPlugin::MClientPlugin(QObject* parent) : QThread(parent) {
    _shortName = "mclientplugin";
    _longName = "The Original MClientPlugin";
    _description = "If you see this text, the plugin author did not replace the default description.";
    _configurable = false;
    _configVersion = "none";
}


MClientPlugin::~MClientPlugin() {
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


const QStringList& MClientPlugin::receivesDataTypes() const {
    return _receivesDataTypes;
}


const QStringList& MClientPlugin::deliversDataTypes() const {
    return _deliversDataTypes;
}


const bool MClientPlugin::configurable() const {
    return _configurable;
}


// This is needed in Qt 4.3 because run is pure virtual -- not in 4.4
void MClientPlugin::run() {
}


// Post an event
void MClientPlugin::postSession(QVariant* payload, QStringList tags) {
  MClientEvent* me = new MClientEvent(new MClientEventData(payload), tags,
				      _session);
  
  QApplication::postEvent(_pluginSession, me);
}


void MClientPlugin::postManager(QVariant* payload, QStringList tags, 
			      QString session) {
  MClientEvent* me = new MClientEvent(new MClientEventData(payload), tags,
				      session);
  
  QApplication::postEvent(_pluginSession->getManager(), me);
}


// Receive the PluginSession reference upon load
void MClientPlugin::setPluginSession(PluginSession *ps) {
  _pluginSession = ps;
  _session = ps->session();
}