#include "ClientWidget.h"

#include "ClientLineEdit.h"
#include "ClientTextEdit.h"
#include "PowwowWrapper.h"
#include "SimpleTestDisplay.h"

#include "MClientEvent.h"
#include "MClientEventData.h"
#include "PluginManager.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QTcpSocket>
#include <QVariant>
#include <QVBoxLayout>


ClientWidget::ClientWidget(SimpleTestDisplay* st, QWidget* parent) 
    : QWidget(parent) {
    _st = st;
    _layout = new QVBoxLayout(this);
    setLayout(_layout);

    _textEdit = new ClientTextEdit(this);
    _layout->addWidget(_textEdit);
    
    _lineEdit = new ClientLineEdit(this);
    _layout->addWidget(_lineEdit);
   
    PowwowWrapper* wrapper = PowwowWrapper::Instance();
    // When the wrapper receives data, display it in the textedit
    connect(_st, SIGNAL(dataReceived(const QString&)), _textEdit, 
            SLOT(displayText(const QString&)));
    
    // Send data when user presses return
    connect(_lineEdit, SIGNAL(returnPressed()), this, 
            SLOT(sendUserInput()));
    
    setMinimumSize(640, 480);
}


void ClientWidget::connectToHost(const QString& host, 
        const qint64& port) const {
    PowwowWrapper* wrapper = PowwowWrapper::Instance();
    wrapper->connectToHost(host, port);
}


ClientWidget::~ClientWidget() {
}


void ClientWidget::sendUserInput() {
    _lineEdit->selectAll();
    //PowwowWrapper::Instance()->getUserInput(_lineEdit->selectedText());
    QByteArray ba(_lineEdit->selectedText().toStdString().c_str());
    QVariant* qv = new QVariant(ba);
    QStringList sl("SendToSocketData");

    MClientEvent* me = new MClientEvent(new MClientEventData(qv),sl);
    QApplication::postEvent(PluginManager::instance(), me);
}
