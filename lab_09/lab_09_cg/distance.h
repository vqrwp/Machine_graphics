#ifndef DISTANCE_H
#define DISTANCE_H

#include "QPoint"
#include <math.h>
#define EPS 1e-10

double distance_to_point(QPoint &p1, QPoint &p2, QPoint &p);
double distance(QPoint &p1, QPoint &p2);

#endif // DISTANCE_H
