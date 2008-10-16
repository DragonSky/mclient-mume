#include "ClientWidget.h"

#include "ClientLineEdit.h"
#include "ClientTextEdit.h"
#include "PowwowWrapper.h"

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
   
    PowwowWrapper& wrapper = PowwowWrapper::Instance();
    connect(&wrapper, SIGNAL(dataReceived(const QString&)), _textEdit, 
            SLOT(displayText(const QString&)));
    connect(_lineEdit, SIGNAL(returnPressed()), this, 
            SLOT(sendUserInput()));
    
    setMinimumSize(640, 480);
}


void ClientWidget::connectToHost(const QString& host, 
        const qint64& port) const {
    PowwowWrapper& wrapper = PowwowWrapper::Instance();
    wrapper.connectToHost(host, port);
}




ClientWidget::~ClientWidget() {
}


void ClientWidget::sendUserInput() {
    _lineEdit->selectAll();
    PowwowWrapper::Instance().getUserInput(_lineEdit->selectedText());
}

