/***************************************************************************
 *   Copyright (C) 2008 by Nils Schimmelmann   *
 *   nschimme@gmail.com   *
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

#include <QKeyEvent>
#include <QTextStream>

#include "keybinder.h"

bool isAcceptableKey(QKeyEvent* ke) {
  switch (ke->modifiers()) {

    case Qt::NoModifier:
      switch (ke->key()) {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Tab:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
        case Qt::Key_F12:
        case Qt::Key_F11:
        case Qt::Key_F10:
        case Qt::Key_F9:
        case Qt::Key_F8:
        case Qt::Key_F7:
        case Qt::Key_F6:
        case Qt::Key_F5:
        case Qt::Key_F4:
        case Qt::Key_F3:
        case Qt::Key_F2:
        case Qt::Key_F1:
        case Qt::Key_Delete:
        case Qt::Key_Home:
        case Qt::Key_Insert:
          return true;
      };
      break;

    case Qt::KeypadModifier:
      switch (ke->key()) {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Clear:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_Delete:
        case Qt::Key_Plus:
        case Qt::Key_Minus:
        case Qt::Key_Slash:
        case Qt::Key_Asterisk:
        case Qt::Key_Insert:
        case Qt::Key_Enter:
        case Qt::Key_Return:
          return true;
      };
      break;

    case Qt::ControlModifier: if (ke->key() == Qt::Key_Control) break;
    case Qt::AltModifier: if (ke->key() == Qt::Key_Alt) break;
    case Qt::MetaModifier: if (ke->key() == Qt::Key_Meta) break;
    case Qt::ShiftModifier: if (ke->key() == Qt::Key_Shift) break;

    case Qt::ShiftModifier ^ Qt::KeypadModifier:
    case Qt::ShiftModifier ^ Qt::ControlModifier:
    case Qt::ShiftModifier ^ Qt::AltModifier:
    case Qt::ShiftModifier ^ Qt::MetaModifier:
    case Qt::ControlModifier ^ Qt::KeypadModifier:
    case Qt::ControlModifier ^ Qt::MetaModifier:
    case Qt::ControlModifier ^ Qt::AltModifier:
    case Qt::MetaModifier ^ Qt::KeypadModifier:
    case Qt::MetaModifier ^ Qt::AltModifier:
    case Qt::AltModifier ^ Qt::KeypadModifier:

    case Qt::ControlModifier ^ Qt::MetaModifier ^ Qt::KeypadModifier:
    case Qt::ControlModifier ^ Qt::MetaModifier ^ Qt::AltModifier:
    case Qt::ControlModifier ^ Qt::MetaModifier ^ Qt::ShiftModifier:
    case Qt::ControlModifier ^ Qt::KeypadModifier ^ Qt::ShiftModifier:
    case Qt::AltModifier ^ Qt::ControlModifier ^ Qt::KeypadModifier:
    case Qt::AltModifier ^ Qt::ControlModifier ^ Qt::ShiftModifier:
    case Qt::AltModifier ^ Qt::KeypadModifier ^ Qt::ShiftModifier:
    case Qt::MetaModifier ^ Qt::AltModifier ^ Qt::KeypadModifier:
    case Qt::MetaModifier ^ Qt::KeypadModifier ^ Qt::ShiftModifier:
    case Qt::MetaModifier ^ Qt::AltModifier ^ Qt::ShiftModifier:

    case Qt::ControlModifier ^ Qt::AltModifier ^ Qt::MetaModifier ^ Qt::KeypadModifier:
    case Qt::ControlModifier ^ Qt::MetaModifier ^ Qt::KeypadModifier ^ Qt::ShiftModifier:
    case Qt::AltModifier ^ Qt::ControlModifier ^ Qt::KeypadModifier ^ Qt::ShiftModifier:
    case Qt::MetaModifier ^ Qt::AltModifier ^ Qt::KeypadModifier ^ Qt::ShiftModifier:
    case Qt::ControlModifier ^ Qt::AltModifier ^ Qt::MetaModifier ^ Qt::ShiftModifier:

    case Qt::ControlModifier ^ Qt::AltModifier ^ Qt::MetaModifier ^ Qt::KeypadModifier ^ Qt::ShiftModifier:
      return true;

  };
  return false;
}

QString generateKeyCodeSequence(QKeyEvent *ke) {
  char *seq = (char*)malloc(20);
  int len = 0, key = ke->key();

  const Qt::KeyboardModifiers modifiers = ke->modifiers();

  if (key > Qt::Key_Escape) 
  {
    seq[len++] = 27;
    seq[len++] = '[';
    key = ke->key() % 0x01000000;
  }

  if (modifiers & Qt::ControlModifier) seq[len++] = Qt::Key_Control;
  if (modifiers & Qt::AltModifier) seq[len++] = Qt::Key_Alt;
  if (modifiers & Qt::MetaModifier) seq[len++] = Qt::Key_Meta;
  if (modifiers & Qt::ShiftModifier) seq[len++] = Qt::Key_Shift;
  if (modifiers & Qt::KeypadModifier) seq[len++] = Qt::Key_NumLock;
  seq[len++] = (char)key;
  seq[len] = '\0';

  QString keyResult(seq);

  int cnt = 0;
  char ch;
  QString out, tmp, ls;
  while (len--) {
    ls += QString("%1, ").arg((int)keyResult.at(cnt++).toAscii());
    ch = *(seq++);
    if (ch == '\033') {
      out += "esc ";
      continue;
    }
    if (ch < ' ') {
      out += '^';
      ch |= '@';
    }
    if (ch == ' ')
      out += "space ";
    else if (ch == 0x7f)
      out += "del ";
    else if (ch & 0x80)
      out += tmp.sprintf("\\%03o ", ch);
    else
      out += tmp.sprintf("%c ", ch);
  }
  qDebug("%s (%s) (%s/%d)", out.toAscii().constData(), ls.toAscii().constData(), ke->text().toAscii().data(), ke->key());

  return keyResult;
}

QString generateSequenceText(QString sequence) {
  char *seq = (char*)malloc(20);
  sprintf(seq, "%s", sequence.toAscii().data());
  int len = strlen(seq);

  // skip first escape
  *(seq++); len--;
  char ch;
  QString out, tmp;
  while (len--) {
    ch = *(seq++);
    if (ch == '\033') {
      out += "esc ";
      continue;
    }
    if (ch < ' ') {
      out += '^';
      ch |= '@';
    }
    if (ch == '!') {out += "ctrl "; continue; }
    else if (ch == '#') {out += "alt ";  continue; }
    else if (ch == '\"') {out += "meta ";  continue; }
    else if (ch == '%') {out += "KP ";  continue; }
    if (ch == ' ')
      out += "space ";
    else if (ch == 0x7f)
      out += "del ";
    else if (ch & 0x80)
      out += tmp.sprintf("\\%03o ", ch);
    else
      out += tmp.sprintf("%c ", ch);
  }

  return out;
}

bool KeyBinder::event(QEvent *event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *ke = static_cast<QKeyEvent *>(event);
    if (isAcceptableKey(ke)) {
      QString result = generateKeyCodeSequence(ke);
      //qDebug("got keycode %d (%s)", ke->key(), result.toAscii().constData());
      keyCode->setText(generateSequenceText(result));
      seq->clear();
      QTextStream(seq) << result;
      return true;
    }
  }
  return QWidget::event(event);
}

/* Objects */

KeyBinder::KeyBinder(QString label, QString &s, QWidget*parent) : QDialog(parent)
{
  setupUi(this);
  keyCode->setFocusProxy(this);
  setFocus();

  seq = &s;
  if (!seq->isEmpty()) keyCode->setText(generateSequenceText(*seq));
  keyLabel->setText(QString("\"%1\"").arg(label));
}

KeyBinder::~KeyBinder() {}

