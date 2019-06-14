#include "canvas.h"

int canvas::is_visible(QPoint &p)
{
    int x = p.x();
    int y = p.y();
    if (y < top[x] && y > bottom[x])
        return 0;
    if (y >= top[x])
        return 1;
    return -1;
}
