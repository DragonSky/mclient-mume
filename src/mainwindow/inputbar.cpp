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

#include "inputbar.h"

#include <QKeyEvent>

#include "main.h" // for edbuf
//#include "defines.h" // helper to edit.h
#include "edit.h" // needed for history

#include "wrapper.h"
#include "wrapper_tty.h"

#include "keybinder.h"

InputBar::InputBar(QWidget *parent) : QLineEdit(parent)
{
  setMaxLength(BUFSIZE-1);
}

void InputBar::toggleEchoMode() {
  if (echoMode() == QLineEdit::Normal) setEchoMode(QLineEdit::Password);
  else {
    clear();
    setEchoMode(QLineEdit::Normal);
  }
}

bool InputBar::emitFakeKey(QKeyEvent *ke) {
  QString input = generateKeyCodeSequence(ke);
  keynode *p;

  for (p = keydefs; (p && (p->seqlen < input.length() || 
       memcmp(input.toAscii().constData(), p->sequence, input.length()))) ;
       p = p->next);

  if (!p) {
    // GH: type the first character and keep processing the rest in the input buffer
    last_edit_cmd = (function_any)0;
  }
  else if (p->seqlen == input.length()) {
    emit mergeInputWrapper(text(), cursorPosition());
    emit keyPressed(input);
    //p->funct(p->call_data);
    //last_edit_cmd = (function_any)p->funct; // GH: keep track of last command
    return true;
  }
  return false;
}

bool InputBar::event(QEvent *event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *ke = static_cast<QKeyEvent *>(event);
    if (isAcceptableKey(ke) && emitFakeKey(ke)) return true;
  }
  return QWidget::event(event);
}

void InputBar::inputMoveTo(int new_pos) {
  //setSelection(new_pos, cursorPosition() - new_pos);
  setCursorPosition(new_pos);
}

void InputBar::inputInsertText(QString input) {
  setSelection(cursorPosition(), input.length());
  insert(input);
}

void InputBar::inputDeleteChars(int num) {
  setSelection(cursorPosition(), num);
  del();
}

void InputBar::inputClear() {
  clear();
}

