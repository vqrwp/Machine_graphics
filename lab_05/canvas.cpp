#include "canvas.h"
#include <stdio.h>
#include <dos.h>
#include <windows.h>

canvas::canvas(QWidget *parent) : QWidget(parent)
{
    int image_height = 600;
    int image_width = 640;
    cur_canvas = QImage(image_width, image_height, QImage::Format_RGB32);
    cur_canvas.fill(color_bg);
}

void canvas::paint_on_image()
{
    calc_sep_place();
    cur_canvas.fill(color_bg);
    draw_point();
    draw_bound();
    this->repaint();
}

void canvas::draw_edge(edge_type &e, QPainter &painter)
{
    draw_line_bre(e.start, e.end, painter, color_bound);
}

void canvas::draw_bound()
{
    QPainter painter(&cur_canvas);
    for (int i = 0; i < edges.size(); i++)
        draw_edge(edges[i], painter);
}

void canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, cur_canvas);
}


