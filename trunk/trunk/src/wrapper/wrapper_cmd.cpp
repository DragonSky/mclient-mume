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

#include "wrapper_cmd.h"
#include "wrapper.h"

int wrapper_get_keybind(char *seq) { }

/*
int wrapper_get_keybind(char *seq) { return wrapper->getKeyBind(seq); }

int Wrapper::getKeyBind(char *seq) {
  WrapperKeyBinder *dlg = new WrapperKeyBinder(seq, m_parent);
  dlg->exec();
  delete dlg;
  //seq = (const char*)key->toAscii();
  qDebug("%s %d", seq, strlen(seq));
  return strlen(seq);
}

/
WrapperKeyBinder::WrapperKeyBinder(char *s, QObject *parent) : QDialog(parent)
{
  setWindowTitle("Key Binder");
  seq = s;
}

WrapperKeyBinder::~WrapperKeyBinder() {}

bool WrapperKeyBinder::event(QEvent *event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *ke = static_cast<QKeyEvent *>(event);

    int keycode = ke->key();
    char *seq2 = (char*)malloc(20);
    sprintf(seq2, "%c", keycode);
  //qDebug("before %d", strlen(seq2));

    int len = 0;
    if (seq2[0] >= ' ' && seq2[0] <= '~') {
      if (ke->modifiers() == Qt::ControlModifier) seq2[len++] = 27;
      seq2[len++] = (char)keycode+80;
    }
    else
      len = strlen(seq2);
    strcpy(seq, seq2);

    char ch;
    QString out, tmp;
    while (len--) {
      ch = *(seq2++);
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
    qDebug("%s (%d)", (const char*)out.toAscii(), keycode);
    
    
    accept();
    hide();
    return true;
  }
  return QWidget::event(event);
}
*/
