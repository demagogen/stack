#ifndef STACK_H_
#define STACK_H_

#include <cstddef>
#include <stdio.h>
#include <cstdint>
#include <random>

typedef int      StackElem_t;
typedef uint64_t Canary_t;

#ifdef UNPROTECT
#undef HASH_PROTECT
#undef CANARY_PROTECT
#endif

/********************************************************DEFINE ASSERTS***********************************************************************/
#define ASSERT(expr)                                                                                                       \
    if (!(expr)) graphic_printf(RED, BOLD, "%s at %s:%d born at %s\n", __PRETTY_FUNCTION__, __FILE__, __LINE__, __func__); \

#define STACK_ASSERT_FUNC(stack, __FILE__, __LINE__);        \
    printf("assert in %s on %d line\n", __FILE__, __LINE__); \

#define LOG_INFO(M, ...)                                                         \
    fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__) \

/********************************************************DEFINE CANARY PROTECT***************************************************************/
#define CANARY_PROTECT

#ifdef CANARY_PROTECT
const Canary_t canary = 1234567; // test canary value (add rand())

#define CANARY_STRUCT_CONST_INIT(canary_const) \
    const Canary_t canary_const = canary;      \

#define CANARY_ELEMENT(element) element

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

#define CHECK_CANARY_PROTECTION(stackInfo)                                                            \
    if (CANARY_VALUE_CHECK(stackInfo->first_struct_canary)  ||                                        \
        CANARY_VALUE_CHECK(stackInfo->second_struct_canary) ||                                        \
        CANARY_STACK_CHECK(stackInfo))                                                                \
    {                                                                                                 \
        graphic_printf(RED, BOLD, "FUCK YOU CANARY\n");                                               \
        graphic_printf(RED, BOLD, "%d\n", CANARY_VALUE_CHECK(stackInfo->first_struct_canary));        \
        graphic_printf(RED, BOLD, "%d\n", CANARY_VALUE_CHECK(stackInfo->second_struct_canary));       \
        graphic_printf(RED, BOLD, "%d\n", CANARY_STACK_CHECK(stackInfo));                             \
        stack_dump(stackInfo);                                                                        \
        ASSERT(0 && "canary died :-(");                                                               \
    }                                                                                                 \

#endif

enum STACK_ERROR
{
    NONE                   = -1,
    STACK_BAD_PTR          =  0,
    STACK_BAD_SIZE         =  1,
    STACK_UNDERFLOW        =  2,
    STACK_OVERFLOW         =  3,
    STACK_ALLOCATION_ERROR =  4
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
    INCREASE   =  1
};

struct STACK
{
    #ifdef CANARY_PROTECT
    CANARY_STRUCT_CONST_INIT(first_struct_canary);
    #endif

    FILE*                       dump_file;
    int                         size;
    int                         capacity;
    STACK_ERROR                 error_info;

    #ifdef HASH_PROTECT
    HASH_SUM_VARIABLE          (hash_sum);
    #endif

    #ifdef CANARY_PROTECT
    CANARY_STRUCT_CONST_INIT(second_struct_canary);
    #endif

    StackElem_t*                stack;
};

int         stack_ctor         (STACK* stackInfo, size_t capacity);
int         stack_push         (STACK* stackInfo, StackElem_t elem);
int         stack_pop          (STACK* stackInfo, StackElem_t* value);
int         stack_realloc      (STACK* stackInfo, RESIZE param);
int         stack_dtor         (STACK* stackInfo);
int         stack_dump         (STACK* stackInfo);
int         verify_stack       (STACK* stackInfo);
const char* stack_struct_error (STACK* stackInfo, STACK_ERROR stack_error);
int         realloc_up         (STACK* stackInfo);
int         realloc_down       (STACK* stackInfo);

#endif
