/* public things from beam.c */

#ifndef _BEAM_H_
#define _BEAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef QTPOWWOW
void finish_edit __P ((editsess *sp));
#endif

int process_message	__P ((char *buf, int len));
void cancel_edit	__P ((editsess *sp));
void abort_edit_fd	__P ((int fd));
void message_edit	__P ((char *text, int msglen, char view, char builtin));
void sig_chld_bottomhalf __P ((void));

extern char edit_start[];
extern char edit_end[];
extern editsess *edit_sess;

#ifdef __cplusplus
}
#endif

#endif /* _BEAM_H_ */

