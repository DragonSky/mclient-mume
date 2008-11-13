#ifndef SOCKETMANAGERIO_H
#define SOCKETMANAGERIO_H

#include "MClientIOPlugin.h"

#include <QDialog>
#include <QHash>
#include <QMultiHash>
#include <QPair>

class SocketManagerIOConfig;
class SocketReader;

class QByteArray;
class QEvent;
class QString;
class QVariant;


class SocketManagerIO : public MClientIOPlugin {
    Q_OBJECT

    public:
        SocketManagerIO(QObject* parent=0);
        ~SocketManagerIO();

        // Plugin members
        void customEvent(QEvent* e);
        void configure();
        const bool loadSettings();
        const bool saveSettings() const;
        const bool startSession(QString s);
        const bool stopSession(QString s);

        // IO members
        void connectDevice(QString s);
        void disconnectDevice(QString s);
        void sendData(const QByteArray data);

        void socketReadData(const QByteArray data, const QString socket);


    private:
        QHash<QString, SocketReader*> _sockets;
        QPointer<SocketManagerIOConfig> _configWidget;
        QMultiHash<QString, QPair<QString, QVariant> > _settings;
    
};


#endif /* SOCKETMANAGERIO_H */
