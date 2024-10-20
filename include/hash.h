#ifndef HASH_H_
#define HASH_H_

#include "stack.h"
#include "stack_typedefs.h"

#ifdef HASH_PROTECT

#define HASH_SUM_VARIABLE  \
    uint32_t hash_sum = 0; \

#define COUNT_HASH_SUM(stackInfo) \
    count_hash_sum(stackInfo);    \

#define CHECK_HASH_SUM(stackInfo)                            \
    if (check_hash_sum(stackInfo))                           \
    {                                                        \
        stack_dump(stackInfo);                               \
        graphic_printf(RED, BOLD, "fucking hash checker\n"); \
        stackInfo->error_info = STACK_HASH_SUM_ERROR;        \
        return STACK_HASH_SUM_ERROR;                         \
    }                                                        \

uint32_t gnu_hash       (const StackElem_t* element);
bool     check_hash_sum (STACK* stackInfo);
int      count_hash_sum (STACK* stackInfo);

#endif

#endif
