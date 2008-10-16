#include "ClientWidget.h"

#include "ClientLineEdit.h"

#include <QDebug>
#include <QString>
#include <QTcpSocket>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>
#include <QVBoxLayout>


ClientWidget::ClientWidget(QWidget* parent) : QWidget(parent) {
    _layout = new QVBoxLayout(this);
    setLayout(_layout);

    _textEdit = new QTextEdit(this);
    _layout->addWidget(_textEdit);
    _doc = new QTextDocument(_textEdit);
    _textEdit->setDocument(_doc);
    
    _lineEdit = new ClientLineEdit(this);
    _layout->addWidget(_lineEdit);
    _cursor = new QTextCursor(_doc);
    
    _socket = new QTcpSocket(this);

    connect(_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(this, SIGNAL(dataReceived(const QString&)), this, 
            SLOT(displayText(const QString&)));

    setMinimumSize(640, 480);
}


void ClientWidget::connectToHost() const {
    _socket->connectToHost("mume.org", 4242);
}


void ClientWidget::connected() { 
    qDebug() << "we're connected now";
}


void ClientWidget::readSocket() {
    QString data;
    data = _socket->read(1024);    
    emit dataReceived(data);
}


ClientWidget::~ClientWidget() {
    _socket->disconnect(); 
}


void ClientWidget::displayText(const QString& str) {
    _doc->setPlainText(str);
}
