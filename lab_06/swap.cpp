#include "swap.h"

void swap_num(int &x, int &y)
{
    int tmp = x;
    x = y;
    y = tmp;
}
