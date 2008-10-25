#ifndef MCLIENTEVENT_H
#define MCLIENTEVENT_H

#include <QEvent>

#include "MClientEventData.h"

#include <QSharedDataPointer>
#include <QString>

//class MClientEventData;

class QObject;
class QSharedData;


class MClientEvent : public QEvent {
    
    public:
        MClientEvent(MClientEventData* payload, const QString type);
        MClientEvent(const MClientEvent& me);
        ~MClientEvent();
        const QString& dataType() const;
        QObject* payload();
//        void relinquishPayload();
        QSharedDataPointer<MClientEventData> shared() const;

    protected:
        QString _dataType;

        // Here's where we store our data
        QSharedDataPointer<MClientEventData> _payload;
                
};


#endif /* MCLIENTEVENT_H */
