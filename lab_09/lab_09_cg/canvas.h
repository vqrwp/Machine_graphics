#ifndef MY_PAINTWIDGET_H
#define MY_PAINTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <math.h>
#include "structs.h"
#include "scalar_mult.h"
#include "vector_mult.h"
#include "vector_len.h"
#include "calculate_parametrical.h"
#include "distance.h"

#define X_max 791
#define X_min 0
#define Y_max 681
#define Y_min 0

#define WORK -5
#define EXIT -10

#define SIGN(x) ((x) > 0) ? 1 : (((x) < 0) ? -1 : 0)


class canvas : public QWidget
{
    Q_OBJECT

private:
    QColor color_bg = QColor(Qt::white);
    QColor color_poly = QColor(Qt::black);
    QColor color_cut = QColor(Qt::magenta);
    QColor color_result = QColor(Qt::cyan);

    QVector<QPoint> cutter;
    QVector<QPoint> polygon;
    QVector<QPoint> P; // result

    bool first_point_poly;
    QPoint f_point_poly;

    bool first_point_cut;
    QPoint f_point_cut;

    int direction;

    QImage picture;

    void draw_point();
    void draw_poly();
    void draw_cut();
    void draw_result();
    void draw_image();

    void sutherland_hodgman();

    bool is_crossing(QPoint &seg_b, QPoint &seg_e, int &index);
    int is_visible(QPoint &p, int &index);

public:
    explicit canvas(QWidget *parent = nullptr);

    segment_type get_edge(int index);
    void add_point_poly(QPoint p);
    void add_point_cut(QPoint p);
    int find_closest(QPoint &p);
    void clear_all();
    void clear_poly();
    void clear_cutter();
    bool cutter_is_empty();
    bool is_convex();
    void cut_result();
    void check_false_edges();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // MY_PAINTWIDGET_H
