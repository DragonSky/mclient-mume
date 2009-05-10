#include "WebKitDisplay.h"

#include "MClientEvent.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QVariant>

#include <QFile>
#include <QDomDocument>
#include <QFont>

Q_EXPORT_PLUGIN2(webkitdisplay, WebKitDisplay)

const QByteArray WebKitDisplay::greatherThanChar(">");
const QByteArray WebKitDisplay::lessThanChar("<");
const QByteArray WebKitDisplay::greatherThanTemplate("&gt;");
const QByteArray WebKitDisplay::lessThanTemplate("&lt;");

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

    // Set up the Browser
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, false);

#ifdef USE_JQUERY
        QFile file;
        file.setFileName(":/webkitdisplay/jquery.min.js");
        file.open(QIODevice::ReadOnly);
        _jQuery = file.readAll();
        file.close();
#endif

        file.setFileName(":/webkitdisplay/page.html");
        file.open(QIODevice::ReadOnly);
        _pageSource = file.readAll();
        file.close();	
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
        appendText(QString("p"), qv->toByteArray());
    }
}

void WebKitDisplay::appendText(QString node, QByteArray text) {
  text.replace(greatherThanChar, greatherThanTemplate);
  text.replace(lessThanChar, lessThanTemplate);
  text.replace(QByteArray("\n"), QByteArray("<br>"));
  QString output = QRegExp::escape(text);

#ifdef USE_JQUERY
  qDebug() << "* Add to" << node << "via Javascript:" << output;
  //QString code = "$('" + node + "').each( function ()
  //{$(this).append('" + output + "') } )";

  QString code = "$('p').each( function () { $(this).css('background-color', 'yellow') } )";

  // Debugging...
  QString testCode = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";
  code = testCode;

  qDebug() << code;
  _view->page()->mainFrame()->evaluateJavaScript(code);
#endif
}

void WebKitDisplay::finishLoading(bool) {
#ifdef USE_JQUERY
  _view->page()->mainFrame()->evaluateJavaScript(_jQuery);
  qDebug() << "* PAGE LOADED, evaluating jQuery!";
#endif
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
    _view = new QWebView;
    _view->load(QUrl("http://google.com"));
    //_view->setHtml(_pageSource);
    _view->show();

    connect(_view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
//     connect(_view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
//     connect(_view, SIGNAL(titleChanged(const QString&)), SLOT(adjustTitle(const QString&)));
//     connect(_view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
//     connect(_view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));    

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
