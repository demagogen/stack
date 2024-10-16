#ifndef STACK_H_
#define STACK_H_

#include <cstddef>
#include <stdio.h>
#include <cstdint>
#include <random>
#include "stack_typedefs.h"

#ifdef UNPROTECT
#undef HASH_PROTECT
#undef CANARY_PROTECT
#endif

/********************************************************DEFINE ASSERTS***********************************************************************/
#define ASSERT(expr, __FILE, __LINE, __func)                                                        \
    if (!(expr)) graphic_printf(RED, BOLD, "%s[%p] at %s:%d born at %s\n", __func, __func,          \
                                                                           __FILE, __LINE, __func); \

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
        stackInfo->error_info = STACK_CANARY_DIED;                                              \
        graphic_printf(RED, BOLD, "FUCK YOU CANARY\n");                                         \
        graphic_printf(RED, BOLD, "%d\n", CANARY_VALUE_CHECK(stackInfo->first_struct_canary));  \
        graphic_printf(RED, BOLD, "%d\n", CANARY_VALUE_CHECK(stackInfo->second_struct_canary)); \
        graphic_printf(RED, BOLD, "%d\n", CANARY_STACK_CHECK(stackInfo));                       \
        stack_dump(stackInfo);                                                                  \
        return STACK_CANARY_DIED;                                                               \
    }                                                                                           \

#endif

enum RESIZE
{
    DECREASE   = -1,
    ERROR_SIZE = 0,
    INCREASE   = 1
};

STACK_ERROR         _stack_ctor         (STACK* stackInfo, ssize_t capacity,   const char* __FILE, const int __LINE, const char* __func);
STACK_ERROR         _stack_push         (STACK* stackInfo, StackElem_t elem,   const char* __FILE, const int __LINE, const char* __func);
STACK_ERROR         _stack_pop          (STACK* stackInfo, StackElem_t* value, const char* __FILE, const int __LINE, const char* __func);
STACK_ERROR         _stack_realloc      (STACK* stackInfo, RESIZE param,       const char* __FILE, const int __LINE, const char* __func);
STACK_ERROR         _stack_dtor         (STACK* stackInfo,                     const char* __FILE, const int __LINE, const char* __func);
STACK_ERROR         _stack_dump         (STACK* stackInfo,                     const char* __FILE, const int __LINE, const char* __func);
STACK_ERROR         _verify_stack       (STACK* stackInfo,                     const char* __FILE, const int __LINE, const char* __func);
const char*         _stack_struct_error (STACK* stackInfo,                     const char* __FILE, const int __LINE, const char* __func);
STACK_ERROR         _realloc_up         (STACK* stackInfo,                     const char* __FILE, const int __LINE, const char* __func);
STACK_ERROR         _realloc_down       (STACK* stackInfo,                     const char* __FILE, const int __LINE, const char* __func);

#define             stack_ctor(stackInfo, capacity) _stack_ctor         (stackInfo, capacity, __FILE__, __LINE__, __func__)
#define             stack_push(stackInfo, elem)     _stack_push         (stackInfo, elem,     __FILE__, __LINE__, __func__)
#define             stack_pop(stackInfo, value)     _stack_pop          (stackInfo, value,    __FILE__, __LINE__, __func__)
#define             stack_realloc(stackInfo, param) _stack_realloc      (stackInfo, param,    __FILE__, __LINE__, __func__)
#define             stack_dtor(stackInfo)           _stack_dtor         (stackInfo,           __FILE__, __LINE__, __func__)
#define             stack_dump(stackInfo)           _stack_dump         (stackInfo,           __FILE__, __LINE__, __func__)
#define             verify_stack(stackInfo)         _verify_stack       (stackInfo,           __FILE__, __LINE__, __func__)
#define             stack_struct_error(stackInfo)   _stack_struct_error (stackInfo,           __FILE__, __LINE__, __func__)
#define             realloc_up(stackInfo)           _realloc_up         (stackInfo,           __FILE__, __LINE__, __func__)
#define             realloc_down(stackInfo)         _realloc_down       (stackInfo,           __FILE__, __LINE__, __func__)

#endif
