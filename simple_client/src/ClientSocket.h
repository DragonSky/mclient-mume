#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>

#include<QNetworkProxy>


class ClientSocket : public QTcpSocket {
    Q_OBJECT
        
    public:
        ClientSocket(QObject* parent=0);
        ~ClientSocket();

    protected:
        QNetworkProxy _proxy;

};


#endif /* CLIENTSOCKET_H */
