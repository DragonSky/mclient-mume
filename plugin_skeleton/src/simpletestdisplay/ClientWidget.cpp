#include "ClientWidget.h"

#include "ClientLineEdit.h"
#include "ClientTextEdit.h"
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


ClientWidget::ClientWidget(QString s, SimpleTestDisplay* st, QWidget* parent) 
    : QWidget(parent) {
    _session = s;

    _st = st;
    _layout = new QVBoxLayout(this);
    setLayout(_layout);

    _textEdit = new ClientTextEdit(this);
    _layout->addWidget(_textEdit);
    
    _lineEdit = new ClientLineEdit(this);
    _layout->addWidget(_lineEdit);
   
    // Send data when user presses return
    connect(_lineEdit, SIGNAL(returnPressed()), this, 
            SLOT(sendUserInput()));
    
    // When the STD receives data, display it in the textedit
    connect(_st, SIGNAL(dataReceived(const QString&)), _textEdit, 
            SLOT(displayText(const QString&)));
    
    setMinimumSize(640, 480);
}


ClientWidget::~ClientWidget() {
}


void ClientWidget::sendUserInput() {
    _lineEdit->selectAll();
    if (_lineEdit->selectedText().startsWith("#connect")) {
      QVariant* qv = new QVariant();
      QStringList sl("ConnectToHost");
      MClientEvent* me = new MClientEvent(new MClientEventData(qv),sl);
      QApplication::postEvent(PluginManager::instance(), me);
      
    } else {
      //PowwowWrapper::Instance()->getUserInput(_lineEdit->selectedText());
      QByteArray ba(_lineEdit->selectedText().toStdString().c_str());
      QVariant* qv = new QVariant(ba);
      QStringList sl("SendToSocketData");
      
      MClientEvent* me = new MClientEvent(new MClientEventData(qv),sl);
      QApplication::postEvent(PluginManager::instance(), me);
    }
}
