#include "distance.h"


double distance(QPoint &p1, QPoint &p2)
{
    return sqrt((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
}

double distance_to_point(QPoint &p1, QPoint &p2, QPoint &p)
{
    double p1p2 = distance(p1, p2);
    double p1p = distance(p1, p);
    double p2p = distance(p2, p);
    if (p1p2 * p1p2 + p1p * p1p < p2p * p2p)
        return p1p;
    if (p1p2 * p1p2 + p2p * p2p < p1p * p1p)
        return p2p;
    int dx = p2.x() - p1.x();
    int dy = p2.y() - p1.y();
    double d = fabs((p.y() - p1.y()) * dx - (p.x() - p1.x()) * dy) / p1p2;
    return d;
}
