#ifndef EMBSH_CONFIG_H
#define EMBSH_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*#define EMBSH_CONFIG_ENABLE_HISTORY */
/*#define EMBSH_CONFIG_DISABLE_ECHO */

#define EMBSH_CONFIG_MAX_INPUT_LENGTH 128
#define EMBSH_CONFIG_MAX_ARGS         16
#define EMBSH_CONFIG_MAX_SEQUENCE     4

static const char embsh_config_prompt[]      = "> ";
static const char embsh_config_line_delim[]  = "\r\n";
static const char embsh_config_token_delim[] = " \t\r\n";

/* Input character sequences: */
static const char embsh_ins_backspace[]      = "\177";
static const char embsh_ins_up[]             = "\033[A";
static const char embsh_ins_down[]           = "\033[B";
static const char embsh_ins_left[]           = "\033[D";
static const char embsh_ins_right[]          = "\033[C";
static const char embsh_ins_ctrlc[]          = "\003";
static const char embsh_ins_ctrld[]          = "\004";

/* Output character sequences: */
static const char embsh_outs_left[]          = "\b";
static const char embsh_outs_newline[]       = "\r\n";
static const char embsh_outs_backspace[]     = "\b \b";
static const char embsh_outs_clear[]         = "\033[2J\033[0;0H";

#ifdef __cplusplus
}
#endif

#endif /* EMBSH_CONFIG_H */
