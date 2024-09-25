#include <stdio.h>

#include "dump.h"
#include "stack.h"

int main() {
    STACK stackInfo = {};

    //dump(&stackInfo, stdout);
    StackCtor(&stackInfo, 10);
    StackPush(&stackInfo, 1);
    StackPush(&stackInfo, 2);
    StackPush(&stackInfo, 3);
    StackPush(&stackInfo, 4);

    //dump(&stackInfo, stdout);
    StackPush(&stackInfo, 5);
    StackPush(&stackInfo, 6);
    StackPush(&stackInfo, 7);
    StackPush(&stackInfo, 8);
    StackPush(&stackInfo, 9);
    StackPush(&stackInfo, 10);
    StackPush(&stackInfo, 11);

    //dump(&stackInfo, stdout);
    StackPop(&stackInfo);
    StackPop(&stackInfo);
    StackPop(&stackInfo);
    StackPop(&stackInfo);
    StackPop(&stackInfo);

    dump(&stackInfo, stdout);

    return 0;
}
