#ifndef MCLIENTPLUGIN_H
#define MCLIENTPLUGIN_H

#include "MClientPluginInterface.h"
#include <QThread>

#include <QtPlugin>

#include <QHash>
#include <QStringList>

class QString;


class MClientPlugin : public QThread, public MClientPluginInterface {
    Q_OBJECT
    Q_INTERFACES(MClientPluginInterface)
    
    public:
        MClientPlugin(QObject* parent=0);
        ~MClientPlugin();

        // The library filename relative to plugins dir
        const QString& libName() const;

        // The short name of the plugin used in hashes and maps
        const QString& shortName() const;

        // The long name of the plugin displayed in the gui
        const QString& longName() const;

        // The description of the plugin shown to the user
        const QString& description() const;

        // The plugin version string
        const QString& version() const;

        // Returns a QStringList of APIs this plugin implements to be used
        // when checking dependencies.
        const QHash<QString, int> implemented() const;
        
        // Returns a QHash of APIs and versions this plugin requires to be
        // implemented before it can be loaded.
        const QHash<QString, int> dependencies() const;

        // Returns a QStringList of data types it cares about
        const QStringList& dataTypes() const;
        
        // Consider putting this here and leaving it virtual.
        virtual void customEvent(QEvent* e)=0;
        
        // Can this be configured manually?
        const bool configurable() const;

        // If so, we need to implement this
        virtual void configure()=0;

        // Also, all plugins need to have the ability to load settings.
        virtual const bool loadSettings()=0;

        // And also they need to save them.
        virtual const bool saveSettings() const=0;

        virtual void run();

    protected:
        QString _libName;
        QString _shortName;
        QString _longName;
        QString _description;
        QString _version;

        bool _configurable;

        QHash<QString, int> _implemented;
        QHash<QString, int> _dependencies;

        QStringList _dataTypes;
};


#endif /* MCLIENTPLUGIN_H */