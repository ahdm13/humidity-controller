#ifndef EMBSH_H
#define EMBSH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* embsh_cmd: Command information */
struct embsh_cmd {
	const char *name;  /* Name (Eg:- "ls") */
	const char *usage; /* Usage (Eg:- "<filename>") */
	int (*handler)(int argc, char **argv);
};

/* embsh: Context information */
struct embsh {
	/* List of commands: */
	struct embsh_cmd *cmd;
	size_t ncmd;

	/* Read / Write functions: */
	int (*getch)(void);
	void (*putch)(char c);
};

/* embsh_exec: Display prompt, read and execute a command, return. The return
   value of the command is stored in ret. Returns 0, unless shell is exiting, in
   which case -1 is returned. The return code of the exiting shell is stored in
   ret. */
int embsh_exec(struct embsh shell, int *ret);

#ifdef __cplusplus
}
#endif

#endif /* EMBSH_H */
