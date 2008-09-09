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
  add_keynode("KP2", "\033Or", 0, key_run_command, "s");
  add_keynode("KP3", "\033Os", 0, key_run_command, "d");
  add_keynode("KP4", "\033Ot", 0, key_run_command, "w");
  add_keynode("KP5", "\033Ou", 0, key_run_command, "exits");
  add_keynode("KP6", "\033Ov", 0, key_run_command, "e");
  add_keynode("KP7", "\033Ow", 0, key_run_command, "look");
  add_keynode("KP8", "\033Ox", 0, key_run_command, "n");
  add_keynode("KP9", "\033Oy", 0, key_run_command, "u");
}

/*
 * initialize the key binding list
 */
void tty_add_initial_binds()
{
  /*
  struct b_init_node {
    char *label, *seq;
    function_any funct;
  };
  static struct b_init_node b_init[] = {
    { "LF",         "\n",           enter_line },
    { "Ret",        "\r",           enter_line },
    { "BS",         "\b",           del_char_left },
    { "Del",        "\177",         del_char_left },
    { "Tab",        "\t",           complete_word },
    { "C-a",        "\001",         begin_of_line },
    { "C-b",        "\002",         prev_char },
    { "C-d",        "\004",         del_char_right },
    { "C-e",        "\005",         end_of_line },
    { "C-f",        "\006",         next_char },
    { "C-k",        "\013",         kill_to_eol },
    { "C-l",        "\014",         redraw_line },
    { "C-n",        "\016",         next_line },
    { "C-p",        "\020",         prev_line },
    { "C-t",        "\024",         transpose_chars },
    { "C-w",        "\027",         to_history },
    { "M-Tab",      "\033\t",       complete_line },
    { "M-b",        "\033b",        prev_word },
    { "M-d",        "\033d",        del_word_right },
    { "M-f",        "\033f",        next_word },
    { "M-k",        "\033k",        redraw_line_noprompt },
    { "M-t",        "\033t",        transpose_words },
    { "M-u",        "\033u",        upcase_word },
    { "M-l",        "\033l",        downcase_word },
    { "M-BS",       "\033\b",       del_word_left },
    { "M-Del",      "\033\177",     del_word_left },
    { "",           "",             0 }
  };
  struct b_init_node *p = b_init;
  do {
    //add_keynode(p->label, p->seq, 0, p->funct, NULL);
    add_keynode(p->label, p->seq, 0, key_run_command, "!");
  } while((++p)->seq[0]);

  if (cursor_left ) add_keynode("Left" , cursor_left , 0, prev_char, NULL);
  if (cursor_right) add_keynode("Right", cursor_right, 0, next_char, NULL);
  if (cursor_up   ) add_keynode("Up"   , cursor_up   , 0, prev_line, NULL);
  if (cursor_down ) add_keynode("Down" , cursor_down , 0, next_line, NULL);
  */
}



void tty_puts(const char *s) { 
  QString result = s;
  if (!result.isEmpty()) wrapper->writeToStdout(result);
}
void tty_putc(unsigned char c) { wrapper->writeToStdout(QString(c)); }
void tty_gets(char* s, int size) { PRINTF("tty_gets"); }
int tty_read(char *buf, int cnt) { PRINTF("tty_read"); }//return wrapper->wrapper_stdin.read(buf, cnt); }

void tty_gotoxy_opt(int fromcol, int fromline, int tocol, int toline) {
  wrapper->emitMoveCursor(fromcol, fromline, tocol, toline);
}

void tty_gotoxy(int col, int line) {
  qDebug("implement me?");
  //wrapper->emitMoveCursor(-1, -1, col, line);
}

void Wrapper::emitMoveCursor(int fromcol, int fromline, int tocol, int toline) {
  int diff = tocol - fromcol;
  qDebug("External: (%d, %d), (%d, %d), Diff %d", fromcol, fromline, tocol, toline, diff);
  emit moveCursor(diff);
}

void Wrapper::mergeInputWrapper(QString inputBarText, int cursorPosition) {
  strcpy(edbuf, inputBarText.toAscii().data());
  edlen = inputBarText.length();
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

