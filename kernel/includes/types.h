// Cenneo OS
// /kernel/include/types.h
// Types defination

#ifndef TYPES_H_
#define TYPES_H_

typedef long long int           intmax_t;
typedef unsigned long long int  uintmax_t;

#ifdef IA32
typedef long int                intptr_t;
typedef unsigned long int       uintptr_t;
#endif

#ifdef AMD64
typedef int                     intptr_t;
typedef unsigned int            uintptr_t;
#endif

#ifdef ARM

#endif

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long int  uint64_t;
typedef signed long long int    int64_t;
typedef signed int              int32_t;
typedef signed short int        int16_t;
typedef signed char             int8_t;


typedef unsigned long long      u64;
typedef unsigned int            u32;
typedef unsigned short int      u16;
typedef unsigned char           u8;
typedef long long int           s64;
typedef int                     s32;
typedef short int               s16;
typedef char                    s8;



typedef unsigned long		    u_long;
typedef unsigned int		    u_int;
typedef unsigned short		    u_short;
typedef unsigned char		    u_char;



typedef unsigned long		    ulong;
typedef unsigned int		    uint;
typedef unsigned short		    ushort;
typedef unsigned char		    unchar;



typedef long long int           int64;
typedef long int                int_long;
typedef int                     int32;
typedef short int               int16;
typedef char                    int8;

#endif
