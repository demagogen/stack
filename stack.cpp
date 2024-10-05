#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <sys/mman.h>

// #define UNPROTECT

#include "stack.h"
#include "color_scheme_changer.h"


int stack_ctor(STACK* stackInfo, size_t capacity)
{

    if (capacity < 0)
    {
        return -1;
    }
    stackInfo->capacity    = capacity;
    stackInfo->size        = EMPTY;
    stackInfo->stack_error = NONE;

    #ifdef CANARY_PROTECT
        stackInfo->stack       = (StackElem_t* ) calloc(1, stackInfo->capacity*sizeof(StackElem_t) + 2*sizeof(Canary_t));
        CANARY_INIT(stackInfo);
    #else
        stackInfo->stack       = (StackElem_t* ) calloc(1, stackInfo->capacity*sizeof(StackElem_t));
    #endif

    #ifdef HASH_PROTECT
        count_hash_sum(stackInfo);
    #endif

    verify_stack(stackInfo);

    return 0;
}

int stack_push(STACK* stackInfo, StackElem_t elem)
{
    verify_stack(stackInfo);

    if (stackInfo->size + 1 >= stackInfo->capacity)
    {
        stack_realloc(stackInfo, INCREASE);
    }
    stackInfo->size++;

    #ifdef CANARY_PROTECT
        *(StackElem_t* )((char* )stackInfo->stack + stackInfo->size*sizeof(StackElem_t) + sizeof(Canary_t)) = elem;
    #else
        *(StackElem_t* )((char* )stackInfo->stack + stackInfo->size*sizeof(StackElem_t)) = elem;
    #endif

    #ifdef HASH_PROTECT
        COUNT_HASH_SUM(stackInfo);
    #endif

    return 0;
}

int stack_pop(STACK* stackInfo, StackElem_t* value)
{
    verify_stack(stackInfo);

    if (stackInfo->size == -1)
    {
        graphic_printf(RED, BOLD, "stack pop error: pop from empty stack\n");
        graphic_printf(RED, BOLD, "operation declined\n");
        return -1;
    }

    #ifdef CANARY_PROTECT
        *value = *(StackElem_t* )((char* )stackInfo->stack + sizeof(Canary_t) + stackInfo->size*sizeof(StackElem_t));
        *(StackElem_t* )((char* )stackInfo->stack + sizeof(Canary_t) + stackInfo->size*sizeof(StackElem_t)) = EMPTY;
    #else
        *value = stackInfo->stack[stackInfo->size];
        *(StackElem_t* )((char* )stackInfo->stack + stackInfo->size*sizeof(StackElem_t)) = EMPTY;
    #endif

    stackInfo->size--;
    if (stackInfo->size < stackInfo->capacity / 2 + 2)
    {
        stack_realloc(stackInfo, DECREASE);
    }
    #ifdef HASH_PROTECT
        COUNT_HASH_SUM(stackInfo);
        CHECK_HASH_SUM(stackInfo);
    #endif

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
    verify_stack(stackInfo);

    free(stackInfo->stack);
    stackInfo->size        = 0;
    stackInfo->capacity    = 0;
    stackInfo->stack_error = NONE;

    return 0;
}

int stack_dump(STACK* stackInfo)
{
    graphic_printf(WHITE, BOLD, "Stack size     %10d\n", stackInfo->size);
    graphic_printf(WHITE, BOLD, "Stack capacity %10d\n", stackInfo->capacity);
    graphic_printf(WHITE, BOLD, "Stack elements list\n");
    graphic_printf(WHITE, BOLD, "Stack array pointer %p\n", stackInfo->stack);

    #ifdef CANARY_PROTECT
        for (size_t element_index = 0; element_index < stackInfo->capacity; element_index++)
        {
            graphic_printf(GREEN, BOLD, "\tstack[%3d]  %5d\n", element_index,
                           *(StackElem_t* )((char* )stackInfo->stack + sizeof(Canary_t) + element_index * sizeof(StackElem_t)));
        }
    #else
        for (size_t element_index = 0; element_index < stackInfo->capacity; element_index++)
        {
            graphic_printf(GREEN, BOLD, "\tstack[%3d]  %5d\n", element_index,
                           *(StackElem_t* )((char* )stackInfo->stack + element_index * sizeof(StackElem_t)));
        }
    #endif

    return 0;
}

int verify_stack(STACK* stackInfo)
{
    #ifdef HASH_PROTECT
        CHECK_HASH_SUM(stackInfo);
    #endif

    #ifdef CANARY_PROTECT
        CHECK_CANARY_PROTECTION;
    #endif

    if (stackInfo->size < -1)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "verify stack error: size < -1\n");
    }
    if (stackInfo->capacity < 0)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "verify stack error: capacity < 0\n");
    }
    if (stackInfo->size > -1 && stackInfo->capacity > 0 && !stackInfo->stack)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "verify stack_error: null pointer on filled stack\n");
    }
    if (stackInfo->size > stackInfo->capacity)
    {
        stack_dump(stackInfo);
        ASSERT(0 && "verify stack error: size > capacity\n");
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
    verify_stack(stackInfo);

    size_t memory_multiply_coeff = 2;
    stackInfo->capacity *= memory_multiply_coeff;
    stackInfo->stack = (StackElem_t* ) realloc(stackInfo->stack,
                                        stackInfo->capacity*sizeof(StackElem_t));
    if (!stackInfo->stack)
    {
        stackInfo->stack_error = STACK_ALLOCATION_ERROR;
        stack_dump(stackInfo);
        ASSERT(0);
        return -1;
    }

    #ifdef CANARY_PROTECT
        CANARY_INIT(stackInfo);
    #endif

    return 0;
}

int realloc_down(STACK* stackInfo)
{
    stackInfo->capacity = stackInfo->capacity / 2 + 2;
    stackInfo->stack = (StackElem_t* ) realloc(stackInfo->stack,
                                               stackInfo->capacity*sizeof(StackElem_t));
    if (!stackInfo->stack)
    {
        stackInfo->stack_error = STACK_ALLOCATION_ERROR;
        stack_dump(stackInfo);
        ASSERT(0);
        return -1;
    }

    #ifdef CANARY_PROTECT
        CANARY_INIT(stackInfo);
    #endif

    return 0;
}

uint32_t gnu_hash(const StackElem_t* element)
{
    uint32_t hash_coefficient = 5381;
    uint32_t hash_result      = 0;

    for (; *element; element++) {
        hash_result = (hash_coefficient << 5) + hash_coefficient + *element;
    }

    return hash_result;
}

#ifdef HASH_PROTECT
int count_hash_sum(STACK* stackInfo)
{
    stackInfo->hash_sum = 0;
    for (int stack_element = 0; stack_element < stackInfo->size; stack_element++)
    {
        stackInfo->hash_sum += gnu_hash((const StackElem_t* ) ((char* )stackInfo->stack + stack_element*sizeof(StackElem_t)));
    }

    return 0;
}

bool check_hash_sum(STACK* stackInfo)
{
    // verify_stack(stackInfo);

    uint32_t check_hash_sum = 0;
    for (int stack_element = 0; stack_element < stackInfo->size; stack_element++)
    {
        check_hash_sum += gnu_hash((const StackElem_t* ) ((char* )stackInfo->stack + stack_element*sizeof(StackElem_t)));
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
#endif
