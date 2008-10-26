#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QThread>

#include <QHash>
#include <QPointer>

class PluginConfigWidget;
class PluginEntry;

class QApplication;
class QEvent;
class QPluginLoader;
class QString;
class QWidget;


class PluginManager : public QThread {
    Q_OBJECT

    public:
        static PluginManager* instance();

        PluginManager(QWidget* display, QObject* io, QObject* filter,
                QObject* parent=0);
        PluginManager(QObject* parent=0);
        ~PluginManager();

        void loadAllPlugins();
        const bool loadPlugin(const QString& libName);
        const QPluginLoader* pluginWithAPI(const QString& api) const;
        void customEvent(QEvent* e);

        void run();

        // for testing
        void configureTest();
        const bool doneLoading() const;
    
    protected:
        // short name -> other info
        QHash<QString, PluginEntry*> _availablePlugins; 
        
        // A hash of the plugin object pointers, short name -> pointer
        // NOTE: this way of indexing plugins gives each one a unique slot
        QHash<QString, QPluginLoader*> _loadedPlugins;
        
        // A hash of the plugin object pointers, api -> pointer
        // NOTE: some plugins may be repeated here.
        QHash<QString, QPluginLoader*> _pluginAPIs;
        
        // A hash of the plugin object pointers, data type -> pointer
        // NOTE: this is used to keep track of what data types each plugin
        // wants for easy event handling.  This one also may have multiple
        // entries for a given plugin.
        QHash<QString, QPluginLoader*> _pluginTypes;

        // Here are the parents
        QWidget* _displayParent;
        QObject* _ioParent;
        QObject* _filterParent;

        // In what directory are plugins located?  ConfigManager knows.
        QString _pluginDir;

        QPointer<PluginConfigWidget> _configWidget;

    private:
        static PluginManager* _pinstance;
        
        // for testing
        bool _doneLoading;

};


#endif // #PLUGINMANAGER_H
