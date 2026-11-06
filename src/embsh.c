#include "embsh.h"
#include "embsh_config.h"
#include <stddef.h>

static int help(int argc, char **argv, struct embsh shell);
static size_t readline(struct embsh shell, char *dst, size_t dstsz);
static void putcs(struct embsh shell, const char *str);
static int string_matches(const char *a, const char *b, int n);
static char *search(char *str, const char *arg);
static char *rsearch(char *s, const char *arg);

int embsh_exec(struct embsh shell, int *ret)
{
	int argc, status, ret_copy;
	char input[EMBSH_CONFIG_MAX_INPUT_LENGTH];
	char *argv[EMBSH_CONFIG_MAX_ARGS + 1];
	char *token, *next;
	size_t i;

	ret_copy = 0;
	argc = 0;
	status = 0;

	putcs(shell, embsh_config_prompt);
	readline(shell, input, sizeof(input));

	/* Tokenize */
	token = rsearch(input, embsh_config_token_delim);
	while (*token != '\0') {
		next = search(token, embsh_config_token_delim);
		if (*next != '\0') {
			*(next++) = '\0';
			next = rsearch(next, embsh_config_token_delim);
		}

		argv[argc++] = token;
		token = next;
	}

	argv[argc] = NULL;
	if (argc == 0)
		goto cleanup;

	/* Exec command */
	if (string_matches(argv[0], "help", -1)) {
		help(argc, argv, shell);
		goto cleanup;
	} else if (string_matches(argv[0], "quit", -1)) {
		status = -1;
	} else if (string_matches(argv[0], "clear", -1)) {
		putcs(shell, embsh_outs_clear);
		goto cleanup;
	} else {
		for (i = 0; i < shell.ncmd && shell.cmd != NULL; i++) {
			if (string_matches(argv[0], shell.cmd[i].name, -1)) {
				ret_copy = (int)shell.cmd[i].handler(argc, argv);
				break;
			}
		}

		if (i == shell.ncmd || shell.cmd == NULL) {
			putcs(shell, "embsh: Unknown command: ");
			putcs(shell, argv[0]);
			putcs(shell, "\n\r");
			ret_copy = 1;
		}

	}

	if (ret_copy == 0) {
		putcs(shell, "OK\n\r\n\r");
	} else {
		putcs(shell, "FAIL\n\r\n\r");
	}

cleanup:
	if (ret)
		*ret = ret_copy;

	return status;
}

static int help(int argc, char **argv, struct embsh shell)
{
	size_t i;

	(void)argc;
	(void)argv;

	putcs(shell, "Available commands:\r\n");
	for (i = 0; i < shell.ncmd && shell.cmd != NULL; i++) {
		putcs(shell, " ");
		putcs(shell, shell.cmd[i].name);
		putcs(shell, " ");
		putcs(shell, shell.cmd[i].usage);
		putcs(shell, "\n\r");
	}

	putcs(shell, "\n\r");
	return 0;
}

/* readline: Retrieve a line from the command-line and store it in dst. dstsz
 * specifies the size of the dst buffer. Returns the number of bytes written to
 * dst (excluding null termination). */
static size_t readline(struct embsh shell, char *dst, size_t dstsz)
{
	size_t pos, /* Current cursor position in input line buffer */
	       end; /* Position of end of line */
	char c[EMBSH_CONFIG_MAX_SEQUENCE];

	pos = 0;
	end = 0;

	if (!dst || !dstsz)
		return 0;

	if (!shell.getch)
		return 0;

	dst[end] = '\0';

	/* Identify character sequences: */
	while (1) {
		size_t i;
		i = 0;

		do {
			const char *tmp;
			size_t nmatch;

			c[i] = shell.getch();
			nmatch = 0;

			/* Check for line delimiter: */
			for (tmp = embsh_config_line_delim;
					*tmp != '\0' && *tmp != c[i];
					tmp++);
			if (*tmp != '\0') {
				for (size_t j = 0; j < i; j++)
					shell.putch(c[j]);
				putcs(shell, embsh_outs_newline);
				return end;
			}


			if (string_matches(c, embsh_ins_left, i + 1)) {
			if (i + 1 == sizeof(embsh_ins_left) - 1) {
				if (pos > 0) {
					putcs(shell, embsh_outs_left);
					pos--;
				}
				break;
			} nmatch++;
			}

			if (string_matches(c, embsh_ins_right, i + 1)) {
			if (i + 1 == sizeof(embsh_ins_right) - 1) {
				if (pos < end) {
					shell.putch(dst[pos]);
					pos++;
				}
				break;
			} nmatch++;
			}

			if (string_matches(c, embsh_ins_up, i + 1)) {
			if (i + 1 == sizeof(embsh_ins_up) - 1) {
				break;
			} nmatch++;
			}

			if (string_matches(c, embsh_ins_down, i + 1)) {
			if (i + 1 == sizeof(embsh_ins_down) - 1) {
				break;
			} nmatch++;
			}

			if (string_matches(c, embsh_ins_backspace, i + 1)) {
			if (i + 1 == sizeof(embsh_ins_backspace) - 1) {
				if (pos == 0)
					break;

				putcs(shell, embsh_outs_left);
				pos--;

				for (size_t j = pos; j < end; j++) {
					dst[j] = dst[j + 1];
					shell.putch(dst[j] == '\0' ? ' ' : dst[j]);
				}

				for (size_t j = pos; j < end; j++)
					putcs(shell, embsh_outs_left);
				end--;

				break;
			} nmatch++;
			}

			if (nmatch == 0) {
				for (size_t j = 0; j <= i; j++) {
					if (end >= dstsz - 1)
						break;

					end++;
					for (size_t k = pos; k < end; k++)
						dst[k + 1] = dst[k];

					dst[pos] = c[j];
					pos++;

					shell.putch(c[j]);
				}
				break;
			}

			i++;
		} while (i < sizeof(c));
	}

	return end;
}

static void putcs(struct embsh shell, const char *str)
{
#ifndef EMBSH_CONFIG_DISABLE_ECHO
	for (; *str != '\0'; str++)
		shell.putch(*str);
#else
	(void)shell;
	(void)str;
#endif
}

static int string_matches(const char *a, const char *b, int n)
{
	if (!n)
		return 1;

	if (!a && !b)
		return 1;

	if ((!a && b) || (a && !b))
		return 0;

	for (int i = 0; i != n; i++) {
		if (a[i] != b[i])
			return 0;

		if (a[i] == '\0' && b[i] == '\0')
			return 1;

		if (a[i] == '\0' || b[i] == '\0')
			return 0;
	}

	return 1;
}

static char *search(char *s, const char *arg)
{
	for (; *s != '\0'; s++) {
		const char *c;
		for (c = arg; *c != '\0'; c++) {
			if (*s == *c)
				break;
		}

		if (*c != '\0')
			break;
	}

	return s;
}

static char *rsearch(char *s, const char *arg)
{
	for (; *s != '\0'; s++) {
		const char *c;
		for (c = arg; *c != '\0'; c++) {
			if (*s == *c)
				break;
		}

		if (*c == '\0')
			break;
	}

	return s;
}
