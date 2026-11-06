#ifndef CMD_INTERNAL_H
#define CMD_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

uint32_t cmd_atou(const char *str);
size_t cmd_utoa(uint32_t value, char *dst, size_t dstsz);
int cmd_checkargc(int argc, int nargs);

#endif /* CMD_INTERNAL_H */
