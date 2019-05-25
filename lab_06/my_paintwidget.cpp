#include "my_paintwidget.h"
#include <stdio.h>
#include <dos.h>
#include <windows.h>

my_paintwidget::my_paintwidget(QWidget *parent) : QWidget(parent)
{
    int image_height = Y_max;
    int image_width = X_max;
    my_image = QImage(image_width, image_height, QImage::Format_RGB32);
    my_image.fill(color_bg);
}

void my_paintwidget::paint_on_image()
{
    my_image.fill(color_bg);
    draw_point();
    draw_bound();
    this->repaint();
}

void my_paintwidget::draw_edge(edge_type &e, QPainter &painter)
{
    draw_line_bre(e.start, e.end, painter, color_bound);
}

void my_paintwidget::draw_bound()
{
    QPainter painter(&my_image);
    for (int i = 0; i < edges.size(); i++)
        draw_edge(edges[i], painter);
}

void my_paintwidget::draw_point()
{
    if (first_point)
    {
        QPainter painter(&my_image);
        painter.setPen(color_bound);
        painter.drawPoint(f_point);
    }
    if (first_pixel)
    {
        QPainter painter(&my_image);
        painter.setPen(color_fill);
        painter.drawPoint(zatr_pix);
    }
}

void my_paintwidget::add_new_point(QPoint &point)
{
    first_point = true;
    f_point = point;

    draw_point();
    //paint_on_image();
}

void my_paintwidget::add_new_edge(edge_type edge)
{
    first_point = false;
    // start.y < end.y
    if (edge.start.y() > edge.end.y())
    {
        QPoint tmp = edge.start;
        edge.start = edge.end;
        edge.end = tmp;
    }
    edges.append(edge);
    draw_bound();
    //paint_on_image();
}

void my_paintwidget::clear()
{
    edges.clear();
    ellipses.clear();
    zatr_pix.setX(-1);
    zatr_pix.setY(-1);
    f_point.setX(-1);
    f_point.setY(-1);
    first_point = true;
    first_pixel = false;
    paint_on_image();
}

void my_paintwidget::update_all()
{
    paint_on_image();
}

void my_paintwidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, my_image);
}
