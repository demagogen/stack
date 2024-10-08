#include "stack.h"
#include "hash.h"
#include "color_scheme_changer.h"

int main() {
    STACK stackInfo = {};
    StackElem_t last_stack_element = 0;
    stack_ctor(&stackInfo, 5);

    stack_dump(&stackInfo);

    char* pointer = (char* )stackInfo.stack;
    for (ssize_t index = 0; index < stackInfo.capacity; index++)
    {
        *((char* )stackInfo.stack + index * sizeof(int)) = index;
    }
    printf("hash sum %d\n", stackInfo.hash_sum);
    stack_dump(&stackInfo);

    return 0;
}
