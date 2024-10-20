#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <sys/mman.h>

#include "stack.h"
#include "hash.h"
#include "color_scheme_changer.h"
#include "stack_typedefs.h"
// #define UNPROTECT

STACK_ERROR _stack_ctor(STACK* stackInfo, ssize_t capacity, const char* __FILE, const int __LINE, const char* __func)
{
    ASSERT(stackInfo->stack, __FILE, __LINE, __func);

    if (capacity < 0)
    {
        return STACK_BAD_CAPACITY;
    }
    stackInfo->capacity   = capacity;
    stackInfo->size       = EMPTY;
    stackInfo->error_info = STACK_NONE;

    #ifdef CANARY_PROTECT
    stackInfo->stack = (StackElem_t* ) calloc(1, stackInfo->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t));
    CANARY_INIT(stackInfo);
    stackInfo->first_struct_canary  = canary;
    stackInfo->second_struct_canary = canary;
    #else
    stackInfo->stack = (StackElem_t* ) calloc(1, stackInfo->capacity * sizeof(StackElem_t));
    #endif

    #ifdef HASH_PROTECT
    count_hash_sum(stackInfo);
    #endif

    verify_stack(stackInfo);

    return STACK_NONE;
}

STACK_ERROR _stack_push(STACK* stackInfo, StackElem_t elem, const char* __FILE, const int __LINE, const char* __func)
{
    verify_stack(stackInfo);

    if (stackInfo->size + 1 >= stackInfo->capacity)
    {
        stack_realloc(stackInfo, INCREASE);
    }
    stackInfo->size++;

    #ifdef CANARY_PROTECT
    *(StackElem_t* )((char* )stackInfo->stack + stackInfo->size * sizeof(StackElem_t) + sizeof(Canary_t)) = elem;
    #else
    *(StackElem_t* )((char* )stackInfo->stack + stackInfo->size * sizeof(StackElem_t)) = elem;
    #endif

    #ifdef HASH_PROTECT
    COUNT_HASH_SUM(stackInfo);
    CHECK_HASH_SUM(stackInfo);
    #endif

    return STACK_NONE;
}

STACK_ERROR _stack_pop(STACK* stackInfo, StackElem_t* value, const char* __FILE, const int __LINE, const char* __func)
{
    verify_stack(stackInfo);

    const int realloc_coeff = 4;

    if (stackInfo->size == -1)
    {
        graphic_printf(RED, BOLD, "stack pop error: pop from empty stack\n");
        graphic_printf(RED, BOLD, "operation declined\n");
        return STACK_BAD_SIZE;
    }

    #ifdef CANARY_PROTECT
    *value = *(StackElem_t* )((char* )stackInfo->stack + sizeof(Canary_t) + stackInfo->size * sizeof(StackElem_t));
    *(StackElem_t* )((char* )stackInfo->stack + sizeof(Canary_t) + stackInfo->size * sizeof(StackElem_t)) = EMPTY;
    #else
    *value = stackInfo->stack[stackInfo->size];
    *(StackElem_t* )((char* )stackInfo->stack + stackInfo->size * sizeof(StackElem_t)) = ZERO;
    #endif

    stackInfo->size--;
    if (stackInfo->size < stackInfo->capacity / realloc_coeff)
    {
        stack_realloc(stackInfo, DECREASE);
    }

    #ifdef HASH_PROTECT
    COUNT_HASH_SUM(stackInfo);
    CHECK_HASH_SUM(stackInfo);
    #endif

    return STACK_NONE;
}

STACK_ERROR _stack_realloc(STACK* stackInfo, RESIZE param, const char* __FILE, const int __LINE, const char* __func)
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
        return STACK_UNKNOWN_PARAM;
    }

    return STACK_NONE;
}

STACK_ERROR _stack_dtor(STACK* stackInfo, const char* __FILE, const int __LINE, const char* __func)
{
    verify_stack(stackInfo);

    free(stackInfo->stack);
    stackInfo->size        = 0;
    stackInfo->capacity    = 0;
    stackInfo->error_info  = STACK_NONE;

    return STACK_NONE;
}

STACK_ERROR _stack_dump(STACK* stackInfo, const char* __FILE, const int __LINE, const char* __func)
{
    printf("------------------------------------------------------------------------\n");
    printf(" __  __  ______   _____            _____   _    _  __  __  _____        \n"
           "|  \\/  ||  ____| / ____|    /\\    |  __ \\ | |  | ||  \\/  ||  __ \\  \n"
           "| \\  / || |__   | |  __    /  \\   | |  | || |  | || \\  / || |__) |   \n"
           "| |\\/| ||  __|  | | |_ |  / /\\ \\  | |  | || |  | || |\\/| ||  ___/   \n"
           "| |  | || |____ | |__| | / ____ \\ | |__| || |__| || |  | || |          \n"
           "|_|  |_||______| \\_____|/_/    \\_\\|_____/  \\____/ |_|  |_||_|     \n\n");

    if (stackInfo->error_info == STACK_NONE)
    {
        graphic_printf(GREEN, BOLD, "NO ERRORS\nGOOOOOOOOOOOOL\n");
    }
    else
    {
        graphic_printf(RED, BOLD, "OHHHHH SHIT, ERRORS GO AGAIN...\n");
        graphic_printf(RED, BOLD, "error in stack: %s\n", stack_struct_error(stackInfo));
    }

    graphic_printf(WHITE, BOLD, "Stack size         %10d\n", stackInfo->size);
    graphic_printf(WHITE, BOLD, "Stack capacity     %10d\n", stackInfo->capacity);
    graphic_printf(WHITE, BOLD, "Stack array pointer    %p\n", stackInfo->stack);
    graphic_printf(WHITE, BOLD, "Stack elements list\n");

    #ifdef CANARY_PROTECT
    for (ssize_t element_index = 0; element_index < stackInfo->capacity; element_index++)
    {
        graphic_printf(GREEN, BOLD, "\t     stack[%3d] %5d\n", element_index,
                       *(StackElem_t* )((char* )stackInfo->stack + sizeof(Canary_t) + element_index * sizeof(StackElem_t)));
    }
    #else
    for (ssize_t element_index = 0; element_index < stackInfo->capacity; element_index++)
    {
        graphic_printf(GREEN, BOLD, "\tstack[%3d]  %5d\n", element_index,
                       *(StackElem_t* )((char* )stackInfo->stack + element_index * sizeof(StackElem_t)));
    }
    #endif
    printf("------------------------------------------------------------------------\n");

    return STACK_NONE;
}

STACK_ERROR _verify_stack(STACK* stackInfo, const char* __FILE, const int __LINE, const char* __func)
{
    #ifdef HASH_PROTECT
    CHECK_HASH_SUM(stackInfo);
    #endif

    #ifdef CANARY_PROTECT
    CHECK_CANARY_PROTECTION(stackInfo);
    #endif

    if (stackInfo->size < -1)
    {
        stackInfo->error_info = STACK_BAD_SIZE;
        stack_dump(stackInfo);
        return STACK_BAD_SIZE;
    }
    if (stackInfo->capacity < 0)
    {
        stackInfo->error_info = STACK_BAD_CAPACITY;
        stack_dump(stackInfo);
        return STACK_BAD_CAPACITY;
    }
    if (stackInfo->size > -1 && stackInfo->capacity > 0 && !stackInfo->stack)
    {
        stackInfo->error_info = STACK_ALLOCATION_ERROR;
        stack_dump(stackInfo);
        return STACK_ALLOCATION_ERROR;
    }
    if (stackInfo->size > stackInfo->capacity)
    {
        stackInfo->error_info = STACK_OVERFLOW;
        stack_dump(stackInfo);
        return STACK_OVERFLOW;
    }
    return STACK_NONE;
}

const char* _stack_struct_error(STACK* stackInfo, const char* __FILE, const int __LINE, const char* __func)
{
    #define DESCRIPTION_(_error) \
        case _error: return #_error

    switch(stackInfo->error_info)
    {
        DESCRIPTION_(STACK_NONE);
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

STACK_ERROR _realloc_up(STACK* stackInfo, const char* __FILE, const int __LINE, const char* __func)
{
    verify_stack(stackInfo);

    ssize_t memory_multiply_coeff = 2;
    stackInfo->capacity *= memory_multiply_coeff;
    #ifdef CANARY_PROTECT
    stackInfo->stack = (StackElem_t* ) realloc(stackInfo->stack,
                                               stackInfo->capacity*sizeof(StackElem_t) + 2 * sizeof(Canary_t));
    for (ssize_t element_index = stackInfo->size; element_index < stackInfo->capacity; element_index++)
    {
        *(StackElem_t* )((char* )stackInfo->stack + element_index * sizeof(StackElem_t)) = 0;
    }
    #else
    stackInfo->stack = (StackElem_t* ) realloc(stackInfo->stack,
                                               stackInfo->capacity * sizeof(StackElem_t));
    for (ssize_t element_index = stackInfo->size + 1; element_index < stackInfo->capacity; element_index++)
    {
        *(StackElem_t* )((char* )stackInfo->stack + element_index * sizeof(StackElem_t)) = 0;
    }
    #endif

    if (!stackInfo->stack)
    {
        stackInfo->error_info = STACK_ALLOCATION_ERROR;
        stack_dump(stackInfo);
        return STACK_ALLOCATION_ERROR;
    }

    #ifdef CANARY_PROTECT
        CANARY_INIT(stackInfo);
    #endif

    return STACK_NONE;
}

STACK_ERROR _realloc_down(STACK* stackInfo, const char* __FILE, const int __LINE, const char* __func)
{
    stackInfo->capacity = stackInfo->capacity / 2 + 2;
    #ifdef CANARY_PROTECT
    stackInfo->stack = (StackElem_t* ) realloc(stackInfo->stack,
                                               stackInfo->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t));
    #else
    stackInfo->stack = (StackElem_t* ) realloc(stackInfo->stack,
                                               stackInfo->capacity * sizeof(StackElem_t));
    #endif

    if (!stackInfo->stack)
    {
        stackInfo->error_info = STACK_ALLOCATION_ERROR;
        stack_dump(stackInfo);
        return STACK_ALLOCATION_ERROR;
    }

    #ifdef CANARY_PROTECT
    CANARY_INIT(stackInfo);
    #endif

    return STACK_NONE;
}
