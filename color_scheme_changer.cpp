#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#include "color_scheme_changer.h"

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
    printf("\033[%d;", color);

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

    switch(color) {
        case BLACK:   printf("30m"); break;
        case RED:     printf("31m"); break;
        case GREEN:   printf("32m"); break;
        case YELLOW:  printf("33m"); break;
        case BLUE:    printf("34m"); break;
        case MAGENTA: printf("35m"); break;
        case CYAN:    printf("36m"); break;
        case WHITE:   printf("37m"); break;

        // TODO same as style
        default:
            assert(0 && "Invalid color change_color");
            break;
    }
}
