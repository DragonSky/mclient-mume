#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>

#include <QHash>
#include <QStringList>


class ConfigManager : public QObject {
    Q_OBJECT

    public:
        static ConfigManager* instance();
        void destroy();

        const bool readApplicationSettings();
        const bool writeApplicationSettings() const;

        const bool readPluginSettings();
        const bool writePluginSettings() const;

        const QStringList& profileNames() const;
        const QStringList profilePlugins(const QString profile) const;


    protected:
        ConfigManager(QObject* parent);
        ~ConfigManager();

        static ConfigManager* _pinstance;


    private:
        QString _settingsFile;
        QStringList _profiles;
        
        QHash<QString,
            QHash<QString,
                QHash<QString,
                    QHash<QString, QString>
                >
            >
        > _config;


};


#endif /* CONFIGMANAGER_H */
