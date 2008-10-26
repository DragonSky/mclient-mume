#ifndef CLIENTLINEEDIT_H
#define CLIENTLINEEDIT_H

#include <QLineEdit>

class ClientLineEdit : public QLineEdit {
    Q_OBJECT

    public:
        ClientLineEdit(QWidget* parent=0);
        ~ClientLineEdit();

};

#endif /* CLIENTLINEEDIT_H */
