#include "scalar_mult.h"


int scalar_mult(vector_type &a, vector_type &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
