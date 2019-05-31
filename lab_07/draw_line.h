#ifndef DRAW_LINE_H
#define DRAW_LINE_H
#include "qpainter.h"

void draw_line(QPainter &painter, int &x, int &y, int &first_x, int &first_y, bool &flag_first_touched, QVector<QPoint> &lines);

#endif // DRAW_LINE_H
