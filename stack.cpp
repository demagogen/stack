#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <sys/mman.h>

#include "stack.h"
#include "color_scheme_changer.h"

#define DUMP_FILE stdout

int stack_ctor(STACK* stackInfo, size_t capacity)
{
    assert(stackInfo);

    if (capacity == 0)
    {
        return -1;
    }
    else if (capacity == 0)
    {
        stackInfo->capacity = 1;

        // TODO move thids out from if
        stackInfo->size = EMPTY;
        stackInfo->stack_error = NONE;
        stackInfo->stack = (StackElem_t* ) calloc(stackInfo->capacity, sizeof(StackElem_t));
        if (verify_stack(stackInfo))
        {
            ASSERT(0);
            return -1;
        }
    }
    else
    {
        stackInfo->capacity = capacity;
        stackInfo->size = EMPTY;
        stackInfo->stack = (StackElem_t* ) calloc(stackInfo->capacity, sizeof(StackElem_t));
        stackInfo->stack_error = NONE;
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
        void* tmp = realloc(stackInfo->stack, 2*stackInfo->capacity*sizeof(StackElem_t));
        if (!tmp)
        {
            stackInfo->stack_error = STACK_ALLOCATION_ERROR;
            stack_dump(stackInfo);
            return -1;
        }
        stackInfo->capacity *= 2;
    }
    else if (param == DECREASE) {
        void* tmp = realloc(stackInfo->stack, (stackInfo->capacity / 2 + 2)*sizeof(StackElem_t));
        if (!tmp)
        {
            stackInfo->stack_error = STACK_ALLOCATION_ERROR;
            stack_dump(stackInfo);
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
    if (stackInfo->size < -1)
    {
        stack_dump(stackInfo);
        assert(0 && "size < -1");
    }
    if (stackInfo->capacity < 0)
    {
        stack_dump(stackInfo);
        assert(0 && "capacity not positive");
    }
    if (!stackInfo->stack)
    {
        stack_dump(stackInfo);
        assert(0 && "not space for stack");
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
    void* tmp = realloc(stackInfo->stack, 2*stackInfo->capacity*sizeof(StackElem_t));
    if (!tmp)
    {
        stackInfo->stack_error = STACK_ALLOCATION_ERROR;
        stack_dump(stackInfo);
        return -1;
    }
    stackInfo->capacity *= 2;

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
        void* tmp = realloc(stackInfo->stack, (stackInfo->capacity / 2 + 2)*sizeof(StackElem_t));
        if (!tmp)
        {
            stackInfo->stack_error = STACK_ALLOCATION_ERROR;
            stack_dump(stackInfo);
            return -1;
        }
        stackInfo->capacity = stackInfo->capacity / 2 + 2;
    }

    return 0;
}
