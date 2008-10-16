#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>

class ClientLineEdit;
class ClientTextEdit;

class QTcpSocket;
class QTextCursor;
class QTextDocument;
class QTextEdit;
class QVBoxLayout;

class ClientWidget : public QWidget {
    Q_OBJECT
    
    public:
        ClientWidget(QWidget* parent=0);
        ~ClientWidget();
        void connectToHost() const;


    public slots:
        void connected();
        void readSocket();


    protected:
        ClientTextEdit* _textEdit;
        ClientLineEdit* _lineEdit;
        QTextCursor* _cursor;
        QTcpSocket* _socket;
        QVBoxLayout* _layout;


    signals:
        void receivedInput();
        void dataReceived(const QString& data);

};


#endif /* CLIENTWIDGET_H */
