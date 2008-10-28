#include "MClientEventData.h"

#include <QDebug>
#include <QVariant>


MClientEventData::MClientEventData(QVariant* payload) : QSharedData() {
//    qDebug() << "creating a shared data containing" << payload;
    _payload = payload;
    
}


MClientEventData::MClientEventData(const MClientEventData& med) {
    if(med._payload) {
        _payload = new QVariant(med._payload);
    }
}


MClientEventData::~MClientEventData() {
    qDebug() << ref << this;
//    qDebug() << "Deleting payload";
    delete _payload;
}


QVariant* MClientEventData::payload() {
//    qDebug() << ref << this << "returns payload";
    return _payload;
}

void MClientEventData::refs() const {
    qDebug() << "refs" << ref;
}
