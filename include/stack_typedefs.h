#ifndef STACK_TYPEDEFS_H_
#define STACK_TYPEDEFS_H_

#include <ctype.h>
#include <cstdio>
#include <cstdint>

// #define CANARY_PROTECT
// #define HASH_PROTECT

typedef int      StackElem_t;
typedef uint64_t Canary_t;

enum STACK_ERROR
{
    STACK_NONE             = 0,
    STACK_BAD_PTR          = 1,
    STACK_BAD_SIZE         = 2,
    STACK_UNDERFLOW        = 3,
    STACK_OVERFLOW         = 4,
    STACK_ALLOCATION_ERROR = 5,
    STACK_BAD_CAPACITY     = 6,
    STACK_UNKNOWN_PARAM    = 7,
    STACK_HASH_SUM_ERROR   = 8,
    STACK_CANARY_DIED      = 9
};

enum STACK_ENUMS
{
    EMPTY      = -1,
    ZERO       =  0,
    ERROR_FILL =  1
};

struct STACK
{
    Canary_t     first_struct_canary;
    FILE*        dump_file;
    ssize_t      size;
    ssize_t      capacity;
    STACK_ERROR  error_info;
    uint32_t     hash_sum;
    uint32_t     descriptor;
    Canary_t     second_struct_canary;
    StackElem_t* stack;
};

#endif
