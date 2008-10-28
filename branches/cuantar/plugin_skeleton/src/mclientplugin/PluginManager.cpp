#include "PluginManager.h"

#include "MClientDisplayInterface.h"
#include "MClientPluginInterface.h"
#include "MClientEvent.h"
#include "PluginConfigWidget.h"
#include "PluginEntry.h"

#include <QApplication>
#include <QDebug>
#include <QHash>
#include <QLibrary>
#include <QPluginLoader>
#include <QString>
#include <QWidget>


PluginManager* PluginManager::_pinstance = 0;

PluginManager* PluginManager::instance() {
    if(!_pinstance) {
        _pinstance = new PluginManager();
    }
    
    return _pinstance;

}


PluginManager::PluginManager(QWidget* display, QObject* io, QObject* filter, 
        QObject* parent) : QThread(parent) {
    _displayParent = display;
    _ioParent = io;
    _filterParent = filter;

    _pluginDir = "./plugins";
}


PluginManager::PluginManager(QObject* parent) : QThread(parent) {
    _pluginDir = "./plugins";

    // Testing
    PluginEntry* e = new PluginEntry();
    e->shortName("socketdatafilter");
    e->longName("Socket Data Filter");
    e->libName("libsocketdatafilter.so");
    e->addAPI("terrible_test_api", 1);
    qDebug() << e->libName();
    _availablePlugins.insert(e->libName(), e);
    
    e = new PluginEntry();
    e->shortName("socketmanagerio");
    e->longName("SocketManager");
    e->libName("libsocketmanagerio.so");
    e->addAPI("some_stupid_api", 10);
    qDebug() << e->libName();
    _availablePlugins.insert(e->libName(), e);
    
    e = new PluginEntry();
    e->shortName("simpletestdisplay");
    e->longName("Simple Test Display");
    e->libName("libsimpletestdisplay.so");
    qDebug() << e->libName();
    _availablePlugins.insert(e->libName(), e);

    _doneLoading = false;
}


PluginManager::~PluginManager() {

    // Remove entries in the available plugins list
    PluginEntry* pe;
    foreach(pe, _availablePlugins) {
        delete pe;
    }

    // Unload all plugins
    QPluginLoader* pm;
    foreach(pm, _loadedPlugins) {
        pm->unload();
        delete pm;
    }
}



void PluginManager::loadAllPlugins() {
    
    PluginEntry* pe;
    foreach(pe, _availablePlugins) {
        if(_loadedPlugins.find(pe->shortName()) == _loadedPlugins.end()) {
            bool loader = loadPlugin(pe->libName());
            if(!loader) {
                qDebug() << "Could not load plugin" << pe->shortName();

            } else {
            //qDebug() << "Successfully loaded plugin" << pe->shortName();
            }
        }
    }

    _doneLoading = true;

}


const bool PluginManager::loadPlugin(const QString& libName) {
    qDebug() << "loadPlugin call for" << libName;

    QString fileName = _pluginDir + "/" + libName;

    if(!QLibrary::isLibrary(fileName)) {
        // Error handling
        qWarning() << fileName << "is not a library!";
        return false;
    }
    
    // Try to load plugin from file
    QPluginLoader* loader = new QPluginLoader(fileName);
    // NOTE: can't have 'this' as parent because PluginManager is in a
    // different thread than its own.
    if(!loader->load()) {
        qWarning() << "failed to load plugin from file" << fileName;
        return false;

    } else {
        QObject* plugin = loader->instance();
        
        // Is it the correct kind of plugin?
        if(!qobject_cast<MClientPluginInterface*>(plugin)) {
            qWarning() << "plugin in file" << fileName 
                    << "does not implement MClientPluginInterface!";
            loader->unload();
            delete loader;
            return false;
        } else {
            MClientPluginInterface* iPlugin = 
                    qobject_cast<MClientPluginInterface*>(plugin);

            QHash<QString, int> deps = iPlugin->dependencies();
           
            if(!deps.isEmpty()) {

            // Check dependencies
            QHash<QString, int>::iterator it = deps.begin();
            for(it; it!=deps.end(); ++it) {
                // Ask what APIs each plugin implements and compare versions
                PluginEntry* pe;
                foreach(pe, _availablePlugins) {
                    int version = pe->version(it.key());
                    if(!version) {
                        qDebug() << "Plugin" << pe->shortName() 
                                 << "does not provide an implementation of" 
                                 << it.key();
                    } else {

                        // Found one of the APIs, so check the version
                        // We want the version of this plugin to be <= the
                        // version of the one we're checking against.
                        // Incompatible updates should change the API name.
                        if(version > it.value()) {
                            qDebug() << "version mismatch!";
                            continue;
                        }

                        qDebug() << "Dependency" << it.key() << it.value()
                                 << "satisfied by" << pe->shortName();
                        if(_loadedPlugins.find(pe->shortName()) == 
                                _loadedPlugins.end()) {
                            qDebug() << "attempting to load dep" 
                                << pe->libName();
                            if(loadPlugin(pe->libName())) {
                                qDebug() << "succeeded!";
                    //        deps.erase(it);

                            } else {
                                qWarning() << "could not load deps for" 
                                    << it.key();
                                return false;
                            }
                        }
                    }
                }



            } // deps
            } // if has deps
           
            // Insert shortname for this plugin into hash
            _loadedPlugins.insert(iPlugin->shortName(),loader);

            // insert APIs for this plugin into hash
            QHash<QString, int> apis = iPlugin->implemented();
            QHash<QString, int>::iterator jt = apis.begin();
            for(jt; jt!=apis.end(); ++jt) {
                _pluginAPIs.insert(jt.key(), loader);
            }
           
            // Insert datatypes this plugin wants into hash
            if(!iPlugin->dataTypes().isEmpty()) {
                QString s;
                foreach(s, iPlugin->dataTypes()) {
                    _pluginTypes.insert(s, loader);
                }
            }

            // Each plugin takes care of its own settings
            iPlugin->loadSettings();
            qDebug() << "Successfully loaded plugin" << iPlugin->shortName();
        } // casted
    } // loaded
    
    return true; // never actually gets reached
}


const QPluginLoader* PluginManager::pluginWithAPI(const QString& api) const {

    QPluginLoader* plugin = _pluginAPIs.find(api).value();
    return plugin;
}


void PluginManager::customEvent(QEvent* e) {
    MClientEvent* me = static_cast<MClientEvent*>(e);
  
    QHash<QString, QPluginLoader*>::iterator it = _pluginTypes.begin();
    
    for(it; it != _pluginTypes.end(); ++it) {
        if(me->dataTypes().contains(it.key())) {
            qDebug() << "posting" << me->dataTypes() << "to" 
                << it.value()->instance();
            // Need to make a copy, since the original event
            // will be deleted when this function returns
            MClientEvent* nme = new MClientEvent(*me);
            QApplication::postEvent(it.value()->instance(),nme);
        }
    }
}


// We want to start an event loop in a separate thread to handle plugins
void PluginManager::run() {
    loadAllPlugins();
    emit doneLoading();
    exec();
}


// For testing
const bool PluginManager::doneLoading() const {
    return _doneLoading;
}


void PluginManager::configureTest() { 
    if(!_configWidget) _configWidget = new PluginConfigWidget(_loadedPlugins);
    if(!_configWidget->isVisible()) _configWidget->show();
}


void PluginManager::initDisplays() {
    QPluginLoader* pl;
    foreach(pl,_loadedPlugins) {
        MClientDisplayInterface* di;
        di = qobject_cast<MClientDisplayInterface*>(pl->instance());
        if(di) {
            di->initDisplay();
        }
    }
}
