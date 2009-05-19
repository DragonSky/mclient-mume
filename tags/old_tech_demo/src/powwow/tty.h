/* public definitions from tty.c */

#ifndef _TTY_H_
#define _TTY_H_

#ifdef __cplusplus
extern "C" {
#endif

/* from telnet.h */
#define TELOPT_TTYPE    24      /* terminal type */
#define TELOPT_ECHO     1       /* echo */
#define TELOPT_SGA      3       /* suppress go ahead */
#define IAC     255             /* interpret as command: */
#define SB      250             /* interpret as subnegotiation */
#define SE      240             /* end sub negotiation */
/* telnet commands */
#define DONT    254             /* you are not to use option */
#define DO      253             /* please, you use option */
#define WONT    252             /* I won't use option */
#define WILL    251             /* I will use option */
#define GA      249             /* you may reverse the line */

extern int tty_read_fd;

extern char *tty_clreoln, *tty_clreoscr, *tty_begoln,
            *tty_modebold, *tty_modeblink, *tty_modeuline,
	    *tty_modenorm, *tty_modenormbackup,
            *tty_modeinv, *tty_modestandon, *tty_modestandoff;

void tty_bootstrap	__P ((void));
void tty_start		__P ((void));
void tty_quit		__P ((void));
void tty_special_keys	__P ((void));
void tty_sig_winch_bottomhalf	__P ((void));
void tty_add_walk_binds		__P ((void));
void tty_add_initial_binds	__P ((void));
void tty_gotoxy			__P ((int col, int line));
void tty_gotoxy_opt		__P ((int fromcol, int fromline, int tocol, int toline));

void input_delete_nofollow_chars	__P ((int n));
void input_overtype_follow		__P ((char c));
void input_insert_follow_chars		__P ((char *str, int n));
void input_moveto			__P ((int new_pos));

#if !defined(USE_LOCALE) && !defined(MCLIENT)

#define tty_puts(s) fputs((s), stdout)
/* printf("%s", (s)); would be as good */

#define tty_putc(c)             putc((unsigned char)(c), stdout)
#define tty_printf              printf
#define tty_read(buf, cnt)      read(tty_read_fd, (buf), (cnt))
#define tty_gets(s,size)        fgets((s), (size), stdin)
#endif

#ifdef USE_LOCALE /* USE_LOCALE */

void tty_puts           __P ((const char *s));
void tty_putc           __P ((char c));
void tty_printf         __P ((const char *format, ...));
int  tty_read           __P ((char *buf, size_t count));
void tty_gets           __P ((char *s, int size));
void tty_flush          __P ((void));
void tty_raw_write      __P ((char *data, int len));
int  tty_has_chars      __P ((void));
#else

#define tty_flush()             fflush(stdout)
#define tty_raw_write(s,size)   do { tty_flush(); write(1, (s), (size)); } while (0)

#endif /* USE_LOCALE */

#ifdef __cplusplus
}
#endif

#endif /* _TTY_H_ */
