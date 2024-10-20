#ifndef COLOR_SCHEME_CHANGER_H_
#define COLOR_SCHEME_CHANGER_H_

/// @brief Перечислимый тип цветов
/// @details Перечислимый тип цветов для функции graphic_printf().

enum COLOR {
    BLACK   = 30, ///<black nigga
    RED     = 31, ///<красный
    GREEN   = 32, ///<зеленый
    YELLOW  = 33, ///<желтый
    BLUE    = 34, ///<голубой
    MAGENTA = 35, ///<магента
    CYAN    = 36, ///<бирюзовый
    WHITE   = 37, ///<белый
};

/// @brief Перечислимый тип стилей
/// @details Перечислимый тип стилей для функции graphic_printf().

enum STYLE {
    RESET         = 0, ///<начальный текст
    BOLD          = 1, ///<включить жирный шрифт
    UNDERLINE     = 4, ///<подчеркнутый текст
    INVERSE       = 7, ///<инверсия
    BOLD_OFF      = 21, ///<выключить жирный шрифт
    UNDERLINE_OFF = 24, ///<выключить подчеркивание
    INVERSE_OFF   = 27, ///<выключить инверсию
};

/// @brief Меняет цвет и стиль вводимой строки
/// @details Меняет цвет и стиль вводимой строки из функции
///          graphic_printf().
/// @param color цвет
/// @param style стиль

void change_color_and_style(COLOR color, STYLE style);

/// @brief Выводит строку с цветом и стилем
/// \details Выводит строку с цветом color и стилей style
///          в поток вывода.
/// @param st строка
/// @param color цвет
/// @param style стиль
/// @param  Строка с цветом color и стилем style

void graphic_printf(COLOR color, STYLE style, const char *st, ...);

#endif
