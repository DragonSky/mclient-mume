#ifndef SOCKETMANAGERIO_H
#define SOCKETMANAGERIO_H

#include "MClientIOPlugin.h"

#include <QDialog>
#include <QHash>

class SocketManagerIOConfig;
class SocketReader;

class QByteArray;
class QEvent;
class QString;


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

        // IO members
        void connectDevice();
        void disconnectDevice();
        void sendData(const QByteArray data);

        void socketReadData(const QByteArray data, const QString socket);


    private:
        QHash<QString, SocketReader*> _sockets;
        QPointer<SocketManagerIOConfig> _configWidget;
    
};


#endif /* SOCKETMANAGERIO_H */
