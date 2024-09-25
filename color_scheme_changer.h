#ifndef COLOR_SCHEME_CHANGER_H_
#define COLOR_SCHEME_CHANGER_H_

/// @brief Перечислимый тип цветов
/// @details Перечислимый тип цветов для функции graphic_printf().

enum COLOR {
    BLACK   = 0, ///<black nigga
    RED     = 1, ///<красный
    GREEN   = 2, ///<зеленый
    YELLOW  = 3, ///<желтый
    BLUE    = 4, ///<голубой
    MAGENTA = 5, ///<магента
    CYAN    = 6, ///<бирюзовый
    WHITE   = 7, ///<белый
};

/// @brief Перечислимый тип стилей
/// @details Перечислимый тип стилей для функции graphic_printf().

enum STYLE {
    RESET         = 0, ///<начальный текст
    BOLD          = 1, ///<включить жирный шрифт
    UNDERLINE     = 2, ///<подчеркнутый текст
    INVERSE       = 3, ///<инверсия
    BOLD_OFF      = 4, ///<выключить жирный шрифт
    UNDERLINE_OFF = 5, ///<выключить подчеркивание
    INVERSE_OFF   = 6, ///<выключить инверсию
};

/// @brief Меняет цвет и стиль вводимой строки
/// @details Меняет цвет и стиль вводимой строки из функции
///          graphic_printf().
/// @param color цвет
/// @param style стиль

void change_color(COLOR color, STYLE style);

/// @brief Выводит строку с цветом и стилем
/// \details Выводит строку с цветом color и стилей style
///          в поток вывода.
/// @param st строка
/// @param color цвет
/// @param style стиль
/// @param  Строка с цветом color и стилем style

void graphic_printf(COLOR color, STYLE style, const char *st, ...);

#endif
