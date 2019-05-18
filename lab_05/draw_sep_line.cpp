#include "canvas.h"

void canvas::draw_sep()
{
    QPainter painter(&cur_canvas);
    QPoint p1(sep_pos_x, min_p.y() - 10);
    QPoint p2(sep_pos_x, max_p.y() + 10);
    draw_line_bre(p1, p2, painter, color_sep);
}

void canvas::calc_sep_place() // calculate the right pos for the middle line
{
    if (edges.size() == 0) // if there are less then 2 dots
        return;
    int x_max = edges[0].start.x(), x_min = edges[0].start.x();
    int y_max = edges[0].start.y(), y_min = edges[0].start.y();
    for (int i = 0; i < edges.size(); i++)
    {
        if (x_max < edges[i].start.x())
            x_max = edges[i].start.x();
        if (x_max < edges[i].end.x())
            x_max = edges[i].end.x();
        if (x_min > edges[i].start.x())
            x_min = edges[i].start.x();
        if (x_min > edges[i].end.x())
            x_min = edges[i].end.x();
        if (y_max < edges[i].end.y())
            y_max = edges[i].end.y();
        if (y_min > edges[i].start.y())
            y_min = edges[i].start.y();
    }
    sep_pos_x = (x_max + x_min) * 0.5;
    max_p.setX(x_max);
    max_p.setY(y_max);
    min_p.setX(x_min);
    min_p.setY(y_min);
}
