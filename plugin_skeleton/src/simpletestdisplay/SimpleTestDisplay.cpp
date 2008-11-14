#include "SimpleTestDisplay.h"

#include "ClientWidget.h"

#include "MClientEvent.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QPushButton>
#include <QVariant>

Q_EXPORT_PLUGIN2(simpletestdisplay, SimpleTestDisplay)


SimpleTestDisplay::SimpleTestDisplay(QWidget* parent) 
        : MClientDisplayPlugin(parent) {
    _shortName = "simpletestdisplay";
    _longName = "Simple Test Display";
    _description = "A simple test display plugin.";
    _dependencies.insert("terrible_test_api", 1);
//    _implemented.insert("some_other_api",1);
    _dataTypes << "XMLDisplayData";
    _configurable = true;

}


SimpleTestDisplay::~SimpleTestDisplay() {
    stopAllSessions();
}


void SimpleTestDisplay::customEvent(QEvent* e) {
    qDebug() << "* bork bork bork!";
    if(!e->type() == 10001) return;
    
    MClientEvent* me;
    me = static_cast<MClientEvent*>(e);

    if(me->dataTypes().contains("XMLDisplayData")) {
        QByteArray ba = me->payload()->toByteArray();
        qDebug() << "Displayed: " << ba.data();
        QVariant* qv = new QVariant(ba);
        emit dataReceived(qv->toByteArray());
    }
}


void SimpleTestDisplay::configure() {
    if(!_button) {
        qDebug() << "creating a new monkeybutton";
        _button = new QPushButton();
    }
    if(_button->thread() != 
            QApplication::instance()->thread()) {
        qDebug() << "not in main thread!";
    }
    _button->setText("monkey!");
    _button->setMinimumSize(400,400);
    if(!_button->isVisible()) _button->show();

    qDebug() << "monkey!";

}


const bool SimpleTestDisplay::loadSettings() {
    return true;
}

        
const bool SimpleTestDisplay::saveSettings() const {
    return true;
}


const bool SimpleTestDisplay::startSession(QString s) {
    initDisplay(s);
    return true;
}


const bool SimpleTestDisplay::stopSession(QString s) {
    return true;
}


// Display plugin members
const bool SimpleTestDisplay::initDisplay(QString s) {
    ClientWidget* cw = new ClientWidget(s, this);
   
    _widgets.insert(s, cw); 

    cw->show();

    emit dataReceived(QString("Type #connect to connect to MUME\n"));

    return true;

}
