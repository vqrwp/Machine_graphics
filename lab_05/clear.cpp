#include "canvas.h"

void canvas::clear()
{
    edges.clear();
    max_p.setX(-1);
    max_p.setY(-1);
    min_p.setX(-1);
    min_p.setY(-1);
    f_point.setX(-1);
    f_point.setY(-1);
    first_point = true;
    paint_on_image();
}
