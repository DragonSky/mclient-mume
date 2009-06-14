#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QThread>

#include <QHash>
#include <QMultiHash>
#include <QPointer>

class PluginConfigWidget;
class PluginEntry;

class QApplication;
class QEvent;
class QPluginLoader;
class QString;
class QWidget;

class CommandManager;
class ConfigManager;
class MainWindow;

class PluginManager : public QThread {
    Q_OBJECT

    public:
        static PluginManager* instance();
        ~PluginManager();

	MainWindow* getMainWindow() { return _mainWindow; }
	CommandManager* getCommandManager() { return _commandManager; }
	ConfigManager* getConfigManager() { return _configManager; }

        void customEvent(QEvent* e);
        void configure();

        void initSession(const QString &s);
        void stopSession(const QString &s);
    
    protected:
        void run();

        PluginManager(QObject *parent=0);

        static PluginManager* _pinstance;

    private:
        void loadAllPlugins();
        const bool loadPlugin(const QString& libName);
        const QPluginLoader* pluginWithAPI(const QString& api) const;

        const bool indexPlugins();
        const bool writePluginIndex();
        const bool readPluginIndex();

	MainWindow *_mainWindow;
	CommandManager *_commandManager;
	ConfigManager *_configManager;
        
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
	QMultiHash<QString, QPluginLoader*> _pluginTypes;

        // In what directory are plugins located?  ConfigManager knows.
        QString _pluginDir;

        // The plugin index
        QString _pluginIndex;

        QPointer<PluginConfigWidget> _configWidget;

    signals:
        void doneLoading();

};


#endif // #PLUGINMANAGER_H
