#ifndef SOCKETREADER_H
#define SOCKETREADER_H

#include <QThread>

#include <QString>
#include <QVector> 

#include <tr1/memory>
using std::tr1::shared_ptr;

class QTcpSocket;


class SocketReader : public QThread {
    Q_OBJECT

    public:
        
        SocketReader(QObject* parent=0);
        ~SocketReader();

        void connectToHost(const QString& host, const int& port);
        
        const int& port() const;
        const QString& host() const;

        void closeSocket() const;

        // Wrapper functions
        //size_t read(char* buf, size_t size) const;
        //bool atEnd() const;
        //qint64 bytesAvailable() const;

    protected: 

        void postEvent(const char* data);

    private:
        QTcpSocket* _socket;
        int _port;
        QString _host;
        int _counter;
        bool _delete;

    private slots:
        void on_connect();
        void on_disconnect();
        void on_readyRead();
        void on_error();
        
};


//By Runner
#define ntohll(x) (((int64_t)(ntohl((int)((x << 32) >> 32))) << 32) | (unsigned int)ntohl(((int)(x >> 32))))
#define htonll(x) ntohll(x)

#endif /* SOCKETREADER_H */
