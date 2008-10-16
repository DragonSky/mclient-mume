#include "ClientTextEdit.h"

#include <QTextCursor>
#include <QTextDocument>


ClientTextEdit::ClientTextEdit(QWidget* parent) : QTextEdit(parent) {
    _doc = new QTextDocument(this);
    setDocument(_doc);
    
    _cursor = new QTextCursor(_doc);
}


ClientTextEdit::~ClientTextEdit() {
}


void ClientTextEdit::displayText(const QString& str) {
    _doc->setPlainText(str);
}
