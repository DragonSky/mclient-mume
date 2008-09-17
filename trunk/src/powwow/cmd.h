/* public things from cmd.c */

#ifndef _CMD_H_
#define _CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MCLIENT
void wrapper_cmd_var            __P ((char *arg));
#endif

typedef struct cmdstruct {
    char *sortname;             /* set to NULL if you want to sort by command name */
    char *name;			/* command name */
    char *help;			/* short help */
    function_str funct;		/* function to call */
    struct cmdstruct *next;
} cmdstruct;

extern cmdstruct *commands;

void show_stat		__P ((void));

void cmd_add_command( cmdstruct *cmd );

#ifdef __cplusplus
}
#endif

#endif /* _CMD_H_ */

