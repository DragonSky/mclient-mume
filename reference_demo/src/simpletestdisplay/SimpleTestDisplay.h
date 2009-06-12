#ifndef SIMPLETESTDISPLAY_H
#define SIMPLETESTDISPLAY_H

#include "MClientDisplayPlugin.h"

#include <QHash>
#include <QPointer>
#include <QDomDocument>

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

	public slots:
	  void sendToSocketData(const QByteArray&);
	void connectToHost();


    private:
        QPointer<QPushButton> _button;
        QHash<QString, ClientWidget*> _widgets;

	QString _settingsFile;

	QColor _blackColor, _redColor, _greenColor, _yellowColor, _blueColor, _magentaColor;
	QColor _cyanColor, _grayColor, _darkGrayColor, _brightRedColor, _brightGreenColor;
	QColor _brightYellowColor, _brightBlueColor, _brightMagentaColor, _brightCyanColor;
	QColor _whiteColor, _foregroundColor, _backgroundColor;
	QFont _serverOutputFont, _inputLineFont;

    signals: 
        void dataReceived(QString s);
};


#endif /* SIMPLETESTDISPLAY_H */
