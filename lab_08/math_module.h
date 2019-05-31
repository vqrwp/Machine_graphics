#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#include "my_paintwidget.h"
void vector_mult(vector_type &a, vector_type &b, vector_type &res);
int scalar_mult(vector_type &a, vector_type &b);
double distance(QPoint &p1, QPoint &p2);
double distance_to_point(QPoint &p1, QPoint &p2, QPoint &p);
QPoint calculate_P(const QPoint &p1, const QPoint &p2, double t);

#endif // MATH_MODULE_H
