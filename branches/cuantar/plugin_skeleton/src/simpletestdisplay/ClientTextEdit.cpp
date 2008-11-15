#include "ClientTextEdit.h"

#include <QDebug>
#include <QScrollBar>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextCharFormat>

ClientTextEdit::ClientTextEdit(QWidget* parent) : QTextEdit(parent) {
    setReadOnly(true);
    setOverwriteMode(true);
    setUndoRedoEnabled(false);
    setDocumentTitle("mClient");
    setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    setTabChangesFocus(false);
    _doc = new QTextDocument(this);
    //_doc->setMaximumBlockCount(Config().scrollbackSize); // max number of lines?
    _doc->setUndoRedoEnabled(false);
    setDocument(_doc);
    QTextFrame* frame = _doc->rootFrame();
    _cursor = frame->firstCursorPosition();

    QTextFrameFormat frameFormat = frame->frameFormat();
    frameFormat.setBackground(Qt::black);
    frameFormat.setForeground(Qt::lightGray);
    frame->setFrameFormat(frameFormat);

    _format = _cursor.charFormat();
    _format.setFont(QFont("Monospace", 8));
    //_format.setFont(Config().serverOutputFont);
    _format.setBackground(Qt::black);
    _format.setForeground(Qt::lightGray);
    _cursor.setCharFormat(_format);

    /*
    QFontMetrics fm(Config().serverOutputFont);
    setTabStopWidth(fm.width(" ") * 8); // A tab is 8 spaces wide
    */
    QFontMetrics fm(font());
    QScrollBar* scrollbar = verticalScrollBar();
    scrollbar->setSingleStep(fm.leading()+fm.height());

    connect(scrollbar, SIGNAL(sliderReleased()), 
                this, SLOT(scrollBarReleased()));
}


ClientTextEdit::~ClientTextEdit() {
}


void ClientTextEdit::displayText(const QString& str) {
    //qDebug() << "Received text:" << str;
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
    /*
    // HACK: ANSI support not included yet.
    while (ansiIterator.hasNext() && i != 0)
      updateFormat(_format, ansiIterator.next().toInt());
    */

    // split the text into sub-blocks
    blocks[i].replace((char)20, " "); // replace hex-spaces with normal spaces

    if ((k = blocks[i].indexOf(subBlockRx)) != -1) {
      j = 0;
      do {
        //qDebug("j%d k%d %s", j, k, blocks[i].toAscii().data());
        if (blocks[i].length() > 2) { // this is for the "You begin to search.." etc lines
          _cursor.insertText(blocks[i].mid(j, k), _format);
          moveCursor(-1);
        } else {                     // HACK because the twiddler has a backspace following the twiddler character
          moveCursor(-1);
          _cursor.insertText(blocks[i].mid(j, k), _format);
        }
        j = k + 1;
      } while ((k = blocks[i].indexOf(subBlockRx, j)) != -1);
      _cursor.insertText(blocks[i].mid(j), _format);
    } else _cursor.insertText(blocks[i], _format);

    /*
    for (j = 0; (j = subBlockRx.indexIn(blocks[i], j)) != -1; j += subBlockRx.matchedLength()) {
    qDebug("%s", subBlock.join(",").toAscii().data());
      if (subBlockRx.cap(1).contains(QRegExp("\\0010"))) {
        _cursor.insertText(blockIterator.next(), _format);
        moveCursor(-1);
      }
      else {
        _cursor.insertText(blockIterator.next(), _format);
      }
    }
    */
  }
  setTextCursor(_cursor);
  ensureCursorVisible();
}


void ClientTextEdit::moveCursor(const int& diff) {
    int col = _cursor.columnNumber();
    int pos = _cursor.position();
    QTextCursor::MoveOperation direction;

    if (diff > 0) {
        direction = QTextCursor::Right;
    
    } else if(diff < 0 && _cursor.columnNumber() != 0) {
        direction = QTextCursor::Left;
    
    } else return;

    _cursor.movePosition(direction, QTextCursor::KeepAnchor, abs(diff));
}


void ClientTextEdit::scrollBarReleased() {
//    if(action == QAbstractSlider::SliderReleased) {
        QScrollBar* sb = verticalScrollBar();
        int val = sb->value()+sb->pageStep();
        int singleStep = sb->singleStep();
        qDebug() << "* singleStep is:" << singleStep;
        int docSize = sb->maximum() - sb->minimum() + sb->pageStep();
        if(val%singleStep != 0) {
            qDebug() << "* val is:" << val;
            // here's what's left over
            int newVal = val/singleStep;
            int pixels = newVal*singleStep;
            qDebug() << "* newVal is:" << newVal;
            if(val-pixels < singleStep/2) {
                val = pixels;
            } else {
                val = pixels + singleStep;
            }
        }
        sb->setSliderPosition(val-sb->pageStep());
        qDebug() << "* set slider position to" << val;
//    }
}
