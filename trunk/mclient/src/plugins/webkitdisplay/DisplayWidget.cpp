#include "WebKitDisplay.h"
#include "DisplayWidget.h"

#include <QDebug>
#include <QtWebKit>
#include <QWebFrame>

#include <QTimer> // Debug (for identifying lag)

//#include <QFile>

DisplayWidget::DisplayWidget(QString s, WebKitDisplay* wkd, QWidget* parent) 
    : QWebView(parent) {
    _session = s;
    _wkd = wkd;

    QWidget::setMinimumSize(0, 30);

    // Sections Information
    _currentSection = 0;
    _maxSections = 1;
    _currentCharacterCount = 0;
    _maxCharacterCount = 300000000;

    // Create WebKit Display
    page()->mainFrame()->load(QUrl("qrc:/webkitdisplay/page.html"));

    // Connect Signals/Slots
    connect(this, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
    connect(_wkd, SIGNAL(dataReceived(const QString&)),
	    SLOT(appendText(const QString&)));

    // TODO: Why aren't frames working?
    connect(page(), SIGNAL(frameCreated(QWebFrame *frame)),
	    SLOT(loadFrame(QWebFrame *frame)));

    // Debugging Information
    qDebug() << "* WebKitDisplay thread:" << _wkd->thread();
    qDebug() << "* DisplayWidget thread:" << this->thread();
    
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    page()->setContentEditable(false);

    /*
#ifdef USE_JQUERY
    QFile file;
    file.setFileName(":/webkitdisplay/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    _jQuery = file.readAll();
    file.close();
#endif
    */
}


DisplayWidget::~DisplayWidget() {
}


void DisplayWidget::appendText(const QString &output) {

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
    QString code = QString("$('.section:first').appendTo('.container').html('%1').show();").arg(output);
    qDebug() << "* Blanking Section:" << code;

#endif
  } else {
  
    // Append text to Section
#ifdef USE_JQUERY
    //QString code = QString("$('<div
    //class=\"section\">Something!</div>').appendTo('.container');");
    //QString code = QString("$('.section').css('background-color', 'yellow');");

    QTime t;
    t.start();
    QString code = QString("$('.section').append('%1');").arg(output);
    page()->mainFrame()->evaluateJavaScript(code);
    qDebug() << "* Displaying (" << t.elapsed() << "ms):" << output;

  }
#endif
  
  scrollToBottom();
}

void DisplayWidget::finishLoading(bool) {
#ifdef USE_JQUERY
  // Populate Page with Sections
  //QString code = QString("$('.section:first').each( function () { for (var id=1;id<%1;id++) { $('.section:first').clone(true).insertAfter(this); } $('.section').hide(); $('.section:last').show(); } )").arg(_maxSections);
  //page()->mainFrame()->evaluateJavaScript(code);
#endif
  
  qDebug() << "* WebKit page loaded";
}

void DisplayWidget::scrollToBottom() {
  int height = page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
  page()->mainFrame()->setScrollBarValue(Qt::Vertical, height);
}


void DisplayWidget::loadFrame(QWebFrame *frame) {
  qDebug() << "frame created!" << frame->frameName();
}