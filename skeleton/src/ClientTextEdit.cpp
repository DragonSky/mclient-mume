#include "ClientTextEdit.h"

#include <QDebug>
#include <QTextCursor>
#include <QTextDocument>


ClientTextEdit::ClientTextEdit(QWidget* parent) : QTextEdit(parent) {
    _doc = new QTextDocument(this);
    setDocument(_doc);
    
    _cursor = new QTextCursor(_doc);
    _format = _cursor->charFormat();
}


ClientTextEdit::~ClientTextEdit() {
}


void ClientTextEdit::displayText(const QString& str) {
    qDebug() << "Received text:" << str;
    //_doc->setPlainText(str);
    addText(str);
}


void ClientTextEdit::addText(const QString& str) {

  // ANSI codes are formatted as the following:
  // escape + [ + n1 (+ n2) + m
  QRegExp ansiRx("\\0033\\[((?:\\d+;)*\\d+)m");
  QRegExp subBlockRx("\\0010");
  QStringList blocks = str.split(ansiRx);
  QStringList ansi, subAnsi, subBlock;
  int i = 0, j, k;

  ansi << "";
  while ((i = ansiRx.indexIn(str, i)) != -1) {
    ansi << ansiRx.cap(1);
    i += ansiRx.matchedLength();
  }

  for (i = 0; i < blocks.count(); i++) {
    // split several semicoloned ansi codes into individual codes
    subAnsi = ansi[i].split(";"); 
    QStringListIterator ansiIterator(subAnsi);
    while (ansiIterator.hasNext() && i != 0)
      //updateFormat(format, ansiIterator.next().toInt()); //TODO format

    // split the text into sub-blocks
    blocks[i].replace((char)20, " "); // replace hex-spaces with normal spaces

    if ((k = blocks[i].indexOf(subBlockRx)) != -1) {
      j = 0;
      do {
        //qDebug("j%d k%d %s", j, k, blocks[i].toAscii().data());
        // this is for the "You begin to search.." etc lines
        if (blocks[i].length() > 2) { 
          _cursor->insertText(blocks[i].mid(j, k), _format);
          moveCursor(-1);
        } else {                     
            // HACK because the twiddler has a backspace following the 
            // twiddler character
          moveCursor(-1);
          _cursor->insertText(blocks[i].mid(j, k), _format);
        }
        j = k + 1;
      } while ((k = blocks[i].indexOf(subBlockRx, j)) != -1);
      _cursor->insertText(blocks[i].mid(j), _format);
    } else _cursor->insertText(blocks[i], _format);

    /*
    for (j = 0; (j = subBlockRx.indexIn(blocks[i], j)) != -1; j += subBlockRx.matchedLength()) {
    qDebug("%s", subBlock.join(",").toAscii().data());
      if (subBlockRx.cap(1).contains(QRegExp("\\0010"))) {
        cursor.insertText(blockIterator.next(), format);
        moveCursor(-1);
      }
      else {
        cursor.insertText(blockIterator.next(), format);
      }
    }
    */
  }
  setTextCursor(*_cursor);
  ensureCursorVisible();
}


void ClientTextEdit::moveCursor(const int& diff) {
    int col = _cursor->columnNumber();
    int pos = _cursor->position();
    QTextCursor::MoveOperation direction;

    if (diff > 0) {
        direction = QTextCursor::Right;
    
    } else if(diff < 0 && _cursor->columnNumber() != 0) {
        direction = QTextCursor::Left;
    
    } else return;

    _cursor->movePosition(direction, QTextCursor::KeepAnchor, abs(diff));
}

