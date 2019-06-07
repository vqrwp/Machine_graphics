#include "normal_funcs.h"

QPointF normalize(vector_type &vect)
{
    double len = vector_len(vect);
    if (fabs(len) < EPS)
        return QPoint(0, 0);
    QPointF res;
    res.setX((double)vect.x / len);
    res.setY(double(vect.y) / len);
    return res;
}

double vector_component(vector_type &v1, vector_type &v2)
{
    int scalar = scalar_mult(v1, v2);
    double v2_len = vector_len(v2);
    if (fabs(v2_len) < EPS)
        return 0;
    return (double)scalar / v2_len;
}

QPoint normal_point(QPoint &A, QPoint &B, QPoint &C)
{
    vector_type AB = vector_type(B, A);
    vector_type AC = vector_type(C, A);
    AB.z = 0;
    AC.z = 0;
    double tmp = vector_component(AC, AB);
    QPointF nAB = normalize(AB);
    nAB.setX(nAB.x() * tmp);
    nAB.setY(nAB.y() * tmp);
    return QPoint(fabs(nAB.x() + A.x()), fabs(nAB.y() + A.y()));
}
