#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#include "color_scheme_changer.h"

void graphic_printf(COLOR color, STYLE style, const char *st, ...) {
    assert(st);

    va_list argument_pointer;
    char *pointer;

    va_start               (argument_pointer, st);
    change_color_and_style (color, style);
    vprintf                (st, argument_pointer);
    printf                 ("\033[0m");
    va_end                 (argument_pointer);
}

void change_color_and_style(COLOR color, STYLE style) {
    printf("\033[%d;", style);

    switch(style) {
        case RESET:
        case BOLD:
        case UNDERLINE:
        case INVERSE:
        case BOLD_OFF:
        case UNDERLINE_OFF:
        case INVERSE_OFF:
            break;

        default:
            assert(0 && "Invalid style");
            break;
    }

    printf("%dm", color);
    switch(color) {
        case BLACK:
        case RED:
        case GREEN:
        case YELLOW:
        case BLUE:
        case MAGENTA:
        case CYAN:
        case WHITE:
            break;

        default:
            assert(0 && "Invalid color change_color");
            break;
    }
}
