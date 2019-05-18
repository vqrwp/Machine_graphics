#include "canvas.h"

void canvas::add_new_point(QPoint &point)
{
    first_point = true;
    f_point = point;
    paint_on_image();
}

void canvas::add_new_edge(edge_type edge)
{
    first_point = false;
    // start.y < end.y
    if (edge.start.y() > edge.end.y())
    {
        QPoint tmp = edge.start;
        edge.start = edge.end;
        edge.end = tmp;
    }
    edges.append(edge);
    paint_on_image();
}
