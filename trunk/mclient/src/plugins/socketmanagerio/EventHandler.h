#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "MClientEventHandler.h"
#include <QPointer>

class QEvent;
class SocketReader;

class EventHandler : public MClientEventHandler {
    Q_OBJECT
    
    public:
        EventHandler(PluginSession *ps, MClientPlugin *mp);
        ~EventHandler();

        void customEvent(QEvent* e);

	const MenuData& createMenus();

public slots:
        void socketReadData(const QByteArray&);
	void displayMessage(const QString&);
	void socketOpened();
	void socketClosed();

        // Menu/Action members
        void connectDevice();
        void disconnectDevice();

 signals:
	void connectToHost();
        void closeSocket();
        void sendToSocket(const QByteArray &);

 private:
	bool _openSocket;
        QPointer<SocketReader> _socketReader;
	
        void sendData(const QByteArray&);

};


#endif /* EVENTHANDLER_H */