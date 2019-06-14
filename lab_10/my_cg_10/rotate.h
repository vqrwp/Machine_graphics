#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#include <math.h>

#define EPS 1e-50
struct point_3d
{
    double x;
    double y;
    double z;
};

struct transform_params
{
    double angle_x;
    double angle_y;
    double angle_z;
    double k;
};

void rotate_point(point_3d &point, transform_params &params);

#endif // MATH_MODULE_H
