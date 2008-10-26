#ifndef SIMPLETESTDISPLAY_H
#define SIMPLETESTDISPLAY_H

#include "MClientDisplayPlugin.h"

#include <QHash>
#include <QPointer>

class QEvent;
class QPushButton;

class ClientWidget;
class SimpleTest;


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

        // Display members
        const bool initDisplay();


    private:
        QPointer<QPushButton> _button;

        QHash<QString, SimpleTest*> _simpleTests;
        QHash<QString, ClientWidget*> _widgets;
};


#endif /* SIMPLETESTDISPLAY_H */
