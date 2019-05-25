#include "my_paintwidget.h"
#include <stdio.h>
#include <dos.h>
#include <windows.h>

void my_paintwidget::draw_line_bre(QPoint &start, QPoint &end, QPainter &painter, QColor &color)
{
    QPen pen(color);
    painter.setPen(pen);
    int x_start = start.x(), y_start = start.y();
    int x_end = end.x(), y_end = end.y();
    int dx = x_end - x_start;
    int dy = y_end - y_start;
    int x = x_start;
    int y = y_start;
    int sx = sign(dx);
    int sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);
    bool swap = (dx > dy ? false : true);
    if (swap)
        swap_num(dx, dy);
    double m = (double)dy / (double)dx;
    double e = m - 0.5;
    for (int i = 0; i < dx + 1; i++)
    {
        painter.drawPoint(x, y);
        if (e >= 0.0)
        {
            if (!swap)
                y += sy;
            else
                x += sx;
            e -= 1;
        }
        if (!swap)
            x += sx;
        else
            y += sy;
        e += m;
    }
}
