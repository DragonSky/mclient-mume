#ifndef CLIENTTEXTEDIT_H
#define CLIENTTEXTEDIT_H

#include <QTextEdit>


class QTextDocument;

class ClientTextEdit : public QTextEdit {
    Q_OBJECT

    public:
        ClientTextEdit(QWidget* parent=0);
        ~ClientTextEdit();
        void addText(const QString& str);
    
    public slots:
        void displayText(const QString& str);
        void moveCursor(const int& diff);


    protected:
        QTextCursor* _cursor;
        QTextDocument* _doc;
        QTextCharFormat _format;

};

#endif /* CLIENTTEXTEDIT_H */
