#include "SocketManagerIO.h"

#include "SocketManagerIOConfig.h"

#include <QByteArray>
#include <QEvent>
#include <QSettings>

Q_EXPORT_PLUGIN2(socketmanagerio, SocketManagerIO)


SocketManagerIO::SocketManagerIO(QObject* parent) 
        : MClientIOPlugin(parent) {
    
    _libName = "libsocketmanagerio.so";
    _shortName = "socketmanagerio";
    _longName = "SocketManager";
    _description = "A socket plugin that reads from sockets and inserts the data back into the stream.";
//    _dependencies.insert("some_stupid_api", 10);
    _implemented.insert("some_stupid_api",10);
    _dataTypes << "SendToSocketEvent";
    _configurable = true;

    // SocketManager members

    // Load config: figure out how many sockets, and load them.
    QSettings s;
    _host = s.value(_shortName+"/host").toString();
    _port = s.value(_shortName+"/port").toInt();
}


SocketManagerIO::~SocketManagerIO() {
    // Clean up the QHash of sockets.
}


// MClientPlugin members
void SocketManagerIO::customEvent(QEvent* e) {
    
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
    QSettings s;
    s.value(_shortName+"/testkey", 60);
}


const bool SocketManagerIO::saveSettings() const {
    QSettings s;
    s.setValue(_shortName+"/testkey", 80);
}


void SocketManagerIO::connectDevice() {
    // Adds a socket to the hash and attempts to connect.  May prompt the
    // user for input about where to connect and on what port.
}


void SocketManagerIO::disconnectDevice() {
    // Disconnect a particular socket.  Removes it if successful.
}


void SocketManagerIO::sendData(const QByteArray data) {
    // Send data to the sockets.
}


// Implementation-specific details: slots for successful operations
