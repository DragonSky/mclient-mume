#ifndef CLIENTTEXTEDIT_H
#define CLIENTTEXTEDIT_H

#include <QTextEdit>


class QTextDocument;

class ClientTextEdit : public QTextEdit {
    Q_OBJECT

    public:
        ClientTextEdit(QWidget* parent=0);
        ~ClientTextEdit();
    
    public slots:
        void displayText(const QString& str);


    protected:
        QTextCursor* _cursor;
        QTextDocument* _doc;

};

#endif /* CLIENTTEXTEDIT_H */
