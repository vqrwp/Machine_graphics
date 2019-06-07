#include "calculate_parametrical.h"

QPoint make_param(const QPoint &p1, const QPoint &p2, double t)
{
    QPoint res;
    res.setX(p1.x() + round((p2.x() - p1.x()) * t));
    res.setY(p1.y() + round((p2.y() - p1.y()) * t));
    return res;
}
