#ifndef SIMPLETESTDISPLAY_H
#define SIMPLETESTDISPLAY_H

#include "MClientDisplayPlugin.h"

#include <QHash>
#include <QPointer>

class QEvent;
class QPushButton;

class ClientWidget;


class SimpleTestDisplay : public MClientDisplayPlugin {
    Q_OBJECT
    
    public:
        SimpleTestDisplay(QWidget* parent=0);
        ~SimpleTestDisplay();

        // Plugin members
        void customEvent(QEvent* e);
        void configure();
        const bool loadSettings();
        const bool saveSettings() const;
        const bool startSession(QString s);
        const bool stopSession(QString s);

        // Display members
        const bool initDisplay(QString s);


    private:
        QPointer<QPushButton> _button;

        QHash<QString, ClientWidget*> _widgets;

    signals: 
        void dataReceived(QString s);
};


#endif /* SIMPLETESTDISPLAY_H */