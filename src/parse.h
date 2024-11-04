#ifndef PARSE_H
#define PARSE_H

#include "map.h"

#define CTL_SEQ_COUNT 100
#define CTL_TOKEN_LENGTH 50
#define CTL_SEQ_PARAM_COUNT 5

struct ctl_seq {
        char id[CTL_TOKEN_LENGTH];
        unsigned long param_count;
        char params[CTL_SEQ_PARAM_COUNT][CTL_TOKEN_LENGTH];
};

unsigned long ctl_seq_parse(const char *str, unsigned long length, unsigned long off, struct ctl_seq *seq, _Bool *status);

_Bool map_parse(const char *str, struct map *ptr);

#endif