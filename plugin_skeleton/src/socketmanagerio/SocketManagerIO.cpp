#include "SocketManagerIO.h"

#include "SocketManagerIOConfig.h"
#include "SocketReader.h"

#include "MClientEvent.h"
#include "PluginManager.h"

#include <QApplication>
#include <QByteArray>
#include <QDebug>
#include <QEvent>
#include <QFile>
#include <QMap>
#include <QSettings>
#include <QtXml>

Q_EXPORT_PLUGIN2(socketmanagerio, SocketManagerIO)


SocketManagerIO::SocketManagerIO(QObject* parent) 
        : MClientIOPlugin(parent) {
    
    _shortName = "socketmanagerio";
    _longName = "SocketManager";
    _description = "A socket plugin that reads from sockets and inserts the data back into the stream.";
//    _dependencies.insert("some_stupid_api", 10);
    _implemented.insert("some_stupid_api",10);
    _dataTypes << "SendToSocketData" << "ConnectToHost";
    _configurable = true;

    // SocketManager members
    _settingsFile = "config/"+_shortName+".xml";

}


SocketManagerIO::~SocketManagerIO() {
    // Clean up the QHash of sockets.
    stopAllSessions();
    saveSettings();
}


// MClientPlugin members
void SocketManagerIO::customEvent(QEvent* e) {
    if(!e->type() == 10001) return;
    
    MClientEvent* me;
    me = static_cast<MClientEvent*>(e);

    if(me->dataTypes().contains("SendToSocketData")) {
        QByteArray ba = me->payload()->toByteArray();
        qDebug() << ba.data();
        
        SocketReader* sr;
        foreach(sr, _sockets.values(me->session())) {
            sr->sendToSocket(new QByteArray(ba.data()));
        }

    } else if(me->dataTypes().contains("ConnectToHost")) {
        /*
        QList<QVariant> ls = me->payload()->toList();
        qDebug() << ls;
        */
        connectDevice(me->session());
    }
}


void SocketManagerIO::configure() {
    // Need to display a table of (identifier, host, port)
    // Selecting one and pushing an "Edit..." button will bring up a dialog
    // with three QLineEdits, one for each field.  Closing that will save the
    // data to a member variable and update the table.
    
    // It will originally be populated by QSettings.

    if(!_configWidget) _configWidget = new SocketManagerIOConfig();
    if(!_configWidget->isVisible()) _configWidget->show();

}


const bool SocketManagerIO::loadSettings() {
    QIODevice* device = new QFile(_settingsFile);
    if(!device->open(QIODevice::ReadOnly)) {
        qCritical() << "Can't open file for reading:" << _settingsFile;
        return false;
    }

    QXmlStreamReader* xml = new QXmlStreamReader(device);
    QString profile;
    QPair<QString, QVariant> p;
    while(!xml->atEnd()) {
        xml->readNext();

        if(xml->isEndElement()) {
            if(xml->name() == "profile") {
                // found </profile>
            }

        } else if(xml->isStartElement()) {
            if(xml->name() == "profile") {
                QXmlStreamAttributes attr = xml->attributes();
                profile = attr.value("name").toString();
                qDebug() << "* found profile:" << profile;

            } else if(xml->name() == "host") {
                QString host = xml->readElementText();
                p.first = "host";
                p.second = host;
                _settings.insert(profile, p);
                qDebug() << "* inserted host:" << host;
            
            } else if(xml->name() == "port") {
                QString port = xml->readElementText();
                p.first = "port";
                p.second = port;
                _settings.insert(profile, p);
                qDebug() << "* inserted port:" << port;
            }
        }
    }

    delete device;
    delete xml;
    
    return true;
}


const bool SocketManagerIO::saveSettings() const {
    QIODevice* device = new QFile(_settingsFile);
    if(!device->open(QIODevice::WriteOnly)) {
        qCritical() << "Can't open file for writing:" << _settingsFile;
        return false;
    }

    QXmlStreamWriter* xml = new QXmlStreamWriter(device);
    xml->setAutoFormatting(true);
    xml->writeStartDocument();
    xml->writeStartElement("config");
    xml->writeAttribute("version", "1");

    foreach(QString s, _settings.uniqueKeys()) {
        xml->writeStartElement("profile");
        xml->writeAttribute("name", s);
        
        QPair<QString, QVariant> p;
        foreach(p, _settings.values(s)) {
            xml->writeStartElement(p.first);
            xml->writeCharacters(p.second.toString());
            xml->writeEndElement();
        }

        xml->writeEndElement(); // profile
    }

    xml->writeEndElement(); // config
    xml->writeEndDocument();
    qDebug() << "* wrote xml";

    delete device;
    delete xml;

    return true;
}


const bool SocketManagerIO::startSession(QString s) {
    
    QString host;
    int port = 0;
    qDebug() << _settings.values(s);
    QPair<QString, QVariant> p;
    foreach(p, _settings.values(s)) {
        if(p.first == "host") host = p.second.toString();
        else if(p.first == "port") port = p.second.toInt();
    }
    SocketReader* sr = new SocketReader(s, this);
    qDebug() << "* threads:" << sr->thread() << this->thread();
    sr->moveToThread(this->thread());
    qDebug() << "* threads:" << sr->thread() << this->thread();
    sr->host(host);
    sr->port(port);
    _sockets.insert(s, sr);
    _runningSessions << s;
    qDebug() << "* inserted SocketReader for session" << s;
    return true;
}


const bool SocketManagerIO::stopSession(QString s) {
    foreach(SocketReader* sr, _sockets.values(s)) {
        delete sr;
        qDebug() << "* removed SocketReader for session" << s;
    }
    _sockets.remove(s);
    int removed = _runningSessions.removeAll(s);
    return removed!=0?true:false;
}


// IO members
void SocketManagerIO::connectDevice(QString s) {
    qDebug() << "* SocketManagerIO thread:" << this->thread();
    // Attempts to connect every socket associated with the session s
    foreach(SocketReader* sr, _sockets.values(s)) {
        qDebug() << "* threads:" << sr->thread() << this->thread();
        sr->connectToHost();//"mume.org",4242);
        qDebug() << "* connected socket for session" << s;
    }
}


void SocketManagerIO::disconnectDevice(QString s) {
    // Disconnect a particular session's sockets.
    foreach(SocketReader* sr, _sockets.values(s)) {
        sr->closeSocket();
    }
}


void SocketManagerIO::sendData(const QByteArray data) {
    // Send data to the sockets.
}


void SocketManagerIO::socketReadData(const QByteArray data, const QString s) {
    qDebug() << "received data from" << s;

    QVariant* qv = new QVariant(data);
    QStringList tags;
    tags << "SocketData";

    postEvent(qv, tags, s);
}


// Implementation-specific details: slots for successful operations
