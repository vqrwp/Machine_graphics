#ifndef MY_PAINTWIDGET_H
#define MY_PAINTWIDGET_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <math.h>
#include "sign.h"
#include "swap.h"
#include <QStack>

#define DELAY_TIME 50
#define X_max 640
#define X_min 0
#define Y_max 600
#define Y_min 0

struct edge_type
{
    QPoint start;
    QPoint end;
};

struct ellipse_type
{
    QPoint center;
    int a;
    int b;
};

class my_paintwidget : public QWidget
{
    Q_OBJECT
private:
    QColor color_bound = QColor(Qt::black);
    QColor color_fill = QColor(Qt::green);
    QColor color_bg = QColor(Qt::white);
    QVector <edge_type> edges;
    QVector <ellipse_type> ellipses;
    QImage my_image;
    QPoint f_point;
    bool first_point;

    QPoint zatr_pix;


    void paint_on_image(void);
    void draw_edge(edge_type &e, QPainter &painter);
    void draw_bound(void);
    void draw_point(void);
    void fill_polygon_(QPainter &painter, bool slow);
    void draw_line_bre(QPoint &start, QPoint &end, QPainter &painter, QColor &color);
    void find_new_pixel(QStack<QPoint> &st, int x, int y, int xr);

public:
    explicit my_paintwidget(QWidget *parent = nullptr);

    void set_color_bound(QColor &color);
    void set_color_fill(QColor &color);
    void set_color_bg(QColor &color);

    QColor get_color_bound(void);
    QColor get_color_fill(void);
    QColor get_color_bg(void);

    void add_new_point(QPoint &point);
    void add_new_edge(edge_type edge);
    void add_first_pixel(QPoint &point);

    void clear(void);
    void update_all(void);

    void fill_polygon(bool slow);

    bool first_pixel = false;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // MY_PAINTWIDGET_H
