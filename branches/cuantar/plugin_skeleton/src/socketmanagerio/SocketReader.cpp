#include "SocketReader.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QTcpSocket>

#include <algorithm>
using std::copy;
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "netinet/in.h"


SocketReader::SocketReader(QObject* parent) : QThread(parent) { 
    

    _socket = new QTcpSocket();
    _socket->setReadBufferSize(0);
    _counter = 0;
    _delete = 0;

    connect(_socket, SIGNAL(connected()), this, SLOT(on_connect())); 
    connect(_socket, SIGNAL(disconnected()), this, SLOT(on_disconnect())); 
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), 
            this, SLOT(on_error())); 
    connect(_socket, SIGNAL(readyRead()), this, SLOT(on_readyRead())); 

}


void SocketReader::connectToHost(const QString& host, const int& port) {
    _host = host;
    _port = port;
    _socket->connectToHost(_host, _port);
}


SocketReader::~SocketReader() {
    delete _socket;
}


void SocketReader::on_connect() {

}


void SocketReader::on_readyRead() {
    
}


void SocketReader::on_disconnect() {
//    Logger(1) << "socket on port" << _port << "has disconnected" << endl;
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
//    Logger(1) << "SocketReader::closeSocket()" << endl;
    _socket->close();
}

void SocketReader::run() {

}
