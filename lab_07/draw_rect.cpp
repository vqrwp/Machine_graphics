#include "draw_rect.h"

void draw_rect(QPainter &painter, int &x, int &y, int &first_rect_x,
               int &first_rect_y, bool &flag_first_rect_touched, bool &flag_rect_set, QVector<int> &rect)
{
    painter.drawPoint(x, y);
    if (flag_first_rect_touched == false)
    {
        first_rect_x = x;
        first_rect_y = y;
        flag_first_rect_touched = true;
    }
    else
    {
        flag_first_rect_touched = false;
        flag_rect_set = true;
        int width_rect = x - first_rect_x;
        int height_rect = y - first_rect_y;
        painter.drawRect(first_rect_x, first_rect_y, width_rect, height_rect);
        rect.append(min(first_rect_x, first_rect_x + width_rect));
        rect.append(max(first_rect_x, first_rect_x + width_rect));
        rect.append(min(first_rect_y, first_rect_y + height_rect));
        rect.append(max(first_rect_y, first_rect_y + height_rect));
    }
}
