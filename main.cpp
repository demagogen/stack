#include "stack.h"
#include "color_scheme_changer.h"

int main() {
    STACK stackInfo = {};
    StackElem_t last_stack_element = 0;
    stack_ctor(&stackInfo, 5);

    printf("start debugging\n");
    for (size_t stack_index = 0; stack_index < 80; stack_index++)
    {
        graphic_printf(MAGENTA, BOLD, "%lld\n", stack_index);
        stack_push(&stackInfo, stack_index);
        stack_dump(&stackInfo);
    }
    stack_push(&stackInfo, 5000);
    stack_push(&stackInfo, 6000);
    stack_dump(&stackInfo);

    return 0;
}
