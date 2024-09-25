#include <cstdlib>
#include <cassert>
#include <cstdio>

#include "stack.h"

int StackCtor(STACK* stackInfo, size_t capacity)
{
    assert(stackInfo);

    if (capacity < 1)
    {
        return -1;
    }
    if (capacity == 0)
    {
        stackInfo->capacity = 1;
        stackInfo->size = EMPTY;
        stackInfo->stack = (StackElem_t* ) calloc(stackInfo->capacity, sizeof(StackElem_t));
    }
    else
    {
        stackInfo->capacity = capacity;
        stackInfo->size = EMPTY;
        stackInfo->stack = (StackElem_t* ) calloc(stackInfo->capacity, sizeof(StackElem_t));
        assert(stackInfo->stack);
    }
    return 0;
}

int StackPush(STACK* stackInfo, StackElem_t elem)
{
    assert(stackInfo);

    if (stackInfo->size >= stackInfo->capacity)
    {
        StackRealloc(stackInfo, INCREASE);
    }

    stackInfo->size++;
    stackInfo->stack[stackInfo->size] = elem;

    return 0;
}

int StackPop(STACK* stackInfo)
{
    if (stackInfo->size < 0)
    {
        exit(0);
        return -1;
    }
    int popValue = stackInfo->stack[stackInfo->size];
    stackInfo->stack[stackInfo->size] = EMPTY;
    stackInfo->size--;
    if (stackInfo->size < stackInfo->capacity / 2 + 2)
    {
        StackRealloc(stackInfo, DECREASE);
    }

    return popValue;
}

int StackRealloc(STACK* stackInfo, RESIZE param)
{
    if (param == INCREASE)
    {
        void* tmp = realloc(stackInfo->stack, 2*stackInfo->capacity*sizeof(StackElem_t));
        if (!tmp)
        {
            return -1;
        }
        stackInfo->capacity *= 2;
    }
    else if (param == DECREASE) {
        void* tmp = realloc(stackInfo->stack, (stackInfo->capacity / 2 + 2)*sizeof(StackElem_t));
        if (!tmp)
        {
            return -1;
        }
        stackInfo->capacity = stackInfo->capacity / 2 + 2;
    }
    else
    {
        return -1;
    }

    return 0;
}

int StackDtor(STACK* stackInfo)
{
    free(stackInfo->stack);
    stackInfo->size = 0;
    stackInfo->capacity = 0;

    return 0;
}

int StackDump(STACK* stackInfo)
{
    FILE* file = NULL;
    file = fopen("dump.txt", "w");

    return 0;
}
