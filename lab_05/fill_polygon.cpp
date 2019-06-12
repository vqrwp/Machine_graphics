#include "canvas.h"
#include "iostream"
#include "windows.h"

void canvas::fill_polygon(QPainter &painter, bool slow)
{
    // ПЕРЕСЕЧЕНИЕ ОПРЕДЕЛЯЕТСЯ С ПОМОЩЬЮ АЛГОРИТМА ЦДА хз че ето чисто геометрически
    int tmp_start = 0;
    int sep = 0;
    for (int i = 0; i < edges.size(); i++)
    {
        int x_start = edges[i].start.x(), y_start = edges[i].start.y();
        int x_end = edges[i].end.x(), y_end = edges[i].end.y();
        bool xl = x_start < sep_pos_x;
        bool xr = x_end < sep_pos_x;
        int dx = x_end - x_start;
        int dy = y_end - y_start;
        double steep = ((double) dx) / dy; // tangent angle - тангенс угла наклона
        if (xl && xr) // x_start < sep_pos_x and x_end < sep_pos_x
        { // if all edge is lefter then the separation line
            for (int y = y_start; y < y_end; y++) // go right (to the right  boundary)
            {
                tmp_start = round(steep * (y - y_start) + x_start); // find the start dot of the x
                for (int x = tmp_start; x < sep_pos_x + 1; x++)
                {
                    choose_color_n_draw_dot(x, y, painter);
                }
                wait_canv(slow);
            }
        }
        else if (!xl && !xr) // x_start >= sep_pos_x and x_end >= sep_pos_x
        { // if all edge is righter then the separation line
            for (int y = y_start; y < y_end; y++) // go left (to the left  boundary)
            {
                tmp_start = round(steep * (y - y_start) + x_start);
                for (int x = tmp_start; x > sep_pos_x; x--)
                {
                    choose_color_n_draw_dot(x, y, painter);
                }
                wait_canv(slow);
            }
        }
        else if (xl && !xr) // x_start < sep_pos_x and x_end >= sep_pos_x
        { // if the edge is crossing the separation line
            sep = round((1 / steep) * (sep_pos_x - x_start) + y_start); //
            for (int y = y_start; y < sep; y++)
            {
                tmp_start = round(steep * (y - y_start) + x_start);
                for (int x = tmp_start; x < sep_pos_x + 1; x++) // x-es which are lefter then sep line
                    choose_color_n_draw_dot(x, y, painter);
                wait_canv(slow);
            }
            for (int y = sep; y < y_end; y++)
            {
                tmp_start = round(steep * (y - y_start) + x_start);
                for (int x = tmp_start - 1; x > sep_pos_x; x--) // x-es which are righter then sep line
                    choose_color_n_draw_dot(x, y, painter);
                wait_canv(slow);
            }
        }
        else // x_start >= sep_pos_x and x_end < sep_pos_x
        { // if the edge is crossing the separation line
            sep = round((1 / steep) * (sep_pos_x - x_start) + y_start);
            for (int y = y_start; y < sep; y++)
            {
                tmp_start = round(steep * (y - y_start) + x_start);
                for (int x = tmp_start - 1; x > sep_pos_x; x--)  // x-es which are righter then sep line
                    choose_color_n_draw_dot(x, y, painter);
                wait_canv(slow);
            }
            for (int y = sep; y < y_end; y++)
            {
                tmp_start = round(steep * (y - y_start) + x_start);
                for (int x = tmp_start; x < sep_pos_x + 1; x++) // x-es which are lefter then sep line
                    choose_color_n_draw_dot(x, y, painter);
                wait_canv(slow);
            }
        }
    }
}

void canvas::wait_canv(bool slow)
{
    if (slow)
    {
        Sleep(DELAY_TIME);
        repaint();
    }
}

void canvas::pre_fill_polygon(bool slow)
{
    paint_on_image();
    draw_sep();
    QPainter painter(&cur_canvas);
    fill_polygon(painter, slow);
    painter.end();
    this->repaint();
}

void canvas::choose_color_n_draw_dot(int x, int y, QPainter &painter)
{
    QColor color = cur_canvas.pixelColor(x, y);
    if (color == color_fill)
    {
        painter.setPen(color_bg);
        painter.drawPoint(x, y);
    }
    else if (color == color_bg)
    {
        painter.setPen(color_fill);
        painter.drawPoint(x, y);
    }
}
