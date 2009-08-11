#ifndef WEBKITDISPLAY_H
#define WEBKITDISPLAY_H

#include "MClientDisplayPlugin.h"

#include <QPointer>

class DisplayWidget;
class DisplayParser;
class QEvent;

class WebKitDisplay : public MClientDisplayPlugin {
    Q_OBJECT
    
    public:
        WebKitDisplay(QWidget* parent=0);
        ~WebKitDisplay();

        // Plugin members
        void customEvent(QEvent* e);
        void configure();
        bool loadSettings();
        bool saveSettings() const;
        bool startSession(QString s);
        bool stopSession(QString s);

        // Display members
        bool initDisplay(QString s);
        QWidget* getWidget(QString s);

  protected:
	void run();

    private:
	QPointer<DisplayWidget> _widget;
	DisplayParser *_parser;

 signals:
	void displayData(const QString&);
	void userInput(const QString&);
};


#endif /* WEBKITDISPLAY_H */