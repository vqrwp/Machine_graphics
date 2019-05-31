#include "get_summ_mults.h"

int logic_mult(int *array1, int *array2,  int size)
{
    int p = 0;
    for (int i = 0; i < size; i++)
    {
        p += array1[i] * array2[i];
    }
    return p;
}
