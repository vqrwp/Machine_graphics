#include "canvas.h"

void canvas::set_color_bound(QColor &color)
{
    color_bound = color;
    paint_on_image();
}

void canvas::set_color_fill(QColor &color)
{
    color_fill = color;
    paint_on_image();
}

void canvas::set_color_bg(QColor &color)
{
    color_bg = color;
    paint_on_image();
}
