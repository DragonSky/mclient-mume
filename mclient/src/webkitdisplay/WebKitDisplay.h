#ifndef WEBKITDISPLAY_H
#define WEBKITDISPLAY_H

#include "MClientPlugin.h"

#include <QPointer>
#include <QDomDocument>

class QEvent;

class WebKitDisplay : public MClientPlugin {
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
	QString _settingsFile;

    signals: 
        void dataReceived(QString s);
};


#endif /* WEBKITDISPLAY_H */
