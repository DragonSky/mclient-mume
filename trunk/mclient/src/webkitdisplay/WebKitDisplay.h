#ifndef WEBKITDISPLAY_H
#define WEBKITDISPLAY_H

#include "MClientDisplayPlugin.h"

#include <QHash>

class DisplayWidget;
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

  protected:
	static const QByteArray greatherThanChar;
	static const QByteArray lessThanChar;
	static const QByteArray greatherThanTemplate;
	static const QByteArray lessThanTemplate;

    private:
	QString _settingsFile;
	QHash<QString, DisplayWidget*> _widgets;

	void parseDisplayData(const QByteArray& text);
	
    signals:
        void dataReceived(QString s);
};


#endif /* WEBKITDISPLAY_H */
