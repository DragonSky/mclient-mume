#ifndef SOCKETDATA_H
#define SOCKETDATA_H

#include <QObject>
#include <QSharedData>


class SocketData : public QObject, public QSharedData {
    Q_OBJECT

    public:
        SocketData(const QByteArray data, QObject* parent=0);
        ~SocketData();

        const QByteArray& data() const;

    private:
        QByteArray _data;

};

#endif /* SOCKETDATA_H */
