#include "SimpleTestDisplay.h"

#include "ClientWidget.h"

#include "MClientEvent.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QPushButton>
#include <QVariant>

#include <QFile>
#include <QDomDocument>
#include <QFont>

Q_EXPORT_PLUGIN2(simpletestdisplay, SimpleTestDisplay)


SimpleTestDisplay::SimpleTestDisplay(QWidget* parent) 
        : MClientDisplayPlugin(parent) {
    _shortName = "simpletestdisplay";
    _longName = "Simple Test Display";
    _description = "A simple test display plugin.";
    _dependencies.insert("terrible_test_api", 1);
//    _implemented.insert("some_other_api",1);
    _dataTypes << "XMLDisplayData";
    _configurable = true;
    _configVersion = "2.0";

    // SocketManager members
    _settingsFile = "config/"+_shortName+".xml";

    // Default Colors
    _foregroundColor = Qt::lightGray;
    _backgroundColor = Qt::black;
    _blackColor = Qt::darkGray;
    _redColor = Qt::darkRed;
    _greenColor = Qt::darkGreen;
    _yellowColor = Qt::darkYellow;
    _blueColor = Qt::darkBlue;
    _magentaColor = Qt::darkMagenta;
    _cyanColor = Qt::darkCyan;
    _grayColor = Qt::lightGray;
    _darkGrayColor = Qt::gray;
    _brightRedColor = Qt::red;
    _brightGreenColor = Qt::green;
    _brightYellowColor = Qt::yellow;
    _brightBlueColor = Qt::blue;
    _brightMagentaColor = Qt::magenta;
    _brightCyanColor = Qt::cyan;
    _whiteColor = Qt::white;

    // Default Fonts
    _serverOutputFont = QFont("Monospace", 8);
    _inputLineFont = QApplication::font();
}


SimpleTestDisplay::~SimpleTestDisplay() {
    stopAllSessions();
    saveSettings();
}


void SimpleTestDisplay::customEvent(QEvent* e) {
//    qDebug() << "* bork bork bork!";
    if(!e->type() == 10001) return;
    
    MClientEvent* me;
    me = static_cast<MClientEvent*>(e);

    if(me->dataTypes().contains("XMLDisplayData")) {
        QByteArray ba = me->payload()->toByteArray();
//        qDebug() << "Displayed: " << ba.data();
        QVariant* qv = new QVariant(ba);
        emit dataReceived(qv->toByteArray());
    }
}


void SimpleTestDisplay::configure() {
    if(!_button) {
        qDebug() << "creating a new monkeybutton";
        _button = new QPushButton();
    }
    if(_button->thread() != 
            QApplication::instance()->thread()) {
        qDebug() << "not in main thread!";
    }
    _button->setText("monkey!");
    _button->setMinimumSize(400,400);
    if(!_button->isVisible()) _button->show();

    qDebug() << "monkey!";

}


const bool SimpleTestDisplay::loadSettings() {
 QDomDocument doc(_shortName);
 QFile file(_settingsFile);
 if (!file.open(QIODevice::ReadOnly)) {
   qCritical() << "Can't open file for reading:" << _settingsFile;
   return false;
 }
 if (!doc.setContent(&file)) {
   qCritical() << "Can't set the file to the XML DOM Reader:" << _settingsFile;
   file.close();
   return false;
 }
 file.close();

 QDomElement docElem = doc.documentElement();
 QDomNode n = docElem.firstChild();
 while(!n.isNull()) {
   QDomElement e = n.toElement(); // try to convert the node to an element.
   if(!e.isNull()) {
     if (e.tagName() == "config") {
       if(e.attribute("version").toDouble() < _configVersion.toDouble())
	 qWarning() << "Config file is too old! Trying anyway...";
     }
     else if (e.tagName() == "foreground") {
       _foregroundColor = (QColor) e.text();
     }
     else if (e.tagName() == "background") {
       _backgroundColor = (QColor) e.text();
     }
     else if (e.tagName() == "black") {
       _blackColor = (QColor) e.text();
     }
     else if (e.tagName() == "red") {
       _redColor = (QColor) e.text();
     }
     else if (e.tagName() == "green") {
       _greenColor = (QColor) e.text();
     }
     else if (e.tagName() == "yellow") {
       _yellowColor = (QColor) e.text();
     }
     else if (e.tagName() == "blue") {
       _blueColor = (QColor) e.text();
     }
     else if (e.tagName() == "magenta") {
       _magentaColor = (QColor) e.text();
     }
     else if (e.tagName() == "cyan") {
       _cyanColor = (QColor) e.text();
     }
     else if (e.tagName() == "gray") {
       _grayColor = (QColor) e.text();
     }
     else if (e.tagName() == "dark gray") {
       _darkGrayColor = (QColor) e.text();
     }
     else if (e.tagName() == "bright red") {
       _brightRedColor = (QColor) e.text();
     }
     else if (e.tagName() == "bright green") {
       _brightGreenColor = (QColor) e.text();
     }
     else if (e.tagName() == "bright yellow") {
       _brightYellowColor = (QColor) e.text();
     }
     else if (e.tagName() == "bright blue") {
       _brightBlueColor = (QColor) e.text();
     }
     else if (e.tagName() == "bright magenta") {
       _brightMagentaColor = (QColor) e.text();
     }
     else if (e.tagName() == "bright cyan") {
       _brightCyanColor = (QColor) e.text();
     }
     else if (e.tagName() == "white") {
       _whiteColor = (QColor) e.text();
     }
     else if (e.tagName() == "server output") {
       _serverOutputFont.fromString(e.text());
     }
     else if (e.tagName() == "input line") {
       _inputLineFont.fromString(e.text());
     }
   }
   n = n.nextSibling();
 }
 
 return true;
}

        
const bool SimpleTestDisplay::saveSettings() const {
  /*
  // Eh. This system blows.

  QDomDocument doc(_shortName);
  QDomElement root = doc.createElement("config");
  root.setAttribute("version", _configVersion);
  doc.appendChild(root);

  QDomElement profile = doc.createElement("profile");
  profile.setAttribute("name", session());
  root.appendChild(profile);
  
  QDomElement colors = doc.createElement("colors");
  profile.appendChild(colors);

  QDomElement tag = doc.createElement("foreground");
  colors.appendChild(t);
  QDomText t = doc.createTextNode(_foregroundColor.name()); 
  tag.appendChild(t);

  QFile file(_settingsFile);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qCritical() << "Can't open file for writing:" << _settingsFile;
    return false;
  }
  
  QTextStream out(&file);
  out << doc.toString();
  file.close();

  qDebug() << "* wrote " << _shortName << " xml";
  */

  return true;
}


const bool SimpleTestDisplay::startSession(QString s) {
    //initDisplay(s);
    return true;
}


const bool SimpleTestDisplay::stopSession(QString s) {
    return true;
}


// Display plugin members
const bool SimpleTestDisplay::initDisplay(QString s) {
    ClientWidget* cw = new ClientWidget(s, this);
   
    _widgets.insert(s, cw); 

    cw->show();

    emit dataReceived(QString("Type #connect to connect to MUME\n"));

    return true;

}

const QWidget* SimpleTestDisplay::getWidget(QString s) {
    ClientWidget* cw = new ClientWidget(s, this);
   
    _widgets.insert(s, cw); 

    //cw->show();

    emit dataReceived(QString("Type #connect to connect to MUME\n"));

    return cw;

}
