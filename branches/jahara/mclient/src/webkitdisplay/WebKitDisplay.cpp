#include "WebKitDisplay.h"

#include "MClientEvent.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QVariant>

#include <QFile>

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
    
    _currentSection = 0;
    _maxSections = 100;
    _currentCharacterCount = 0;
    _maxCharacterCount = 2000;
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
        appendText(qv->toByteArray());
    }
}

void WebKitDisplay::appendText(QByteArray text) {
  qDebug() << "appending Text!";
  text.replace(greatherThanChar, greatherThanTemplate);
  text.replace(lessThanChar, lessThanTemplate);
  text.replace(QByteArray("\n"), QByteArray("<br>"));
  QString output = QRegExp::escape(text);

  // Maximum Amount of Permitted Characters/Sections
  _currentCharacterCount += output.size();
  if (_currentCharacterCount >= _maxCharacterCount) {
    _currentCharacterCount = 0;

    if (_currentSection >= _maxSections)
      _currentSection = 0;
    else
      _currentSection++;

    // Replace/Blank Section
#ifdef USE_JQUERY
    QString code = QString("$('.section:first').appendTo('.container').text('%1').show()").arg(output);
    qDebug() << "* Blanking Section:" << code;
    _view->page()->mainFrame()->evaluateJavaScript(code);
    return;
#endif
  }
  
  // Append text to Section
#ifdef USE_JQUERY
  QString code = QString("$('.section:last').append('%1')").arg(output);
  qDebug() << "* Appending Section:" << code;
  _view->page()->mainFrame()->evaluateJavaScript(code);
#endif
}

void WebKitDisplay::finishLoading(bool) {
#ifdef USE_JQUERY
  _view->page()->mainFrame()->evaluateJavaScript(_jQuery);

  // Populate Page with Sections
  QString code = QString("$('.section:first').each( function () { for (var id=1;id<%1;id++) { $('.section:first').clone(true).insertAfter(this); } $('.section').hide(); $('.section:last').show(); } )").arg(_maxSections);
  _view->page()->mainFrame()->evaluateJavaScript(code);
  
  code = "$('h1').fadeOut(100000, $(this).remove())";
  _view->page()->mainFrame()->evaluateJavaScript(code);

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
    _view = new QWebView;

    _view->setHtml(_pageSource);
#ifdef USE_JQUERY
    finishLoading(true); // load jQuery
#endif

    connect(_view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
    return true;
}


const bool WebKitDisplay::stopSession(QString s) {
    return true;
}


// Display plugin members
const bool WebKitDisplay::initDisplay(QString s) {
    _view->show();
    return true;
}

const QWidget* WebKitDisplay::getWidget(QString s) {
    return (QWidget*)_view;
}
