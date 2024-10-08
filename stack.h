#ifndef STACK_H_
#define STACK_H_

#include <cstddef>
#include <stdio.h>
#include <cstdint>
#include <random>
#include "stack_typedefs.h"

#define CANARY_PROTECT

#ifdef UNPROTECT
#undef HASH_PROTECT
#undef CANARY_PROTECT
#endif

/********************************************************DEFINE ASSERTS***********************************************************************/
#define ASSERT(expr)                                                                                                 \
    if (!(expr)) graphic_printf(RED, BOLD, "%s[%p] at %s:%d born at %s\n", __PRETTY_FUNCTION__, __PRETTY_FUNCTION__, \
                                                                           __FILE__, __LINE__, __func__);            \

#define STACK_ASSERT_FUNC(stack, __FILE__, __LINE__);        \
    printf("assert in %s on %d line\n", __FILE__, __LINE__); \

#define LOG_INFO(M, ...)                                                         \
    fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__) \

/********************************************************DEFINE CANARY PROTECT***************************************************************/

#ifdef CANARY_PROTECT
const Canary_t canary = 12345678; // test canary value (add rand())

#define CANARY_ELEMENT(element) element \

#define CANARY_INIT(stackInfo)                                                                                     \
    *(Canary_t* )((char* )stackInfo->stack) = canary;                                                              \
    *(Canary_t* )((char* )stackInfo->stack + sizeof(Canary_t) + stackInfo->capacity*sizeof(StackElem_t)) = canary; \

#define CANARY_STACK_INIT(stackInfo)                                                                            \
    stackInfo->stack = (StackElem_t* ) calloc(1, stackInfo->capacity*sizeof(StackElem_t) + 2*sizeof(Canary_t)); \

#define CANARY_VALUE_CHECK(canary_stack) \
    canary_stack != canary               \

#define CANARY_STACK_CHECK(stackInfo)                                                                              \
    *(Canary_t* )((char* )stackInfo->stack) != canary &&                                                           \
    *(Canary_t* )((char* )stackInfo->stack + sizeof(Canary_t) + stackInfo->capacity*sizeof(StackElem_t)) != canary \

#define CHECK_CANARY_PROTECTION(stackInfo)                                                      \
    if (CANARY_VALUE_CHECK(stackInfo->first_struct_canary)  ||                                  \
        CANARY_VALUE_CHECK(stackInfo->second_struct_canary) ||                                  \
        CANARY_STACK_CHECK(stackInfo))                                                          \
    {                                                                                           \
        graphic_printf(YELLOW, BOLD, "DONT GIVE UP!!!\n");                                      \
        stackInfo->error_info = CANARY_DIED;                                                    \
        graphic_printf(RED, BOLD, "FUCK YOU CANARY\n");                                         \
        graphic_printf(RED, BOLD, "%d\n", CANARY_VALUE_CHECK(stackInfo->first_struct_canary));  \
        graphic_printf(RED, BOLD, "%d\n", CANARY_VALUE_CHECK(stackInfo->second_struct_canary)); \
        graphic_printf(RED, BOLD, "%d\n", CANARY_STACK_CHECK(stackInfo));                       \
        stack_dump(stackInfo);                                                                  \
        return CANARY_DIED;                                                                     \
    }                                                                                           \

#endif

enum STACK_ERROR
{
    NONE                   = 0,
    STACK_BAD_PTR          = 1,
    STACK_BAD_SIZE         = 2,
    STACK_UNDERFLOW        = 3,
    STACK_OVERFLOW         = 4,
    STACK_ALLOCATION_ERROR = 5,
    STACK_BAD_CAPACITY     = 6,
    UNKNOWN_PARAM          = 7,
    HASH_SUM_ERROR         = 8,
    CANARY_DIED            = 9
};

enum STACK_ENUMS
{
    EMPTY      = -1,
    ERROR_FILL = 0
};

enum RESIZE
{
    DECREASE   = -1,
    ERROR_SIZE = 0,
    INCREASE   = 1
};

struct STACK
{
    Canary_t     first_struct_canary;
    FILE*        dump_file;
    ssize_t      size;
    ssize_t      capacity;
    STACK_ERROR  error_info;
    uint32_t     hash_sum;
    Canary_t     second_struct_canary;
    StackElem_t* stack;
};

STACK_ERROR         stack_ctor         (STACK* stackInfo, ssize_t capacity);
STACK_ERROR         stack_push         (STACK* stackInfo, StackElem_t elem);
STACK_ERROR         stack_pop          (STACK* stackInfo, StackElem_t* value);
STACK_ERROR         stack_realloc      (STACK* stackInfo, RESIZE param);
STACK_ERROR         stack_dtor         (STACK* stackInfo);
STACK_ERROR         stack_dump         (STACK* stackInfo);
STACK_ERROR         verify_stack       (STACK* stackInfo);
const char*         stack_struct_error (STACK* stackInfo);
STACK_ERROR         realloc_up         (STACK* stackInfo);
STACK_ERROR         realloc_down       (STACK* stackInfo);

#endif
