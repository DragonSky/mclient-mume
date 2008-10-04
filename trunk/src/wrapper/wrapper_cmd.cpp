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

#include <limits.h>
#include "wrapper_cmd.h"
#include "wrapper.h"

#include "keybinder.h"

void suspend_powwow() {}

int wrapper_get_keybind(char *seq) { return Wrapper::self()->getKeyBind(seq); }

int Wrapper::getKeyBind(char *seq) {
  QString label("Blank"), sequence;
  KeyBinder *dlg = new KeyBinder(label, sequence, (QWidget*)parent);
  if (dlg->exec()) {
    strcpy(seq, sequence.toAscii().constData());
  }
  delete dlg;
  return strlen(seq);
}

