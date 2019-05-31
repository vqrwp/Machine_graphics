#include "set_bits.h"

void set_bits(QVector<int> rect, QPoint point, int *array)
{
    int x = point.x();
    int y = point.y();
    x < rect[0] ? array[3] = 1 : array[3] = 0; // rect[0] = min x
    x > rect[1] ? array[2] = 1 : array[2] = 0; // rect[1] = max x
    y < rect[2] ? array[1] = 1 : array[1] = 0; // rect[2] = min y
    y > rect[3] ? array[0] = 1 : array[0] = 0; // rect[3] = max y
}
