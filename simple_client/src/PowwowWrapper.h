#ifndef POWWOWWRAPPER_H
#define POWWOWWRAPPER_H

#include <QObject>


class ClientSocket;

class QString;

class PowwowWrapper : public QObject {
    Q_OBJECT

    public:
        static PowwowWrapper* Instance();
        static void Delete();
        ~PowwowWrapper();
        void getUserInput(const QString& str) const;
        void connectToHost(const QString& host, const qint64& port) const;

    public slots:
        void connected();
        void readSocket();

    protected:
        ClientSocket* _socket;
        PowwowWrapper(QObject* parent=0);
        
        static PowwowWrapper* _pinstance;

    signals:
        void dataReceived(const QString& data);
};


#endif /* POWWOWWRAPPER_H */
