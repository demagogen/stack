#include "stack.h"

int main() {
    STACK stackInfo = {};
    StackElem_t last_stack_element = 0;
    stack_ctor(&stackInfo, 5);

    stack_push(&stackInfo, 1000);
    stack_push(&stackInfo, 2000);
    stack_push(&stackInfo, 3000);
    stack_push(&stackInfo, 4000);
    // stack_push(&stackInfo, 5000);
    // stack_push(&stackInfo, 6000);
    stack_dump(&stackInfo);

    printf("first stack_pop\n");
    stack_pop(&stackInfo, &last_stack_element);
    printf("second stack_pop\n");
    stack_pop(&stackInfo, &last_stack_element);
    printf("third stack_pop\n");
    stack_pop(&stackInfo, &last_stack_element);
    printf("fourth stack_pop\n");
    stack_pop(&stackInfo, &last_stack_element);
    printf("fifth stack_pop\n");
    stack_pop(&stackInfo, &last_stack_element);
    printf("sixst stack_pop\n");
    stack_pop(&stackInfo, &last_stack_element);
    printf("seventh stack pop\n");
    stack_pop(&stackInfo, &last_stack_element);
    printf("eigthth stack pop\n");
    stack_pop(&stackInfo, &last_stack_element);

    return 0;
}
