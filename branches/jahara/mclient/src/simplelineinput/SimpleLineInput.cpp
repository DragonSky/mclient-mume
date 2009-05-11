#include <QLineEdit>
#include <QApplication>
#include <QDebug>
#include <QVariant>

#include "SimpleLineInput.h"

#include "MClientEvent.h"
#include "MClientEventData.h"
#include "PluginManager.h"

Q_EXPORT_PLUGIN2(simplelineinput, SimpleLineInput)


SimpleLineInput::SimpleLineInput(QWidget* parent) 
        : MClientIOPlugin(parent) {
    _shortName = "simplelineinput";
    _longName = "Simple Line Input";
    _description = "A simple line input plugin.";
    //_dependencies.insert("terrible_test_api", 1);
//    _implemented.insert("some_other_api",1);
    _dataTypes << "ChangeUserInput";
    _configurable = false;
    _configVersion = "2.0";

    // SocketManager members
    _settingsFile = "config/"+_shortName+".xml";
}


SimpleLineInput::~SimpleLineInput() {
    stopAllSessions();
    saveSettings();
}

void SimpleLineInput::customEvent(QEvent* e) {
//    qDebug() << "* bork bork bork!";
    if(!e->type() == 10001) return;
    
    MClientEvent* me;
    me = static_cast<MClientEvent*>(e);

    if(me->dataTypes().contains("ChangeUserInput")) {
      //        QByteArray ba = me->payload()->toByteArray();
//        qDebug() << "Displayed: " << ba.data();
//        QVariant* qv = new QVariant(ba);
      //emit dataReceived(qv->toByteArray());
    }
}

void SimpleLineInput::sendUserInput() {
    _lineEdit->selectAll();

    // Display Data
    QString input = _lineEdit->selectedText();
    QVariant* qv = new QVariant(input);
    QStringList sl("XMLDisplayData");
    MClientEvent* me = new MClientEvent(new MClientEventData(qv),sl);
    me->session(_session);
    QApplication::postEvent(PluginManager::instance(), me);
    
    sl.clear();
    if (_lineEdit->selectedText().startsWith("#")) {
      // Parse as Command
      input = _lineEdit->selectedText().mid(1);
      qDebug() << "posting INPUT " << input;
      qv = new QVariant(input);
      sl << "CommandInput";
      me = new MClientEvent(new MClientEventData(qv),sl);
      me->session(_session);
      QApplication::postEvent(PluginManager::instance(), me);
	  
    } else {
      // Send to Socket
      QByteArray ba(_lineEdit->selectedText().toLatin1());
      qDebug() << "Input Entered:" << ba << ba.length();
      ba.append("\n");
      qv = new QVariant(ba);
      sl << "SendToSocketData";
      me = new MClientEvent(new MClientEventData(qv),sl);
      me->session(_session);
      QApplication::postEvent(PluginManager::instance(), me);    
    }
}


void SimpleLineInput::configure() {
}


const bool SimpleLineInput::loadSettings() {
  return true;
}

        
const bool SimpleLineInput::saveSettings() const {
  return true;
}


const bool SimpleLineInput::startSession(QString s) {
    _session = s;
    _lineEdit = new QLineEdit;
    _lineEdit->setMaxLength(255);

    // Send data when user presses return
    connect(_lineEdit, SIGNAL(returnPressed()), this, SLOT(sendUserInput()));

    initDisplay(s);
    return true;
}


const bool SimpleLineInput::stopSession(QString s) {
    return true;
}


// Display plugin members
const bool SimpleLineInput::initDisplay(QString s) {
  _lineEdit->show();
    return true;
}

const QWidget* SimpleLineInput::getWidget(QString s) {
  return 0;
}
