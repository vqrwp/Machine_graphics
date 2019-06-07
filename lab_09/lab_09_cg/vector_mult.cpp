#include "vector_mult.h"

void vector_mult(vector_type &a, vector_type &b, vector_type &res)
{
    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;
}
