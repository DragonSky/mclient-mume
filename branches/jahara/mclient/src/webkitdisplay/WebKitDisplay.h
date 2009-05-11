#ifndef WEBKITDISPLAY_H
#define WEBKITDISPLAY_H

#include "MClientDisplayPlugin.h"

#include <QtWebKit>

// for Qt 4.5, we use JQuery
// for Qt >= 4.6, we use the QWebElement API
#if QT_VERSION < 0x040600
#define USE_JQUERY
#endif

#if QT_VERSION < 0x0040500
#error You need Qt 4.5 or newer
#endif

class QEvent;

class WebKitDisplay : public MClientDisplayPlugin {
    Q_OBJECT
    
    public:
        WebKitDisplay(QWidget* parent=0);
        ~WebKitDisplay();

        // Plugin members
        void customEvent(QEvent* e);
        void configure();
        const bool loadSettings();
        const bool saveSettings() const;
        const bool startSession(QString s);
        const bool stopSession(QString s);

        // Display members
        const bool initDisplay(QString s);
        const QWidget* getWidget(QString s);

    private:
	QString _settingsFile, _pageSource;
	QWebView* _view;
#ifdef USE_JQUERY
	QString _jQuery;
#endif
	int _currentSection, _maxSections,
	  _currentCharacterCount, _maxCharacterCount;
	
	void appendText(QByteArray text);

 protected:
	static const QByteArray greatherThanChar;
	static const QByteArray lessThanChar;
	static const QByteArray greatherThanTemplate;
	static const QByteArray lessThanTemplate;

protected slots:
	void finishLoading(bool);

    signals: 
        void dataReceived(QString s);
};


#endif /* WEBKITDISPLAY_H */
