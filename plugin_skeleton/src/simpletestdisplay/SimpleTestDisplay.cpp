#include "SimpleTestDisplay.h"

#include "SimpleTest.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QPushButton>

Q_EXPORT_PLUGIN2(simpletestdisplay, SimpleTestDisplay)


SimpleTestDisplay::SimpleTestDisplay(QWidget* parent) 
        : MClientDisplayPlugin(parent) {
    _libName = "libsimpletestdisplay.so";
    _shortName = "simpletestdisplay";
    _longName = "Simple Test Display";
    _description = "A simple test display plugin.";
    _dependencies.insert("terrible_test_api", 1);
//    _implemented.insert("some_other_api",1);
    _dataTypes << "SimpleTestEvent";
    _configurable = true;

    qDebug() << this->configurable();
}


SimpleTestDisplay::~SimpleTestDisplay() {
}


void SimpleTestDisplay::customEvent(QEvent* e) {
    qDebug() << "simple test display got an event!";
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

    // This is a test
    //_simpleTests.insert("SimpleTest", new SimpleTest());
    qDebug() << "monkey!";
}


const bool SimpleTestDisplay::loadSettings() {
    return true;
}

        
const bool SimpleTestDisplay::saveSettings() const {
    return true;
}
