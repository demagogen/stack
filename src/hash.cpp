#include <stdint.h>

#include "stack.h"
#include "hash.h"
#include "stack_typedefs.h"
#include "color_scheme_changer.h"

#ifdef HASH_PROTECT
uint32_t gnu_hash(const StackElem_t* element)
{
    uint32_t hash_coefficient = 5381;
    uint32_t hash_result      = 0;

    for (; *element; element++)
    {
        hash_result = (hash_coefficient << 5) + hash_coefficient + *element;
    }

    return hash_result;
}

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
    for (int stack_element = 0; stack_element < stackInfo->capacity; stack_element++)
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

    return 0;
}
#endif
