#ifndef MY_PAINTWIDGET_H
#define MY_PAINTWIDGET_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <math.h>
#define SEP_X 320
#define DELAY_TIME 30

struct edge_type
{
    QPoint start;
    QPoint end;
};


class canvas : public QWidget
{
    Q_OBJECT
private:
    QColor color_bound = QColor(Qt::black);
    QColor color_fill = QColor(Qt::magenta);
    QColor color_bg = QColor(Qt::white);
    QColor color_sep = QColor(Qt::darkMagenta);
    int sep_pos_x = SEP_X;
    QPoint min_p;
    QPoint max_p;
    QVector <edge_type> edges;
    QImage cur_canvas;
    QPoint f_point;
    bool first_point;

    void wait_canv(bool slow);
    void paint_on_image(void);
    void draw_edge(edge_type &e, QPainter &painter);
    void draw_bound(void);
    void draw_point(void);
    void draw_sep(void);
    void calc_sep_place(void);
    void fill_polygon(QPainter &painter, bool slow);
    void draw_line_bre(QPoint &start, QPoint &end, QPainter &painter, QColor &color);
    void choose_color_n_draw_dot(int x, int y, QPainter &painter);

public:
    explicit canvas(QWidget *parent = nullptr);

    void set_color_bound(QColor &color);
    void set_color_fill(QColor &color);
    void set_color_bg(QColor &color);
    void set_color_sep(QColor &color);

    QColor get_color_bound(void);
    QColor get_color_fill(void);
    QColor get_color_bg(void);
    QColor get_color_sep(void);

    void add_new_point(QPoint &point);
    void add_new_edge(edge_type edge);

    void clear(void);

    void pre_fill_polygon(bool slow);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // MY_PAINTWIDGET_H
