#include "canvas.h"

void canvas::draw_point()
{
    if (first_point)
    {
        QPainter painter(&cur_canvas);
        painter.setPen(color_bound);
        painter.drawPoint(f_point);
    }
}
