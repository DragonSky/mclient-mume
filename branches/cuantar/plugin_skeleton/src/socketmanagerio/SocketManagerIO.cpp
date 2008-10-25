#include "SocketManagerIO.h"

#include <QByteArray>
#include <QEvent>

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

}


SocketManagerIO::~SocketManagerIO() {
    // Clean up the QHash of sockets.
}


// MClientPlugin members
void SocketManagerIO::customEvent(QEvent* e) {
}


void SocketManagerIO::configure() {
}


const bool SocketManagerIO::loadSettings() {
}


const bool SocketManagerIO::saveSettings() const {
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
