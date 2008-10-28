#include "SimpleTestDisplay.h"

#include "ClientWidget.h"
#include "PowwowWrapper.h"
#include "SimpleTest.h"

#include "MClientEvent.h"
#include "SocketData.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QPushButton>
#include <QVariant>

Q_EXPORT_PLUGIN2(simpletestdisplay, SimpleTestDisplay)


SimpleTestDisplay::SimpleTestDisplay(QWidget* parent) 
        : MClientDisplayPlugin(parent) {
    _libName = "libsimpletestdisplay.so";
    _shortName = "simpletestdisplay";
    _longName = "Simple Test Display";
    _description = "A simple test display plugin.";
    _dependencies.insert("terrible_test_api", 1);
//    _implemented.insert("some_other_api",1);
    _dataTypes << "FilteredData";
    _configurable = true;

    qDebug() << this->configurable();
}


SimpleTestDisplay::~SimpleTestDisplay() {
}


void SimpleTestDisplay::customEvent(QEvent* e) {
    qDebug() << "simple test display got an event!";
    if(!e->type() == 10001) return;
    
    MClientEvent* me;
    me = static_cast<MClientEvent*>(e);

    if(me->dataTypes().contains("FilteredData")) {
        QByteArray ba = me->payload()->toByteArray();
        qDebug() << ba.data();
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


// Display plugin members
const bool SimpleTestDisplay::initDisplay() {
    PowwowWrapper* pw = PowwowWrapper::Instance();
    ClientWidget* cw = new ClientWidget();
   
    _widgets.insert("monkey!", cw); 

    //pw->connectToHost("mume.org", 4242);
    cw->show();

    return true;

}
