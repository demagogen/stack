#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#include "ColorSchemeChanger.h"

void graphic_printf(COLOR color, STYLE style, const char *st, ...) {
    assert(st);

    va_list argument_pointer;
    char *pointer;

    va_start     (argument_pointer, st);
    change_color (color, style);
    vprintf      (st, argument_pointer);
    printf       ("\033[0m");
    va_end       (argument_pointer);
}

void change_color(COLOR color, STYLE style) {
    printf("\033[");

    switch(style) {
        case RESET:         printf("0;");  break;
        case BOLD:          printf("1;");  break;
        case UNDERLINE:     printf("4;");  break;
        case INVERSE:       printf("7;");  break;
        case BOLD_OFF:      printf("21;"); break;
        case UNDERLINE_OFF: printf("24;"); break;
        case INVERSE_OFF:   printf("27;"); break;

        default:
            assert(0 && "Invalid style");
            break;
    }

    switch(color) {
        case BLACK:   printf("30m"); break;
        case RED:     printf("31m"); break;
        case GREEN:   printf("32m"); break;
        case YELLOW:  printf("33m"); break;
        case BLUE:    printf("34m"); break;
        case MAGENTA: printf("35m"); break;
        case CYAN:    printf("36m"); break;
        case WHITE:   printf("37m"); break;

        default:
            assert(0 && "Invalid color change_color");
            break;
    }
}
