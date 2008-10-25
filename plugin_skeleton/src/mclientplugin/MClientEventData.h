#ifndef MCLIENTEVENTDATA_H
#define MCLIENTEVENTDATA_H

#include <QSharedData>

class QObject;


class MClientEventData : public QSharedData {
    
    public:
        MClientEventData(QObject* payload);
        ~MClientEventData();

        QObject* payload();
        //void refs() const;

    private:
        QObject* _payload;

};


#endif /* MCLIENTEVENTDATA_H */
