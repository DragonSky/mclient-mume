#include "SocketManagerIO.h"

#include "SocketManagerIOConfig.h"
#include "SocketReader.h"

#include "MClientEvent.h"
#include "PluginManager.h"

#include <QApplication>
#include <QByteArray>
#include <QDebug>
#include <QEvent>
#include <QIODevice>
#include <QMap>
#include <QSettings>

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
    saveSettings();

}


SocketManagerIO::~SocketManagerIO() {
    // Clean up the QHash of sockets.
    stopAllSessions();
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
    // Here's one profile -- need to be able to load many
    QSettings s;
    QMap<QString, QVariant> m;
    m = s.value(_shortName+"/test").toMap();
    qDebug() << m.value("host").toString();
    qDebug() << m.value("port").toInt();
}


const bool SocketManagerIO::saveSettings() const {
    QSettings s;
    QMap<QString, QVariant> m;
    m.insert("host", "mume.org");
    m.insert("port", 4242);
    s.setValue(_shortName+"/test", QVariant(m));
}


const bool SocketManagerIO::startSession(QString s) {
    SocketReader* sr = new SocketReader(s, this);
    qDebug() << "* threads:" << sr->thread() << this->thread();
    sr->moveToThread(this->thread());
    qDebug() << "* threads:" << sr->thread() << this->thread();
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
        sr->connectToHost("mume.org",4242);
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
