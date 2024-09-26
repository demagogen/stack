#include <cstdlib>
#include <cassert>
#include <cstdio>

#include "stack.h"
#include "color_scheme_changer.h"

int stack_ctor(STACK* stackInfo, size_t capacity)
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

int stack_push(STACK* stackInfo, StackElem_t elem)
{
    assert(stackInfo);

    if (stackInfo->size >= stackInfo->capacity)
    {
        stack_realloc(stackInfo, INCREASE);
    }

    stackInfo->size++;
    stackInfo->stack[stackInfo->size] = elem;

    return 0;
}

int stack_pop(STACK* stackInfo)
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
        stack_realloc(stackInfo, DECREASE);
    }

    return popValue;
}

int stack_realloc(STACK* stackInfo, RESIZE param)
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

int stack_dtor(STACK* stackInfo)
{
    free(stackInfo->stack);
    stackInfo->size = 0;
    stackInfo->capacity = 0;

    return 0;
}

int stack_dump(STACK* stackInfo, FILE* file)
{
    graphic_printf(WHITE, BOLD, "Stack size     %10d\n", stackInfo->size);
    graphic_printf(WHITE, BOLD, "Stack capacity %10d\n", stackInfo->capacity);
    graphic_printf(WHITE, BOLD, "Stack elements list\n");

    for (size_t element_index = 0; element_index < stackInfo->capacity; element_index++)
    {
        graphic_printf(GREEN, BOLD, "\tstack[%3d]  %5d\n", element_index, stackInfo->stack[element_index]);
    }

    return 0;
}

int stack_ok(STACK* stackInfo, FILE* file)
{
    if (stackInfo->size < -1)
    {
        stack_dump(stackInfo, file);
        assert(0 && "size < -1");
    }
    if (stackInfo->capacity < 0)
    {
        stack_dump(stackInfo, file);
        assert(0 && "capacity not positive");
    }
    if (!stackInfo->stack)
    {
        stack_dump(stackInfo, file);
        assert(0 && "not space for stack");
    }
    if (stackInfo->size > stackInfo->capacity)
    {
        stack_dump(stackInfo, file);
        ASSERT(0 && "break limit size");
    }
    return 0;
}

const char* stack_str_error(STACK* stackInfo, STACK_ERROR stack_error)
{
    #define DESCR_(_error) \
        case _error: return #_error
    switch(stack_error)
    {
        DESCR_(STACK_BAD_PTR);
        DESCR_(STACK_BAD_SIZE);

        default:
            return "error print error";
    }
    #undef DESCR_
}
