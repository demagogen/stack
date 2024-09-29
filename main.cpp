#include "stack.h"

int main() {
    STACK stackInfo = {};
    StackElem_t last_stack_value = 0;
    printf("start using stack\n");
    stack_ctor(&stackInfo, 5);
    printf("end ctor\n");
    printf("%ld\n", *(stackInfo.stack + stackInfo.capacity));
    printf("1  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 1);
    printf("2  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 2);
    printf("3  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 3);
    printf("4  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 4);
    printf("5  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 5);
    printf("6  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 6);
    printf("7  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 7);
    printf("8  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 8);
    printf("9  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 9);
    printf("%10  lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 10);
    printf("11  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);

    stack_push(&stackInfo, 11);
    printf("12  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);

    //printf("control position\n");
    stack_push(&stackInfo, 12);
    printf("13  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);

    stack_pop(&stackInfo, &last_stack_value);
    printf("14  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);

    stack_pop(&stackInfo, &last_stack_value);
    printf("15  %lld\n", stackInfo.hash_sum);
    stack_pop(&stackInfo, &last_stack_value);
    printf("16  %lld\n", stackInfo.hash_sum);
    stack_pop(&stackInfo, &last_stack_value);
    printf("17  %lld\n", stackInfo.hash_sum);
    stack_pop(&stackInfo, &last_stack_value);
    printf("18  %lld\n", stackInfo.hash_sum);
    //stack_dump(&stackInfo);
    verify_stack(&stackInfo);
    printf("19  %lld\n", stackInfo.hash_sum);
    stack_dump(&stackInfo);
    printf("20  %lld\n", stackInfo.hash_sum);
    printf("21  %lld\n", stackInfo.hash_sum);

    return 0;
}
