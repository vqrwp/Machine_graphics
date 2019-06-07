#ifndef STRUCTS_H
#define STRUCTS_H

#include "QPoint"

struct segment_type
{
    QPoint p1;
    QPoint p2;
    segment_type() {}
    segment_type(QPoint t1, QPoint t2)
    {
        p1 = t1;
        p2 = t2;
    }
};

struct vector_type
{
    int x;
    int y;
    int z;
    vector_type() {}
    vector_type(int tx, int ty, int tz = 0)
    {
        x = tx;
        y = ty;
        z = tz;
    }
    vector_type(QPoint p2, QPoint p1)
    {
        x = p2.x() - p1.x();
        y = p2.y() - p1.y();
        z = 0;
    }
    vector_type(segment_type seg)
    {
        x = seg.p2.x() - seg.p1.x();
        y = seg.p2.y() - seg.p1.y();
        z = 0;
    }
};

#endif // STRUCTS_H
