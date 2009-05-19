#include "WebKitDisplay.h"
#include "DisplayWidget.h"

#include "MClientEvent.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QVariant>
#include <QtWebKit>

Q_EXPORT_PLUGIN2(webkitdisplay, WebKitDisplay)

const QByteArray WebKitDisplay::greatherThanChar(">");
const QByteArray WebKitDisplay::lessThanChar("<");
const QByteArray WebKitDisplay::greatherThanTemplate("&gt;");
const QByteArray WebKitDisplay::lessThanTemplate("&lt;");

WebKitDisplay::WebKitDisplay(QWidget* parent) 
        : MClientDisplayPlugin(parent) {
    _shortName = "webkitdisplay";
    _longName = "WebKit Display";
    _description = "A display plugin using WebKit.";
//   _dependencies.insert("terrible_test_api", 1);
//    _implemented.insert("some_other_api",1);
    _dataTypes << "XMLDisplayData";
    _configurable = false;
    _configVersion = "2.0";

    // SocketManager members
    _settingsFile = "config/"+_shortName+".xml";

    // Set up the Browser
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, false);

    // Debugging
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    // Allowable Display Locations
    SET(_displayLocations, DL_CENTER);
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
        QVariant* qv = new QVariant(ba);
	parseDisplayData(qv->toByteArray());
    }
}

void WebKitDisplay::parseDisplayData(const QByteArray& output) {
  QString text(output);
  qDebug() << "* WebKit appending text:" << text;
  text.replace(greatherThanChar, greatherThanTemplate);
  text.replace(lessThanChar, lessThanTemplate);
  text.replace(QByteArray("\n"), QByteArray("<br>"));
  text.replace(QByteArray("'"), QByteArray("\\'"));
  text.replace(QByteArray("`"), QByteArray("\\`"));
  text.replace(QByteArray("$"), QByteArray("\\$"));

  // ANSI Removal
  QRegExp ansiRx("\\0033\\[((?:\\d+;)*\\d+)m");
  QStringList blocks = text.split(ansiRx);
  
  emit dataReceived(blocks.join(""));
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
    initDisplay(s);
    _runningSessions << s;
    return true;
}


const bool WebKitDisplay::stopSession(QString s) {
    int removed =  _runningSessions.removeAll(s);
    return removed!=0?true:false;
}


// Display plugin members
const bool WebKitDisplay::initDisplay(QString s) {
    DisplayWidget* dw = new DisplayWidget(s, this);
    _widgets.insert(s, dw); 
    dw->show();

    return true;
}

const QWidget* WebKitDisplay::getWidget(QString s) {
    return (QWidget*)_widgets[s];
}
