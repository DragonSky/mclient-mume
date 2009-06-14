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

    _textEdit->setFocusProxy(_lineEdit);
    qDebug() << "* SimpleTestDisplay thread:" << _st->thread();
    qDebug() << "* ClientWidget thread:" << this->thread();
    qDebug() << "* ClientTextEdit thread:" << _textEdit->thread();
   
    // Send data when user presses return
    connect(_lineEdit, SIGNAL(returnPressed()), this, 
            SLOT(sendUserInput()));
    
    // When the STD receives data, display it in the textedit
    connect(_st, SIGNAL(dataReceived(const QString&)), _textEdit, 
            SLOT(displayText(const QString&)));
    
    connect(this, SIGNAL(connectToHost()), _st, SLOT(connectToHost()));
    connect(this, SIGNAL(sendToSocketData(const QByteArray&)), _st, SLOT(sendToSocketData(const QByteArray&)));

    setMinimumSize(640, 480);
}


ClientWidget::~ClientWidget() {
}


void ClientWidget::sendUserInput() {
    _lineEdit->selectAll();
    _textEdit->displayText(_lineEdit->selectedText().append("\n"));
    if (_lineEdit->selectedText().startsWith("#connect")) {
      emit connectToHost();

    } else {
      //PowwowWrapper::Instance()->getUserInput(_lineEdit->selectedText());

      QString str = _lineEdit->selectedText();
      QByteArray ba(str.toLatin1());
              //toStdString().c_str(), s
              //_lineEdit->selectedText().toStdString().size());
      qDebug() << "Input Entered:" << ba << ba.length();
      ba.append("\n");
      emit sendToSocketData(ba);
    }
}