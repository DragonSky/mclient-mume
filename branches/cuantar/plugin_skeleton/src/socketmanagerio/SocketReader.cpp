#include "SocketReader.h"

#include "SocketManagerIO.h"

#include "MClientEvent.h"
#include "PluginManager.h"
#include <QVariant>

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QTcpSocket>


SocketReader::SocketReader(QString id, QObject* parent) : QThread(parent) { 
   
    _id = id;
    _sm = qobject_cast<SocketManagerIO*>(parent);
    if(!_sm) qWarning() << "you can't set something else as a socketreader parent!";

    _socket = new QTcpSocket();
    _proxy.setType(QNetworkProxy::NoProxy);
    //_proxy.setType(QNetworkProxy::Socks5Proxy);
    //_proxy.setHostName("proxy.example.com");
    //_proxy.setPort(1080);
    //_proxy.setUser("username");
    //_proxy.setPassword("password");
    //QNetworkProxy::setApplicationProxy(proxy)
    _socket->setProxy(_proxy);
    

    _delete = 0;

    connect(_socket, SIGNAL(connected()), this, SLOT(on_connect())); 
    connect(_socket, SIGNAL(disconnected()), this, SLOT(on_disconnect())); 
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), 
            this, SLOT(on_error())); 
    connect(_socket, SIGNAL(readyRead()), this, SLOT(on_readyRead())); 

}


void SocketReader::connectToHost(const QString host, const int& port) {
    _host = host;
    _port = port;
    _socket->connectToHost(_host, _port);
}


SocketReader::~SocketReader() {
    delete _socket;
}


void SocketReader::on_connect() {
  qDebug() << "\t!! Socket CONNECTED";

    QVariant* qv = new QVariant();
    QStringList tags;
    tags << "SocketConnected";
    MClientEvent* me;
    me = new MClientEvent(new MClientEventData(qv), tags);
    me->session(_id);
    QApplication::postEvent(PluginManager::instance(), me);    
}


void SocketReader::on_readyRead() {
    if(!isRunning()) start(LowPriority);
}


void SocketReader::on_disconnect() {
  qDebug() << "\t!! Socket DISCONNECTED";

    QVariant* qv = new QVariant();
    QStringList tags;
    tags << "SocketDisconnected";
    MClientEvent* me;
    me = new MClientEvent(new MClientEventData(qv), tags);
    me->session(_id);
    QApplication::postEvent(PluginManager::instance(), me);

    if(_delete == 0) {
        _delete = 1;
        deleteLater();
    }
}

void SocketReader::on_error() {

//    qWarning() << "Error involving" 
//       << _host << _port << _socket->errorString();
    if(_delete == 0) {
        _delete = 1;
        deleteLater();
    }
}


const int& SocketReader::port() const {
    return _port;
}


void SocketReader::closeSocket() const {
    _socket->close();
}


void SocketReader::run() {
    
    qDebug() << "SocketReader" << _id << "reading data!";
    // Just read everything there is to be read into a QByteArray.
    QByteArray ba = _socket->readAll(); 
    _sm->socketReadData(ba, _id);

}


void SocketReader::sendToSocket(QByteArray ba) {
    qDebug() << "Got user input:" << ba << ba.count();//str << str.size();
    //int len = _socket->write(ba.toLatin1().data(), str.size());
    int len = _socket->write(ba);
    _socket->write("\n");
    qDebug() << len << "bytes written";
}
