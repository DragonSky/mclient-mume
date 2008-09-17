/* public things from tcp.c */

#ifndef _TCP_H_
#define _TCP_H_

#ifdef MCLIENT
#include "defines.h"

#  ifdef __cplusplus
   extern "C" {
#  endif

   /* from telnet.h */
#  define TELOPT_TTYPE    24      /* terminal type */
#  define TELOPT_ECHO     1       /* echo */
#  define TELOPT_SGA      3       /* suppress go ahead */
#  define IAC     255             /* interpret as command: */
#  define SB      250             /* interpret as subnegotiation */
#  define SE      240             /* end sub negotiation */
#  define DONT    254             /* you are not to use option */
#  define DO      253             /* please, you use option */
#  define WONT    252             /* I won't use option */
#  define WILL    251             /* I will use option */
#  define GA      249             /* you may reverse the line */

#endif

extern int tcp_fd;		/* current socket file descriptor */
extern int tcp_main_fd;		/* main session socket */
extern int tcp_max_fd;		/* highest used fd */

extern int tcp_count;		/* number of open connections */
extern int tcp_attachcount;	/* number of spawned or attached commands */

extern int conn_max_index;	/* 1 + highest used conn_list[] index */

    
/* multiple connections control */

/* state of telnet connection */
#define NORMAL		0
#define ALTNORMAL	1
#define GOT_N		2
#define GOT_R		3
#define GOTIAC		4
#define GOTWILL		5
#define GOTWONT		6
#define GOTDO		7
#define GOTDONT		8
#define GOTSB		9
#define GOTSBIAC       10

/* connection flags: */
/* ACTIVE:	display remote output	*/
/* SPAWN:	spawned cmd, not a mud	*/
/* IDEDITOR:	sent #request editor	*/
/* IDPROMPT:	sent #request prompt	*/
#define ACTIVE	 1
#define SPAWN	 2
#define IDEDITOR 4
#define IDPROMPT 8

typedef struct {
    char *id;			/* session id */
    char *host;			/* address of remote host */
    int port;			/* port number of remote host */
    int fd;			/* fd number */
    char *fragment;		/* for SPAWN connections: unprocessed text */
    char flags;
    char state;
} connsess;

extern connsess conn_list[MAX_CONNECTS];     /* connection list */

extern byte conn_table[MAX_FDSCAN];	     /* fd -> index translation table */

#define CONN_LIST(n) conn_list[conn_table[n]]
#define CONN_INDEX(n) conn_list[n]

#ifndef MCLIENT
extern fd_set fdset;               /* set of descriptors to select() on */
#endif

int  tcp_connect	__P ((char *addr, int port));
int  tcp_read		__P ((int fd, char *buffer, int maxsize));
void tcp_raw_write	__P ((int fd, char *data, int len));
void tcp_write_tty_size __P ((void));
void tcp_write		__P ((int fd, char *data));
void tcp_main_write	__P ((char *data));
void tcp_flush		__P ((void));
void tcp_set_main	__P ((int fd));
void tcp_open		__P ((char *id, char *initstring, char *host, int port));
int  tcp_find		__P ((char *id));
void tcp_show		__P ((void));
void tcp_close		__P ((char *id));
void tcp_togglesnoop	__P ((char *id));
void tcp_spawn		__P ((char *id, char *cmd));
int  tcp_unIAC		__P ((char *data, int len));
int  tcp_read_addIAC	__P ((int fd, char *data, int len));

#ifdef __cplusplus
}
#endif

#endif /* _TCP_H_ */

