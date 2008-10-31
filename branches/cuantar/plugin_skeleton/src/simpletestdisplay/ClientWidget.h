#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>

class ClientLineEdit;
class ClientTextEdit;
class SimpleTestDisplay;

class QTcpSocket;
class QVBoxLayout;

class ClientWidget : public QWidget {
    Q_OBJECT
    
    public:
        ClientWidget(QString s, SimpleTestDisplay* st, QWidget* parent=0);
        ~ClientWidget();

    public slots:
        void sendUserInput();


    protected:
        ClientTextEdit* _textEdit;
        ClientLineEdit* _lineEdit;
        QTcpSocket* _socket;
        QVBoxLayout* _layout;
        SimpleTestDisplay* _st;

    private:
        QString _session;


    signals:
        void receivedInput();

};


#endif /* CLIENTWIDGET_H */
