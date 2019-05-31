#ifndef DRAW_POLYGON_H
#define DRAW_POLYGON_H
#include "qpainter.h"
#include "min_n_max.h"

void draw_rect(QPainter &painter, int &x, int &y, int &first_rect_x,
               int &first_rect_y, bool &flag_first_rect_touched, bool &flag_rect_set, QVector<int> &rect);

#endif // DRAW_POLYGON_H
