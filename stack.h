#ifndef STACK_H_
#define STACK_H_

#include <cstddef>

typedef int StackElem_t;

enum STACK_ENUMS
{
    EMPTY = -1
};

enum RESIZE
{
    DECREASE = -1,
    INCREASE =  1
};

struct STACK
{
    size_t       size;
    size_t       capacity;
    StackElem_t* stack;
};

int StackCtor(STACK* stackInfo, size_t capacity);
int StackPush(STACK* stackInfo, StackElem_t elem);
int StackPop (STACK* stackInfo);
int StackRealloc(STACK* stackInfo, RESIZE param);
int StackDtor(STACK* stackInfo);
int StackDump(STACK* stackInfo);

#endif
