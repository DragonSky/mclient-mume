#ifndef WRAPPERSOCKET_H
#define WRAPPERSOCKET_H

#include <QTcpSocket>

class PowwowWrapper;


class ClientSocket: public QTcpSocket
{
  Q_OBJECT

  public:
    ClientSocket(char*, int, PowwowWrapper *parent);
    ~ClientSocket();

    int i;
    char *id;
    char *initstring;

  public slots:
    void contentAvailable();
    void socketConnected();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError socketError);

  private:
    PowwowWrapper *wrapper;
};

#endif // WRAPPERSOCKET_H
