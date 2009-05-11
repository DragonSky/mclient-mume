#ifndef SIMPLELINEINPUT_H
#define SIMPLELINEINPUT_H

#include "MClientIOPlugin.h"

#include <QLineEdit>

class QEvent;

class SimpleLineInput : public MClientIOPlugin {
    Q_OBJECT
    
    public:
        SimpleLineInput(QWidget* parent=0);
        ~SimpleLineInput();

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

       public slots:
        void sendUserInput();

    private:
	QLineEdit* _lineEdit;
	QString _settingsFile;
        QString _session;
};


#endif /* SIMPLELINEINPUT_H */
