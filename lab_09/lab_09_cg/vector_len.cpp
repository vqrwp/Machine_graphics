#include "vector_len.h"

double vector_len(vector_type &vect)
{
    return sqrt(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
}
