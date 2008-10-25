#include "MClientEvent.h"

#include "MClientEventData.h"

#include <QDebug>
#include <QObject>
#include <QString>


MClientEvent::MClientEvent(MClientEventData* payload, const QString type) 
        : QEvent(QEvent::Type(10001)) {
//    qDebug() << "MClientEvent::MClientEvent()";

    // We take ownership of payload!
    _payload = QSharedDataPointer<MClientEventData>(payload);
    //_payload->refs();
    _dataType = type;
}

MClientEvent::MClientEvent(const MClientEvent& me) 
        : QEvent(QEvent::Type(10001)), _payload(me._payload) {
     //       qDebug() << "copying shit, yo";
/*
//    MClientEvent& me2 = const_cast<MClientEvent&>(me);
    _dataType = me.dataType();
    _payload = QSharedDataPointer<MClientEventData>(me.shared());
//    me2.relinquishPayload();
*/
}


MClientEvent::~MClientEvent() {
    //qDebug() << "MClientEvent::~MClientEvent()";
}


const QString& MClientEvent::dataType() const {
    return _dataType;
}


QObject* MClientEvent::payload() {
    return _payload->payload();
}


QSharedDataPointer<MClientEventData> MClientEvent::shared() const {
    return _payload;
}

/*
void MClientEvent::relinquishPayload() {
    qDebug() << "giving up payload... :(";
    _payload = 0;
}
*/
