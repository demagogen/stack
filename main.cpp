#include "stack.h"

int main() {
    STACK stackInfo = {};
    StackElem_t last_stack_value = 0;

    stack_ctor(&stackInfo, 5);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 1);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 2);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 3);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 4);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 5);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 6);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 7);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 8);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 9);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 10);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 11);
    //stack_dump(&stackInfo);

    //printf("control position\n");
    stack_push(&stackInfo, 12);
    //stack_dump(&stackInfo);

    stack_pop(&stackInfo, &last_stack_value);
    //stack_dump(&stackInfo);

    stack_pop(&stackInfo, &last_stack_value);
    stack_pop(&stackInfo, &last_stack_value);
    stack_pop(&stackInfo, &last_stack_value);
    stack_pop(&stackInfo, &last_stack_value);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);
    stack_dump(&stackInfo);
    printf("%d\n", last_stack_value);

    return 0;
}
