#include "rotate.h"
#include "canvas.h"

///*
void rotate_point_x(point_3d &point, double angle)
{
    if (fabs(angle) >= EPS)
    {
        double tmp_x = point.x, tmp_y = point.y, tmp_z = point.z;
        point.x = tmp_x;
        point.y = tmp_y * cos(angle) - tmp_z * sin(angle);
        point.z = tmp_y * sin(angle) + tmp_z * cos(angle);
    }
}

void rotate_point_y(point_3d &point, double angle)
{
    if (fabs(angle) >= EPS)
    {
        double tmp_x = point.x, tmp_y = point.y, tmp_z = point.z;
        point.x = tmp_x * cos(angle) - tmp_z * sin(angle);
        point.y = tmp_y;
        point.z = tmp_x * sin(angle) + tmp_z * cos(angle);
    }
}

void rotate_point_z(point_3d &point, double angle)
{
    if (fabs(angle) >= EPS)
    {
        double tmp_x = point.x, tmp_y = point.y, tmp_z = point.z;
        point.x = tmp_x * cos(angle) - tmp_y * sin(angle);
        point.y = tmp_x * sin(angle) + tmp_y * cos(angle);
        point.z = tmp_z;
    }
}

void rotate_point(point_3d &point, transform_params &params)
{
    double k = (double)M_PI / 180; // 1 градус
    // поворот всех точек на указанное количство градусов
    rotate_point_x(point, params.angle_x * k);
    rotate_point_y(point, params.angle_y * k);
    rotate_point_z(point, params.angle_z * k);
}
//*/
/*
void rotate_point_x(point_3d &point, double angle)
{
    angle = angle * M_PI / 180;
    double buf = point.y;
    point.y = cos(angle) * point.y - sin(angle) * point.z;
    point.z = cos(angle) * point.z + sin(angle) * buf;
}

void rotate_point_y(point_3d &point, double angle)
{
    angle = angle * M_PI / 180;
    double buf = point.x;
    point.x = cos(angle) * point.x - sin(angle) * point.z;
    point.z = cos(angle) * point.z + sin(angle) * buf;
}

void rotate_point_z(point_3d &point, double angle)
{
    angle = angle * M_PI / 180;
    double buf = point.x;
    point.x = cos(angle) * point.x - sin(angle) * point.y;
    point.y = cos(angle) * point.y + sin(angle) * buf;
}
void rotate_point(point_3d &point, transform_params &params)
{
    double x_tmp = point.x;
    double y_tmp = point.y;
    double z_tmp = point.z;
    rotate_point_x(point, params.angle_x);
    rotate_point_y(point, params.angle_y);
    rotate_point_z(point, params.angle_z);
    //point.x = static_cast<int>(round(x_tmp * params.k + X_CENTER));
    //point.y = static_cast<int>(round(y_tmp * params.k + Y_CENTER));
}
*/
