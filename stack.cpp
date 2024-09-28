#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <sys/mman.h>

#include "stack.h"
#include "color_scheme_changer.h"

int stack_ctor(STACK* stackInfo, size_t capacity)
{
    ASSERT(stackInfo);

    if (capacity < 0)
    {
        return -1;
    }
    else if (capacity == 0)
    {
        capacity = 1;
    }
    stackInfo->capacity    = capacity;
    stackInfo->size        = EMPTY;
    stackInfo->stack_error = NONE;
    stackInfo->stack       = (char* ) calloc(stackInfo->capacity + CANARY_ELEMENT(sizeof(canary)),
                                             sizeof(StackElem_t));
    CANARY_INIT(stackInfo->stack, stackInfo->capacity + CANARY_ELEMENT(sizeof(canary)));
    if (verify_stack(stackInfo))
    {
        return -1;
    }

    return 0;
}

int stack_push(STACK* stackInfo, StackElem_t elem)
{
    assert(stackInfo);

    if (stackInfo->size + 1 >= stackInfo->capacity)
    {
        stack_realloc(stackInfo, INCREASE);
    }
    stackInfo->size++;
    *(stackInfo->stack + stackInfo->size) = elem;

    return 0;
}

int stack_pop(STACK* stackInfo, StackElem_t* value)
{
    if (stackInfo->size < 0)
    {
    stackInfo->stack_error = STACK_UNDERFLOW;
        stack_dump(stackInfo);
        return -1;
    }
    *value = stackInfo->stack[stackInfo->size];
    stackInfo->stack[stackInfo->size] = EMPTY;
    stackInfo->size--;
    if (stackInfo->size < stackInfo->capacity / 2 + 2)
    {
        stack_realloc(stackInfo, DECREASE);
    }

    return 0;
}

int stack_realloc(STACK* stackInfo, RESIZE param)
{
    if (param == INCREASE)
    {
        realloc_up(stackInfo);
    }
    else if (param == DECREASE)
    {
        realloc_down(stackInfo);
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
    stackInfo->stack_error = NONE;

    return 0;
}

int stack_dump(STACK* stackInfo)
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

int verify_stack(STACK* stackInfo)
{
    if (stackInfo->canary1 != canary || stackInfo->canary2 != canary)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "penetration error");
    }
    CANARY_CHECK(stackInfo->stack, stackInfo->capacity + CANARY_ELEMENT(sizeof(canary)));
    if (stackInfo->size < -1)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "size < -1");
    }
    if (stackInfo->capacity < 0)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "capacity not positive");
    }
    if (!stackInfo->stack)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "not space for stack");
    }
    if (stackInfo->size > stackInfo->capacity)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "break limit size");
    }
    return 0;
}

const char* stack_struct_error(STACK* stackInfo, STACK_ERROR stack_error)
{
    #define DESCRIPTION_(_error) \
        case _error: return #_error

    switch(stack_error)
    {
        DESCRIPTION_(NONE);
        DESCRIPTION_(STACK_BAD_PTR);
        DESCRIPTION_(STACK_BAD_SIZE);
        DESCRIPTION_(STACK_UNDERFLOW);
        DESCRIPTION_(STACK_OVERFLOW);
        DESCRIPTION_(STACK_ALLOCATION_ERROR);

        default:
            return "error print error";
    }

    #undef DESCRIPTION_
}

int realloc_up(STACK* stackInfo)
{
    if (verify_stack(stackInfo))
    {
        ASSERT(0);
        return -1;
    }
    stackInfo->capacity *= 2;
    stackInfo->stack = (char* ) realloc(stackInfo->stack,
                                        stackInfo->capacity*sizeof(StackElem_t) + CANARY_ELEMENT(sizeof(canary)));
    if (!stackInfo->stack)
    {
        stackInfo->stack_error = STACK_ALLOCATION_ERROR;
        stack_dump(stackInfo);
        ASSERT(0);
        return -1;
    }

    return 0;
}

int realloc_down(STACK* stackInfo)
{
    if (verify_stack(stackInfo))
    {
        ASSERT(0);
        return -1;
    }
    else
    {
        stackInfo->capacity = stackInfo->capacity / 2 + 2;
        stackInfo->stack = (char* ) realloc(stackInfo->stack,
                                            stackInfo->capacity*sizeof(StackElem_t) + CANARY_ELEMENT(sizeof(canary)));
        if (!stackInfo->stack)
        {
            stackInfo->stack_error = STACK_ALLOCATION_ERROR;
            stack_dump(stackInfo);
            return -1;
        }
    }

    return 0;
}
