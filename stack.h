#ifndef STACK_H_
#define STACK_H_

#include <cstddef>
#include <stdio.h>
#include <cstdint>

typedef int StackElem_t;

#define ASSERT(expr)                                                                                  \
    if (!(expr)) printf("%s at %s:%d born at %s", __PRETTY_FUNCTION__, __FILE__, __LINE__, __func__); \

#define STACK_ASSERT_FUNC(stack, __FILE__, __LINE__);        \
    printf("assert in %s on %d line\n", __FILE__, __LINE__); \

#define LOG_INFO(M, ...)                                                         \
    fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__) \

#define CANARY_STRUCT_CONST_INIT(canary_const) \
    const uint64_t canary_const = canary;      \

#define CANARY_ELEMENT(element) element

#define CANARY_INIT(stack, canary_position) \
    stack[canary_position] = canary     \

#define CANARY_STRUCT_CHECK(canary_stack) \
    canary_stack != canary                \

#define CANARY_END_CHECK(stack, canary_position) \
    stack[canary_position] != -63         \

#define HASH_SUM_VARIABLE(name_of_const) \
    uint32_t hash_sum;                   \

#define COUNT_HASH_SUM(stackInfo) \
    count_hash_sum(stackInfo);    \

#define CHECK_CANARY_PROTECTION                                                                      \
    if (CANARY_STRUCT_CHECK(stackInfo->start_struct_canary) ||                                       \
        CANARY_STRUCT_CHECK(stackInfo->end_struct_canary)   ||                                       \
        CANARY_END_CHECK   (stackInfo->stack, stackInfo->capacity))                                  \
    {\
        graphic_printf(RED, BOLD, "FUCK YOU CANARY\n");                                    \
        printf("%d\n", CANARY_STRUCT_CHECK(stackInfo->start_struct_canary));                         \
        printf("%d\n", CANARY_STRUCT_CHECK(stackInfo->end_struct_canary));                           \
        printf("%d\n", CANARY_END_CHECK   (stackInfo->stack, stackInfo->capacity));                  \
        stack_dump(stackInfo);                                                                       \
        ASSERT(0 && "penetration error");                                                            \
    }                                                                                                \

#define CHECK_HASH_SUM(stack)             \
    if (check_hash_sum(stack))            \
    {\
        stack_dump(stackInfo); \
        graphic_printf(RED, BOLD, "fucking hash checker\n");          \
        assert(0 && "penetration_error\n");        \
        ASSERT(0 && "penetration error"); \
    }                                     \

const uint64_t canary = 9112001;

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
    CANARY_STRUCT_CONST_INIT   (start_struct_canary);
    FILE*                       dump_file;
    int                         size;
    int                         capacity;
    STACK_ERROR                 stack_error;
    CANARY_STRUCT_CONST_INIT   (end_struct_canary);
    HASH_SUM_VARIABLE          (hash_sum);
    char*                       stack;
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
uint32_t    gnu_hash           (const StackElem_t* element);
int         count_hash_sum     (STACK* stackInfo);
bool        check_hash_sum     (STACK* stackInfo);

#endif
