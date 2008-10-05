#ifndef WRAPPERSOCKET_H
#define WRAPPERSOCKET_H

#include <QTcpSocket>

class Wrapper;


class WrapperSocket: public QTcpSocket
{
  Q_OBJECT

  public:
    WrapperSocket(char*, int, Wrapper *parent);
    ~WrapperSocket();

    int i;
    char *id;
    char *initstring;

  public slots:
    void contentAvailable();
    void socketConnected();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError socketError);

  private:
    Wrapper *wrapper;
};

#endif // WRAPPERSOCKET_H
