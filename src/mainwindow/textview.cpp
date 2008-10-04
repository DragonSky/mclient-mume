/***************************************************************************
 *   Copyright (C) 2008 by Nils Schimmelmann                               *
 *   nschimme@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/

#include "textview.h"
#include "configuration.h"

#include <QTextFrame>
#include <QTextCharFormat>
#include <QFont>
#include <QBrush>

#include <QEvent>
#include <QKeyEvent>
#include <QTextCursor>

#include "main.h"
#include "tcp.h"

TextView::TextView(QLineEdit *lineEdit, QWidget *parent) : QTextEdit(parent)
{
  qRegisterMetaType<QTextCursor>("QTextCursor");
  qRegisterMetaType<QString>("QString&");

  setReadOnly(true);
  setOverwriteMode(true);
  setUndoRedoEnabled(false);
  setDocumentTitle("mClient");
  setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
  setTabChangesFocus(false);
  setFocusProxy(lineEdit);
  doc = new QTextDocument(this);
  doc->setMaximumBlockCount(Config().scrollbackSize); // max number of lines?
  doc->setUndoRedoEnabled(false);
  setDocument(doc);
  QTextFrame* frame = doc->rootFrame();
  cursor = frame->firstCursorPosition();

  QTextFrameFormat frameFormat = frame->frameFormat();
  frameFormat.setBackground(Config().backgroundColor);
  frameFormat.setForeground(Config().foregroundColor);
  frame->setFrameFormat(frameFormat);

  format = cursor.charFormat();
  setDefaultFormat(format);
  defaultFormat = format;
  cursor.setCharFormat(format);

  QFontMetrics fm(Config().serverOutputFont);
  setTabStopWidth(fm.width(" ") * 8); // A tab is 8 spaces wide

  connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(autoCopySelectedText(bool)));

  previous = 0;
  qDebug("TextView created.");
}

/*
 * Calculate Visible View Dimensions
 * HACK: This doesn't work perfectly...
 */
void TextView::viewDimensionsChanged() {
  /*
  int fontWidth, fontHeight;
  QFontMetrics fm(Config().serverOutputFont);
  fontWidth = fm.width(" "); // How many pixels wide is this text?
  fontHeight = fm.height();  // How many pixels tall is this font?
  */
  lines = maximumViewportSize().height();
  cols_1 = cols = maximumViewportSize().width();

  qDebug("Viewport: Lines: %d, Width: %d", lines, cols);

  if (tcp_main_fd != -1)
    tcp_write_tty_size();
}

void TextView::moveCursor(int diff) {
  int col = cursor.columnNumber();
  int pos = cursor.position();
  QTextCursor::MoveOperation direction;

  //qDebug("Internal: Column %d, Position %d, Diff %d", col, pos, diff);

  if (diff > 0) direction = QTextCursor::Right;
  else if (diff < 0 && cursor.columnNumber() != 0) direction = QTextCursor::Left;
  else return;

  cursor.movePosition(direction, QTextCursor::KeepAnchor, abs(diff));
  //qDebug("Post move: Column %d, Position %d", cursor.columnNumber(), cursor.position());
}

void TextView::autoCopySelectedText(bool yes)
{
  if (yes)
    copy();
}

void TextView::addText(QString& str) // TODO: Make this a state machine
{
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
      updateFormat(format, ansiIterator.next().toInt());

    // split the text into sub-blocks
    blocks[i].replace((char)20, " "); // replace hex-spaces with normal spaces

    if ((k = blocks[i].indexOf(subBlockRx)) != -1) {
      j = 0;
      do {
        //qDebug("j%d k%d %s", j, k, blocks[i].toAscii().data());
        if (blocks[i].length() > 2) { // this is for the "You begin to search.." etc lines
          cursor.insertText(blocks[i].mid(j, k), format);
          moveCursor(-1);
        } else {                     // HACK because the twiddler has a backspace following the twiddler character
          moveCursor(-1);
          cursor.insertText(blocks[i].mid(j, k), format);
        }
        j = k + 1;
      } while ((k = blocks[i].indexOf(subBlockRx, j)) != -1);
      cursor.insertText(blocks[i].mid(j), format);
    } else cursor.insertText(blocks[i], format);

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
  setTextCursor(cursor);
  ensureCursorVisible();
}

void TextView::setDefaultFormat(QTextCharFormat& format)
{
  /*
  QFont font = format.font();
  font.setFamily("Bitsream Vera Sans Mono");
  font.setFixedPitch(true);
  font.setStyle(QFont::StyleNormal);
  */

  format.setFont(Config().serverOutputFont);
  format.setBackground(Config().backgroundColor);
  format.setForeground(Config().foregroundColor);
}

void TextView::updateFormat(QTextCharFormat& format, int ansiCode)
{
  /*
  if (ansiCode != previous)
    qDebug("new code: %d", ansiCode);
  else
    qDebug("repeat");
  */
  previous = ansiCode;
  QBrush tempBrush;
  switch (ansiCode) {
    case 0:
      // turn ANSI off (i.e. return to normal defaults)
      format = defaultFormat;
      //format.clearBackground();
      //format.clearForeground();
      break;
    case 1:
      // bold
      format.setFontWeight(QFont::Bold);
      updateFormatBoldColor(format);
      break;
    case 4:
      // underline
      format.setFontUnderline(true);
      break;
    case 5:
      // blink
      // TODO
      break;
    case 7:
      // inverse
      tempBrush = format.background();
      format.setBackground(format.foreground());
      format.setForeground(tempBrush);
      break;
    case 8:
      // strike-through
      format.setFontStrikeOut(true);
      break;
    case 30:
      // black foreground
      format.setForeground(Config().blackColor);
      break;
    case 31:
      // red foreground
      format.setForeground(Config().redColor);
      break;
    case 32:
      // green foreground
      format.setForeground(Config().greenColor);
      break;
    case 33:
      // yellow foreground
      format.setForeground(Config().yellowColor);
      break;
    case 34:
      // blue foreground
      format.setForeground(Config().blueColor);
      break;
    case 35:
      // magenta foreground
      format.setForeground(Config().magentaColor);
      break;
    case 36:
      // cyan foreground
      format.setForeground(Config().cyanColor);
      break;
    case 37:
      // gray foreground
      format.setForeground(Config().grayColor);
      break;
    case 40:
      // black background
      format.setBackground(Config().blackColor);
      break;
    case 41:
      // red background
      format.setBackground(Config().redColor);
      break;
    case 42:
      // green background
      format.setBackground(Config().greenColor);
      break;
    case 43:
      // yellow background
      format.setBackground(Config().yellowColor);
      break;
    case 44:
      // blue background
      format.setBackground(Config().blueColor);
      break;
    case 45:
      // magenta background
      format.setBackground(Config().magentaColor);
      break;
    case 46:
      // cyan background
      format.setBackground(Config().cyanColor);
      break;
    case 47:
      // gray background
      format.setBackground(Config().grayColor);
      break;
    case 90:
      // high-black foreground
      format.setForeground(Config().darkGrayColor);
      break;
    case 91:
      // high-red foreground
      format.setForeground(Config().brightRedColor);
      break;
    case 92:
      // high-green foreground
      format.setForeground(Config().brightGreenColor);
      break;
    case 93:
      // high-yellow foreground
      format.setForeground(Config().brightYellowColor);
      break;
    case 94:
      // high-blue foreground
      format.setForeground(Config().brightBlueColor);
      break;
    case 95:
      // high-magenta foreground
      format.setForeground(Config().brightMagentaColor);
      break;
    case 96:
      // high-cyan foreground
      format.setForeground(Config().brightCyanColor);
      break;
    case 97:
      // high-white foreground
      format.setForeground(Config().whiteColor);
      break;
    case 100:
      // high-black background
      format.setBackground(Config().darkGrayColor);
      break;
    case 101:
      // high-red background
      format.setBackground(Config().brightRedColor);
      break;
    case 102:
      // high-green background
      format.setBackground(Config().brightGreenColor);
      break;
    case 103:
      // high-yellow background
      format.setBackground(Config().brightYellowColor);
      break;
    case 104:
      // high-blue background
      format.setBackground(Config().brightBlueColor);
      break;
    case 105:
      // high-magenta background
      format.setBackground(Config().brightMagentaColor);
      break;
    case 106:
      // high-cyan background
      format.setBackground(Config().brightCyanColor);
      break;
    case 107:
      // high-white background
      format.setBackground(Config().whiteColor);
      break;
    default:
      qDebug("Unknown!");
      format.setBackground(Qt::gray);
  };
}

void TextView::updateFormatBoldColor(QTextCharFormat& format) {
  if (format.foreground() == Config().blackColor)
    format.setForeground(Config().darkGrayColor);
  else if (format.foreground() == Config().redColor)
    format.setForeground(Config().brightRedColor);
  else if (format.foreground() == Config().greenColor)
    format.setForeground(Config().brightGreenColor);
  else if (format.foreground() == Config().yellowColor)
    format.setForeground(Config().brightYellowColor);
  else if (format.foreground() == Config().blueColor)
    format.setForeground(Config().brightBlueColor);
  else if (format.foreground() == Config().magentaColor)
    format.setForeground(Config().brightMagentaColor);
  else if (format.foreground() == Config().cyanColor)
    format.setForeground(Config().brightCyanColor);
  else if (format.foreground() == Config().grayColor)
    format.setForeground(Config().whiteColor);
}

