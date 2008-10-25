#include "MClientEventData.h"

#include <QDebug>
#include <QObject>


MClientEventData::MClientEventData(QObject* payload) : QSharedData() {
//    qDebug() << "creating a shared data containing" << payload;
    _payload = payload;
    
}


MClientEventData::~MClientEventData() {
//    qDebug() << ref << this;
//    qDebug() << "Deleting payload";
    delete _payload;
}


QObject* MClientEventData::payload() {
//    qDebug() << ref << this << "returns payload";
    return _payload;
}

/*
void MClientEventData::refs() const {
    qDebug() << "refs" << ref;
}
*/
