#include <stdio.h>

#include "dump.h"

FILE* file = stdout;

int dump(STACK* stackInfo, FILE* file)
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
