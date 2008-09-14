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

#ifndef _WRAPPER_TTY_H
#define _WRAPPER_TTY_H

class Wrapper;

extern "C" void tty_putc(unsigned char c);
extern "C" void tty_printf(const char *format, ...);
extern "C" void tty_puts(const char *s);
extern "C" int tty_read(char *buf, int cnt);
extern "C" void tty_gets(char* s, int size);

extern "C" void tty_gotoxy(int col, int line);
extern "C" void tty_gotoxy_opt (int fromcol, int fromline, int tocol, int toline);

extern "C" void input_delete_nofollow_chars(int n);
extern "C" void input_overtype_follow(char c);
extern "C" void input_insert_follow_chars(char *str, int n);
extern "C" void input_moveto(int new_pos);

extern "C" void wrapper_input_set(char *str);
//extern "C" void wrapper_edit_input_insert_follow_chars(char *str);
//extern "C" void wrapper_set_input(QString contents, int position);

extern "C" void tty_add_walk_binds();
extern "C" void tty_add_initial_binds();

/* ------------ 
extern "C" void tty_gotoxy(int col, int line);
extern "C" void tty_gotoxy_opt(int fromcol, int fromline, int tocol, int toline);
extern "C" void tty_add_walk_binds();
extern "C" void tty_add_initial_binds();
*/

#endif /* _WRAPPER_TTY_H */
