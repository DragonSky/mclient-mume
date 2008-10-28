#include "SocketData.h"

#include <QByteArray>


SocketData::SocketData(const QByteArray data, QObject* parent) 
        : QObject(parent) {
    _data = data;
}


SocketData::~SocketData() {
}


QByteArray SocketData::data() const {
    return _data;
}
