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

#include "wrapper.h"
#include "wrapper_tty.h"

//#include "defines.h"
#include "edit.h" //
#include "list.h" //add_keynode
#include "main.h"
#include "utils.h"
//#include "tcp.h"

#ifndef _WIN32
#include <alloca.h>
#endif

#include <QString>

static char kpadstart[CAPLEN], kpadend[CAPLEN],
        leftcur[CAPLEN], rightcur[CAPLEN], upcur[CAPLEN], curgoto[CAPLEN],
        delchar[CAPLEN], insstart[CAPLEN], insstop[CAPLEN], 
        inschar[CAPLEN],
        begoln[CAPLEN], clreoln[CAPLEN], clreoscr[CAPLEN],
        cursor_left[CAPLEN], cursor_right[CAPLEN], cursor_up[CAPLEN],
        cursor_down[CAPLEN];

static char modebold[] = "\033[1m", modeblink[] = "\033[5m", modeinv[] = "\033[7m",
            modeuline[] = "\033[4m", modestandon[] = "", modestandoff[] = "",
            modenorm[] = "\033[0m", modenormbackup[4];

static int len_begoln, len_clreoln, len_leftcur, len_upcur, gotocost,
           deletecost, insertcost, insertfinish, inscharcost;

char *tty_modebold = modebold,       *tty_modeblink = modeblink,
     *tty_modeinv  = modeinv,        *tty_modeuline = modeuline,
     *tty_modestandon = modestandon, *tty_modestandoff = modestandoff,
     *tty_modenorm = modenorm,       *tty_modenormbackup = modenormbackup,
     *tty_begoln   = begoln,         *tty_clreoln = clreoln,
     *tty_clreoscr = clreoscr;

/*
 * add the default keypad bindings to the list
 */
void tty_add_walk_binds()
{
    /*
  * Note: termcap doesn't have sequences for the numeric keypad, so we just
  * assume they are the same as for a vt100. They can be redefined
  * at runtime anyway (using #bind or #rebind)
    */
  add_keynode("KP2", "\033[%\025", 0, key_run_command, "south");
  add_keynode("KP3", "\033[%\027", 0, key_run_command, "down");
  add_keynode("KP4", "\033[%\022", 0, key_run_command, "west");
  add_keynode("KP5", "\033[%\013", 0, key_run_command, "exits");
  add_keynode("KP6", "\033[%\024", 0, key_run_command, "east");
  add_keynode("KP7", "\033[%\020", 0, key_run_command, "look");
  add_keynode("KP8", "\033[%\023", 0, key_run_command, "north");
  add_keynode("KP9", "\033[%\026", 0, key_run_command, "up");
}

/*
 * initialize the key binding list
 */
void tty_add_initial_binds()
{
  struct b_init_node {
    char *label, *seq;
    function_str funct;
  };
  static struct b_init_node b_init[] = {
    //{ "LF",         "\033[\005",           enter_line },
    //{ "Ret",        "\033[\004",           enter_line },
    //{ "BS",         "\033[\003",           del_char_left },
    //{ "Del",        "\033[\007",         del_char_left },
    { "Tab",        "\033[\001",  complete_word },
    { "C-a",        "!A",         begin_of_line },
    { "C-b",        "!B",         prev_char },
    { "C-d",        "!D",         del_char_right },
    { "C-e",        "!E",         end_of_line },
    { "C-f",        "!F",         next_char },
    { "C-k",        "!K",         kill_to_eol },
    { "C-l",        "!L",         redraw_line },
    { "C-n",        "!N",         next_line },
    { "C-p",        "!P",         prev_line },
    { "C-t",        "!T",         transpose_chars },
    { "C-w",        "!W",         to_history },
    { "C-Tab",      "\033[!\001", complete_line },
    { "M-b",        "#B",         prev_word },
    { "M-d",        "#D",         del_word_right },
    { "M-f",        "#F",         next_word },
    //{ "M-k",        "#K",         redraw_line_noprompt },
    { "M-t",        "#T",         transpose_words },
    { "M-u",        "#U",         upcase_word },
    { "M-l",        "#L",         downcase_word },
    { "M-BS",       "\033[#\003", del_word_left },
    { "M-Del",      "\033[#\007", del_word_left },
    { "",           "",             0 }
  };
  struct b_init_node *p = b_init;
  do {
    add_keynode(p->label, p->seq, 0, p->funct, NULL);
  } while((++p)->seq[0]);

  //if (cursor_left ) add_keynode("Left" , "\033[\022" , 0, prev_char, NULL);
  //if (cursor_right) add_keynode("Right", "\033[\024", 0, next_char, NULL);
  if (cursor_up   ) add_keynode("Up"   , "\033[\023"   , 0, prev_line, NULL);
  if (cursor_down ) add_keynode("Down" , "\033[\025" , 0, next_line, NULL);
}



void tty_puts(const char *s) { 
  QString result = s;
  if (!result.isEmpty()) wrapper->writeToStdout(result);
}
void tty_putc(unsigned char c) { wrapper->writeToStdout(QString(c)); }
void tty_gets(char* s, int size) { PRINTF("<-- tty_gets"); }
int tty_read(char *buf, int cnt) { PRINTF("<-- tty_read"); }//return wrapper->wrapper_stdin.read(buf, cnt); }

void tty_gotoxy_opt(int fromcol, int fromline, int tocol, int toline) {
  wrapper->emitMoveCursor(fromcol, fromline, tocol, toline);
}

void tty_gotoxy(int col, int line) {
  qDebug("implement me?");
  //wrapper->emitMoveCursor(-1, -1, col, line);
}
void wrapper_input_set(char *str) { wrapper->emitInputSet(str); }
void Wrapper::emitInputSet(char *str) {
  emit inputClear();
  emit inputInsertText(str);
}

void Wrapper::emitMoveCursor(int fromcol, int fromline, int tocol, int toline) {
  int diff = tocol - fromcol;
  qDebug("External: (%d, %d), (%d, %d), Diff %d", fromcol, fromline, tocol, toline, diff);
  emit moveCursor(diff);
}

void Wrapper::mergeInputWrapper(QString inputBarText, int cursorPosition) {
  strcpy(edbuf, inputBarText.toAscii().data());
  edlen = inputBarText.length();
  edbuf[edlen] = '\0';
  pos = cursorPosition;
}

void input_overtype_follow(char c) {
  wrapper->emitInputInsertText(QString(c));
  pos++;
}
void input_delete_nofollow_chars(int n) {
  qDebug("deleting %d chars", n);
  wrapper->emitInputDeleteChars(n);
  pos -= n;
}
void input_moveto(int new_pos) {
  qDebug("moving cursor to %d", new_pos);
  wrapper->emitInputMoveTo(new_pos);
  pos = new_pos;
}
void input_insert_follow_chars(char *str, int n) {
  wrapper->emitInputInsertText(QString(str).left(n));
  pos += n;
} // input parsing & displaying

void tty_printf(const char *format, ...) {
  char buf[1024], *bufp = buf;
  va_list va;
  int res;

  va_start(va, format);
  res = vsnprintf(buf, sizeof buf, format, va);
  va_end(va);

  if (res >= (signed)sizeof buf) {
#ifdef _WIN32
    bufp = (char*)malloc(res + 1); // HACK
#else
    bufp = (char*)alloca(res + 1);
#endif
    va_start(va, format);
    vsprintf(bufp, format, va);
    va_end(va);
  }

  tty_puts(bufp);
}

