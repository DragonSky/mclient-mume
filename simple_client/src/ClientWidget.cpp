#include "ClientWidget.h"

#include "ClientLineEdit.h"
#include "ClientTextEdit.h"

#include <QDebug>
#include <QString>
#include <QTcpSocket>
#include <QVBoxLayout>


ClientWidget::ClientWidget(QWidget* parent) : QWidget(parent) {
    _layout = new QVBoxLayout(this);
    setLayout(_layout);

    _textEdit = new ClientTextEdit(this);
    _layout->addWidget(_textEdit);
    
    _lineEdit = new ClientLineEdit(this);
    _layout->addWidget(_lineEdit);
    
    _socket = new QTcpSocket(this);

    connect(_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(this, SIGNAL(dataReceived(const QString&)), _textEdit, 
            SLOT(displayText(const QString&)));

    setMinimumSize(640, 480);
}


void ClientWidget::connectToHost(const QString& host, 
        const qint64& port) const {
    _socket->connectToHost(host, port);
}


void ClientWidget::connected() { 
    qDebug() << "We're connected now!";
}


void ClientWidget::readSocket() {
    QString data = _socket->read(1024);    
    emit dataReceived(data);
}


ClientWidget::~ClientWidget() {
    _socket->disconnect(); 
}
