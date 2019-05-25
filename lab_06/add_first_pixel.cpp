#include "my_paintwidget.h"
#include <stdio.h>
#include <dos.h>
#include <windows.h>


void my_paintwidget::add_first_pixel(QPoint &point)
{
    first_pixel = true;
    zatr_pix = point;
    draw_point();
    //paint_on_image();
}
