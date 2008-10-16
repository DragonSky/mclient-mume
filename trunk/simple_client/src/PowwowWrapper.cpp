#include "PowwowWrapper.h"

#include "ClientSocket.h"

#include <QDebug>
#include <QString>


PowwowWrapper& PowwowWrapper::Instance() {
    static PowwowWrapper pow;
    return pow;
}


PowwowWrapper::PowwowWrapper(QObject* parent) : QObject(parent) {
    _socket = new ClientSocket(this);
    
    connect(_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}


PowwowWrapper::~PowwowWrapper() {
}


void PowwowWrapper::getUserInput(const QString& str) const {
    qDebug() << "Got user input:" << str << str.size();
    int len = _socket->write(str.toStdString().c_str(), str.size());
    qDebug() << len << "bytes written";

}


void PowwowWrapper::connected() { 
    qDebug() << "We're connected now!";
}


void PowwowWrapper::readSocket() {
    QString data = _socket->read(1024);    
    emit dataReceived(data);
}


void PowwowWrapper::connectToHost(const QString& host, 
        const qint64& port) const {
    _socket->connectToHost(host, port);
}
