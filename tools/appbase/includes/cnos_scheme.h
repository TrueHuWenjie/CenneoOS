

#ifndef CNOS_SCHEME_H_
#define CNOS_SCHEME_H_

#define SCHEME_RECORD_LEVEL_ERROR 0xff
#define SCHEME_RECORD_LEVEL_FAULT 0xc0
#define SCHEME_RECORD_LEVEL_WARN  0x80
#define SCHEME_RECORD_LEVEL_NOTE  0x40
#define SCHEME_RECORD_LEVEL_DEBUG 0x00

struct scheme_record_args
{
    char level;
    char *info;
    int size;
};

struct scheme_log_args
{
    char level;
    char *info;
    int size;
};


#endif
