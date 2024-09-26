#include <stdio.h>

#include "stack.h"

int main() {
    STACK stackInfo = {};

    stack_ctor(&stackInfo, 10);
    stack_push(&stackInfo, 1);
    stack_push(&stackInfo, 2);
    stack_push(&stackInfo, 3);
    stack_push(&stackInfo, 4);
    stack_ok  (&stackInfo, stdout);

    stack_push(&stackInfo, 5);
    stack_push(&stackInfo, 6);
    stack_push(&stackInfo, 7);
    stack_push(&stackInfo, 8);
    stack_push(&stackInfo, 9);
    stack_push(&stackInfo, 10);
    stack_push(&stackInfo, 11);

    stack_pop(&stackInfo);
    stack_pop(&stackInfo);
    stack_pop(&stackInfo);
    stack_pop(&stackInfo);
    stack_pop(&stackInfo);
    //stack_dump(&stackInfo, stdout);
    ASSERT(2 == 1);
    LOG_INFO("Struct info %d", stackInfo.size);

    return 0;
}
