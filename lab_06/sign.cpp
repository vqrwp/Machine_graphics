#include "sign.h"



int sign(double x)
{
    if (x > EPS)
        return 1;
    else if (x == EPS)
        return 0;
    return -1;
}
