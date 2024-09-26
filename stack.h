#ifndef STACK_H_
#define STACK_H_

#include <cstddef>

typedef int StackElem_t;

#define ASSERT(expr) \
    if (!(expr)) \
        printf("%s at %s:%d born at %d", __PRETTY_FUNCTION__, __FILE__, __LINE__);

#define STACK_ASSERT_FUNC(stack, __FILE__, __LINE__); \
    printf("assert in %s on %d line\n", __FILE__, __LINE__);

#define LOG_INFO(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n",\
    __FILE__, __LINE__, ##__VA_ARGS__)

enum STACK_ERROR
{
    NONE            = -1,
    STACK_BAD_PTR   = 0,
    STACK_BAD_SIZE  = 1,
    STACK_UNDERFLOW = 2,
    STACK_OVERFLOW  = 3,
    STACK_ALLOCATION_ERROR = 4
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
    int          size;
    size_t       capacity;
    StackElem_t* stack;
    STACK_ERROR stack_error;
};

int stack_ctor(STACK* stackInfo, size_t capacity);
int stack_push(STACK* stackInfo, StackElem_t elem);
int stack_pop (STACK* stackInfo);
int stack_realloc(STACK* stackInfo, RESIZE param);
int stack_dtor(STACK* stackInfo);
int stack_dump(STACK* stackInfo, FILE* file);
int stack_ok(STACK* stackInfo, FILE* file);
const char* stack_struct_error(STACK* stackInfo, STACK_ERROR stack_error);

#endif
