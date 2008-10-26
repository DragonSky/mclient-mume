#include "PowwowWrapper.h"

#include "ClientSocket.h"

#include <QDebug>
#include <QString>


PowwowWrapper* PowwowWrapper::_pinstance = 0;

PowwowWrapper* PowwowWrapper::Instance() {
    if(_pinstance == 0) {
        _pinstance = new PowwowWrapper();
        return _pinstance;
    }
}


void PowwowWrapper::Delete() {
    delete _pinstance;
    _pinstance = 0;
}


PowwowWrapper::PowwowWrapper(QObject* parent) : QObject(parent) {
    _socket = new ClientSocket(this);
    
    connect(_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}


PowwowWrapper::~PowwowWrapper() {
}


void PowwowWrapper::getUserInput(const QString& str) const {
    //if(linemode & LM_CHAR) {
    /* char mode. */
    /*
        for (i = 0; i < n; i++) {
            wrapper_tcp_write(tcp_fd, input.mid(i,1).toAscii().data(), 1);
            if (!(linemode & LM_NOECHO))
            tty_putc(input.at(i).toAscii() );
            last_edit_cmd = (function_any)0;
        }
    */
    //}

    qDebug() << "Got user input:" << str << str.size();
    int len = _socket->write(str.toLatin1().data(), str.size());
    _socket->write("\n");
    qDebug() << len << "bytes written";

}


void PowwowWrapper::connected() { 
    qDebug() << "We're connected now!";
}


void PowwowWrapper::readSocket() {
    QString data = _socket->read(1024);    
    emit dataReceived(data);
}


void PowwowWrapper::connectToHost(const QString& host, 
        const qint64& port) const {
    _socket->connectToHost(host, port);
}

/*
#include <boost/python.hpp>
using namespace boost::python;
*/
//BOOST_PYTHON_MODULE(powwow) 
//{
    //class_<QObject>("QObject", no_init);
    //class_<PowwowWrapper, bases<QObject> >("PowwowWrapper", no_init);
    /*
        .def("connectToHost", &PowwowWrapper::connectToHost)
        .def("getUserInput", &PowwowWrapper::getUserInput)
    ;
    */
    //def("Instance", &PowwowWrapper::Instance);,
   //         return_value_policy<manage_new_object>());
//}
