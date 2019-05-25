#include "my_paintwidget.h"
#include <stdio.h>
#include <dos.h>
#include <windows.h>


void my_paintwidget::set_color_bound(QColor &color)
{
    color_bound = color;
    //paint_on_image();
}

void my_paintwidget::set_color_fill(QColor &color)
{
    color_fill = color;
    //paint_on_image();
}

void my_paintwidget::set_color_bg(QColor &color)
{
    color_bg = color;
    //paint_on_image();
}

QColor my_paintwidget::get_color_bound()
{
    return color_bound;
}

QColor my_paintwidget::get_color_fill()
{
    return color_fill;
}

QColor my_paintwidget::get_color_bg()
{
    return color_bg;
}
