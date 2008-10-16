#include "ClientLineEdit.h"


ClientLineEdit::ClientLineEdit(QWidget* parent) : QLineEdit(parent) {
    setMaxLength(255); //BUFSIZE-1

}


ClientLineEdit::~ClientLineEdit() {
}
