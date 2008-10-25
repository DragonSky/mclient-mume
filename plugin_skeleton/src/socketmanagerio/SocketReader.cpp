#include "SocketReader.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QTcpSocket>

#include <algorithm>
using std::copy;
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <tr1/memory>
using std::tr1::shared_ptr;

#include "netinet/in.h"


SocketReader::SocketReader(QObject* parent) : QThread(parent) { 
    

    _socket = new QTcpSocket();
    _socket->setReadBufferSize(0);
    _counter = 0;
    _delete = 0;

    connect(_socket, SIGNAL(connected()), this, SLOT(on_connect())); 
    connect(_socket, SIGNAL(disconnected()), this, SLOT(on_disconnect())); 
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), 
            this, SLOT(on_error())); 
    connect(_socket, SIGNAL(readyRead()), this, SLOT(on_readyRead())); 

}


void SocketReader::connectToHost(const QString& host, const int& port) {
    _host = host;
    _port = port;
    _socket->connectToHost(_host, _port);
}


SocketReader::~SocketReader() {
    delete _socket;
}


void SocketReader::on_connect() {
    char c[sizeof(uint16_t)+17];
    uint16_t s = 17;
    s = htons(s);
    
    *(reinterpret_cast<uint16_t*>(&c[0])) = s;
    
    char d[18] = "MTRawHitHistogram";
    copy(d, d+sizeof(d)-1, c+sizeof(uint16_t));

    _socket->write(c, sizeof(c));
}


void SocketReader::on_readyRead() {
    
    int len = 0;
    // read the size of the entire data stream
    char tsizea[sizeof(uint64_t)];
    if(!_socket->atEnd()) { 
        len = _socket->peek(tsizea,sizeof(tsizea));
        if(len != sizeof(tsizea)) { 
            qWarning() << "total size isn't a uint64_t!" << len;
            return;
        }
    }
    int64_t tsize = *(reinterpret_cast<uint64_t*>(&tsizea[0]));
    tsize = ntohll(tsize);
    
    if(_socket->bytesAvailable() < tsize) { //==16384) {
   //     Logger(2) << "SOMETHING TERRIBLE IS ABOUT TO HAPPEN!" << endl; 
        return;
    }
    
    ++_counter;
//    if(!isRunning()) start(LowPriority);


}


void SocketReader::on_disconnect() {
//    Logger(1) << "socket on port" << _port << "has disconnected" << endl;
    if(_delete == 0) {
        _delete = 1;
        deleteLater();
    }
}

void SocketReader::on_error() {

//    qWarning() << "Error involving" 
//       << _host << _port << _socket->errorString();
    if(_delete == 0) {
        _delete = 1;
        deleteLater();
    }
}


const int& SocketReader::port() const {
    return _port;
}


void SocketReader::closeSocket() const {
//    Logger(1) << "SocketReader::closeSocket()" << endl;
    _socket->close();
}

/*
void SocketReader::run() {
//    Logger(cout) << "SocketReader::run()" << endl;
    while(_counter > 0) {
//        Logger(2) << "_counter:" << _counter << endl;
    
        int len = 0;
    
        // read the size of the entire data stream (could skip this)
        char tsizea[sizeof(uint64_t)];
        if(!_socket->atEnd()) { 
            len = _socket->read(tsizea,sizeof(tsizea));
            if(len != sizeof(tsizea)) { 
                qWarning() << "total size isn't a uint64_t!" << len;
                return;
            }
        }
        uint64_t tsize = *(reinterpret_cast<uint64_t*>(&tsizea[0]));
        tsize = ntohll(tsize);
//        Logger(2) << "tsize:" << tsize << endl;

        // read in the entire stream
        char stream[tsize];
        if(!_socket->atEnd()) { 
            len = _socket->read(stream,sizeof(stream));
            if(len != sizeof(stream)) { 
                qWarning() << "Couldn't read" << tsize 
                    << "byes from stream!" << len;
                --_counter;
                return;
            }
        }

//        Logger(2) << "read " << len << " bytes from stream" << endl;

        // read the type to determine how many sizes to read
        uint8_t type = static_cast<uint8_t>(stream[0]);
//        Logger(2) << "type: " << type << endl;

        if(type != 1) {
            qWarning() << "type != 1!" << type;
            --_counter;
            return;
        }
    
        // Create a data object and send that out
        char data[sizeof(stream)-sizeof(type)];
        copy(&stream[sizeof(type)], 
            &stream[sizeof(type)]+sizeof(stream)-sizeof(type), data);
        DataContainer* test = DataContainerFactory::create(type, data);
//        Logger(2) << "type:" << test->type() << endl;
   
        postEvent(data); 
//        sendData(test);
//        Logger(2) << "Received:" 
        //<< sizeof(*str)*(sizeof(type)+sizeof(size)+sizeof(data)) << "bytes";
            << sizeof(stream) << "bytes" << endl;
 

        test->decRef();
        --_counter;
    }
}
*/


void SocketReader::postEvent(const char* data) {
//    Logger(2) << "Received:" << endl; 
    /*
    for(uint32_t i=0; i<_listeners.size(); i++) {
        if(_listeners[i] != 0) {
            OneDeeHistogramEvent* e = new OneDeeHistogramEvent(data);
            QApplication::postEvent(_listeners[i], e);
        }
    }
    */
    qDebug() << "really should post that data now";

}
