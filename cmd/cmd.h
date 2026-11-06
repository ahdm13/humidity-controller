#ifndef CMD_CMD_H
#define CMD_CMD_H

#include <stddef.h>

int cmd_dht22_data(int argc, char **argv);
int cmd_dht22(int argc, char **argv);
int cmd_dht22_humidity(int argc, char **argv);
int cmd_mistmaker_set(int argc, char **argv);

static struct embsh_cmd cmd[] = {
	{ "dht22-data", "", cmd_dht22_data },
	{ "dht22", "[enable|disable]", cmd_dht22 },
	{ "dht22-humidity", "", cmd_dht22_humidity },
	{ "mistmaker-set", "<percent>", cmd_mistmaker_set }
};
static const size_t ncmd = sizeof(cmd)/sizeof(*cmd);

#endif /* CMD_CMD_H */
