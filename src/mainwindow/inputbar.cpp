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

void InputBar::emitFakeKey(QKeyEvent *key) {
  KeyBinder *kb = new KeyBinder();
  QString keyResult = kb->generateKeyCodeSequence(key);
  
  emit keyPressed(keyResult);
}

bool InputBar::event(QEvent *event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *ke = static_cast<QKeyEvent *>(event);
    switch (ke->modifiers()) {

      case Qt::NoModifier:
        switch (ke->key()) {           /* Keypad Section */
          case Qt::Key_Up:
            getPreviousLine();
            return true;
          case Qt::Key_Down:
            getNextLine();
            return true;
          case Qt::Key_Tab:
            completeWord();
            return true;
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
            emitFakeKey(ke);
            return true;
        };
        break;

      case Qt::ControlModifier:
        switch (ke->key()) {
          case Qt::Key_A:              // Interferes with select all
            home(false);
            return true;
          case Qt::Key_B:
            cursorBackward(false);
            return true;
          case Qt::Key_D:
            del();
            return true;
          case Qt::Key_E:
            end(false);
            return true;
          case Qt::Key_F:
            cursorForward(false);
            return true;
          case Qt::Key_N:
            getNextLine();
            return true;
          case Qt::Key_P:
            getPreviousLine();
            return true;
          case Qt::Key_T:
            transposeCharacters();
            return true;
          case Qt::Key_W:
            put_history(text().toAscii().data());
            selectAll();
            return true;
          case Qt::Key_Z:
            emit minimizeParent();
            return true;
          case Qt::Key_Insert:        // Skip keys that do stuff as standard Qt shortcuts
          case Qt::Key_Delete:
          case Qt::Key_Left:
          case Qt::Key_Right:
            return QWidget::event(event);
        };
      case Qt::ControlModifier ^ Qt::KeypadModifier:
        switch (ke->key()) {
          case Qt::Key_Left:
          case Qt::Key_Right:
          case Qt::Key_Up:
          case Qt::Key_Down:
          case Qt::Key_Home:
          case Qt::Key_End:
          case Qt::Key_Delete:
          case Qt::Key_Plus:
          case Qt::Key_Minus:
          case Qt::Key_Clear:
          case Qt::Key_Slash:
          case Qt::Key_Asterisk:
          case Qt::Key_Insert:
          case Qt::Key_PageUp:
          case Qt::Key_PageDown:
          case Qt::Key_ScrollLock:
          case Qt::Key_Pause:
          case Qt::Key_Print:
          case Qt::Key_Escape:
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
          case Qt::Key_Tab:
          case Qt::Key_Meta:
          case Qt::Key_Enter:
          case Qt::Key_Return:
            emitFakeKey(ke);
            return true;
        };
        break;

      case Qt::AltModifier:
        switch (ke->key()) {
          case Qt::Key_Tab:
            completeLine();
            return true;
          case Qt::Key_B:
            cursorWordBackward(false);
            return true;
          case Qt::Key_D:
            cursorWordForward(true);
            del();
            return true;
          case Qt::Key_F:
            cursorWordForward(false);
            return true;
          case Qt::Key_T:
            transposeWords();
            return true;
          case Qt::Key_U:
            uppercaseWord();
            return true;
          case Qt::Key_L:
            lowercaseWord();
            return true;
          case Qt::Key_Backspace:
          case Qt::Key_Delete:
            cursorWordBackward(true);
            del();
            return true;
        };
      case Qt::MetaModifier:
        switch (ke->key()) {
          case Qt::Key_Tab:
            completeLine();
            return true;
        };
      case Qt::AltModifier ^ Qt::KeypadModifier:
      case Qt::MetaModifier ^ Qt::KeypadModifier:
        switch (ke->key()) {
          case Qt::Key_Left:
          case Qt::Key_Right:
          case Qt::Key_Up:
          case Qt::Key_Down:
          case Qt::Key_Home:
          case Qt::Key_End:
          case Qt::Key_Delete:
          case Qt::Key_Plus:
          case Qt::Key_Minus:
          case Qt::Key_Clear:
          case Qt::Key_Slash:
          case Qt::Key_Asterisk:
          case Qt::Key_Insert:
          case Qt::Key_PageUp:
          case Qt::Key_PageDown:
          case Qt::Key_ScrollLock:
          case Qt::Key_Pause:
          case Qt::Key_Print:
          case Qt::Key_Escape:
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
          case Qt::Key_Tab:
          case Qt::Key_Meta:
          case Qt::Key_Enter:
          case Qt::Key_Return:
            emitFakeKey(ke);
            return true;
        };
        break;

      case Qt::KeypadModifier:
        switch (ke->key()) {
          case Qt::Key_Up:
            emit keyPressed("\033Ox");
            return true;
          case Qt::Key_Down:
            emit keyPressed("\033Or");
            return true;
          case Qt::Key_Left:
            emit keyPressed("\033Ot");
            return true;
          case Qt::Key_Right:
            emit keyPressed("\033Ov");
            return true;
          case Qt::Key_Clear:
            emit keyPressed("\033Ou");
            return true;
          case Qt::Key_PageUp:
            emit keyPressed("\033Oy");
            return true;
          case Qt::Key_PageDown:
            emit keyPressed("\033Os");
            return true;
          case Qt::Key_Home:
            emit keyPressed("\033Ow");
            return true;
          case Qt::Key_End:
          case Qt::Key_Delete:
          case Qt::Key_Plus:
          case Qt::Key_Minus:
          case Qt::Key_Slash:
          case Qt::Key_Asterisk:
          case Qt::Key_Insert:
          //case Qt::Key_Enter:
          //case Qt::Key_Return:
            emitFakeKey(ke);
            return true;
        };
        break;
    };
  }
  return QWidget::event(event);
}

void InputBar::inputMoveTo(int new_pos) {
  //setSelection(new_pos, cursorPosition() - new_pos);
  setCursorPosition(new_pos);
}

void InputBar::inputInsertText(QString input) {
  qDebug("insertText '%s'", input.toAscii().data());
  setSelection(cursorPosition(), input.length());
  insert(input);
}

void InputBar::inputDeleteChars(int num) {
  setSelection(cursorPosition(), num);
  del();
}

void InputBar::transposeCharacters() {
  last_edit_cmd = (function_any)transpose_chars;

  if (cursorPosition() < 1 || text().length() < 2)
    return;

  if (cursorPosition() <= text().length()-1) setSelection(cursorPosition()-1, 2);
  else setSelection(cursorPosition()-2, 2);

  insert(QString("%2%1").arg(selectedText().at(0)).arg(selectedText().at(1)));
}

void InputBar::transposeWords() {
  emit mergeInputWrapper(text(), cursorPosition());
  transpose_words("dummy");
  last_edit_cmd = (function_any)transpose_words;
  /*
  if (text().left(cursorPosition()+1).count(QRegExp("\\b")) <= 2)
    return;

  int oldPosition = cursorPosition();
  cursorWordForward(false);
  cursorWordBackward(true);
  cursorWordBackward(true);

  QRegExp rx("^(.+)\\s+(.+)$");
  if (rx.exactMatch(selectedText().trimmed()) ) {
    QStringList list = rx.capturedTexts();
    list.removeAt(0);
    list.swap(0, 1);
    insert(list.join(" "));
    if (cursorPosition() != text().length()) {
      insert(" ");
      cursorBackward(false);
    }
  } else {
    qDebug("No regexp match for words... unable to transpose!");
    deselect();
    setCursorPosition(oldPosition);
  }
  */
}

void InputBar::getNextLine() {
  emit mergeInputWrapper(text(), cursorPosition());
  next_line("dummy");
  if (hist[pickline]) {
    setText(edbuf);
    selectAll();
  }
  last_edit_cmd = (function_any)next_line;
}

void InputBar::getPreviousLine() {
  emit mergeInputWrapper(text(), cursorPosition());
  prev_line("dummy");
  if (hist[pickline]) {
    setText(edbuf);
    selectAll();
  }
  last_edit_cmd = (function_any)prev_line;
}

void InputBar::completeWord() {
  emit mergeInputWrapper(text(), cursorPosition());
  complete_word("dummy");
  last_edit_cmd = (function_any)complete_word;
}

void InputBar::completeLine() {
  emit mergeInputWrapper(text(), cursorPosition());
  complete_line("dummy");
  setText(edbuf);
  last_edit_cmd = (function_any)complete_line;
}

void InputBar::uppercaseWord() {
  emit mergeInputWrapper(text(), cursorPosition());
  upcase_word("dummy");
  last_edit_cmd = (function_any)upcase_word;
  /*
  int oldPosition = cursorPosition();
  cursorWordBackward(false);
  cursorWordForward(true);
  int upperCount = 0;
  for (upperCount = 0; upperCount < selectedText().length(); upperCount++) {
    if (!isupper(selectedText().at(upperCount).toAscii()))
      break;
    else upperCount++;
  }
  if (upperCount == selectedText().length()) selectAll();
  insert(selectedText().toUpper());
  setCursorPosition(oldPosition);
  */
}

void InputBar::lowercaseWord() {
  emit mergeInputWrapper(text(), cursorPosition());
  downcase_word("dummy");
  last_edit_cmd = (function_any)downcase_word;
  /*
  int oldPosition = cursorPosition();
  cursorWordBackward(false);
  cursorWordForward(true);
  int lowerCount = 0;
  for (lowerCount = 0; lowerCount < selectedText().length(); lowerCount++) {
    if (!islower(selectedText().at(lowerCount).toAscii()))
      break;
  }
  if (lowerCount == selectedText().length()) selectAll();
  insert(selectedText().toLower());
  setCursorPosition(oldPosition);
  */
}

