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
    COUNT_HASH_SUM(stackInfo);
    CANARY_INIT(stackInfo->stack, stackInfo->capacity);
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
    COUNT_HASH_SUM(stackInfo);

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
    COUNT_HASH_SUM(stackInfo);
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
    CHECK_CANARY_PROTECTION;
    CHECK_HASH_SUM(stackInfo);

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
    CANARY_INIT(stackInfo->stack, stackInfo->capacity);
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
    CANARY_INIT(stackInfo->stack, stackInfo->capacity);

    return 0;
}

uint32_t gnu_hash(const StackElem_t* element)
{
    uint32_t hash_coefficient = 5381;
    uint32_t hash_result = 0;

    for (; *element; element++) {
        hash_result = (hash_coefficient << 5) + hash_coefficient + *element;
    }

    return hash_result;
}

int count_hash_sum(STACK* stackInfo)
{
    if (!stackInfo)
    {
        verify_stack(stackInfo);
        return -1;
    }
    stackInfo->hash_sum = 0;
    for (int stack_element = 0; stack_element < stackInfo->size; stack_element++)
    {
        stackInfo->hash_sum += gnu_hash((const StackElem_t* ) (stackInfo->stack + stack_element));
    }

    return 0;
}

bool check_hash_sum(STACK* stackInfo)
{
    if (!stackInfo)
    {
        verify_stack(stackInfo);
        return false;
    }

    uint32_t check_hash_sum = 0;
    for (int stack_element = 0; stack_element < stackInfo->size; stack_element++)
    {
        check_hash_sum += gnu_hash((const StackElem_t* ) (stackInfo->stack + stack_element));
    }
    if (stackInfo->hash_sum == check_hash_sum)
    {
        return false;
    }
    else
    {
        return true;
    }
}
