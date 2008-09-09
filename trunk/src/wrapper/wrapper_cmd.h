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

#ifndef _WRAPPER_CMD_H
#define _WRAPPER_CMD_H

#include <QDialog>

extern "C" int wrapper_get_keybind(char *seq);

class Wrapper;

/*
class WrapperKeyBinder : public QDialog
{
  Q_OBJECT

  public:
    WrapperKeyBinder(char *, QObject* parent);
    virtual ~WrapperKeyBinder();
    
    char *seq;

    bool event(QEvent *event);
};
*/

#endif /* _WRAPPER_CMD_H */
