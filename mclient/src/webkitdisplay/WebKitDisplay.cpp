#include "WebKitDisplay.h"

#include "MClientEvent.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QVariant>

#include <QFile>
#include <QDomDocument>
#include <QFont>

#include <QtWebKit>

Q_EXPORT_PLUGIN2(webkitdisplay, WebKitDisplay)


WebKitDisplay::WebKitDisplay(QWidget* parent) 
        : MClientPlugin(parent) {
    _shortName = "webkitdisplay";
    _longName = "WebKit Display";
    _description = "A display plugin using WebKit.";
    _dependencies.insert("terrible_test_api", 1);
//    _implemented.insert("some_other_api",1);
    _dataTypes << "XMLDisplayData";
    _configurable = true;
    _configVersion = "2.0";

    // SocketManager members
    _settingsFile = "config/"+_shortName+".xml";
}


WebKitDisplay::~WebKitDisplay() {
    stopAllSessions();
    saveSettings();
}


void WebKitDisplay::customEvent(QEvent* e) {
//    qDebug() << "* bork bork bork!";
    if(!e->type() == 10001) return;
    
    MClientEvent* me;
    me = static_cast<MClientEvent*>(e);

    if(me->dataTypes().contains("XMLDisplayData")) {
        QByteArray ba = me->payload()->toByteArray();
//        qDebug() << "Displayed: " << ba.data();
        QVariant* qv = new QVariant(ba);
        emit dataReceived(qv->toByteArray());
    }
}


void WebKitDisplay::configure() {
}


const bool WebKitDisplay::loadSettings() {
 return true;
}

        
const bool WebKitDisplay::saveSettings() const {
  return true;
}


const bool WebKitDisplay::startSession(QString s) {
    //initDisplay(s);
  QWebView *view = new QWebView;
  view->load(QUrl("http://mume.org"));
  view->show();
    return true;
}


const bool WebKitDisplay::stopSession(QString s) {
    return true;
}


// Display plugin members
const bool WebKitDisplay::initDisplay(QString s) {

    return true;
}

const QWidget* WebKitDisplay::getWidget(QString s) {
    return 0;

}
