#include "ClientSocket.h"

#include <QNetworkProxy>


ClientSocket::ClientSocket(QObject* parent) : QTcpSocket(parent) {
    _proxy.setType(QNetworkProxy::NoProxy);
    //_proxy.setType(QNetworkProxy::Socks5Proxy);
    //_proxy.setHostName("proxy.example.com");
    //_proxy.setPort(1080);
    //_proxy.setUser("username");
    //_proxy.setPassword("password");
    //QNetworkProxy::setApplicationProxy(proxy)
    setProxy(_proxy);
}


ClientSocket::~ClientSocket() {
}
