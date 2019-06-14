#ifndef MY_PAINTWIDGET_H
#define MY_PAINTWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QPair>
#include <QHash>

#include "functions.h"
#include "rotate.h"
#include "sign.h"
#include "swap.h"

#define X_MAX 600
#define Y_MAX 600
#define X_CENTER 300
#define Y_CENTER 300
#define DELAY_TIME 20

class canvas : public QWidget
{
    Q_OBJECT

private:
    QColor color_draw = QColor(Qt::red);
    QColor color_bg = QColor(Qt::white);
    QImage my_image;

    QHash <int, int> top;
    QHash <int, int> bottom;

    QPoint rotate_n_scale(point_3d &point, transform_params &p);
    void horizon(QPoint start, QPoint end, int flag);
    QPoint calculate_cross(QPoint &prev, QPoint &cur, QHash<int, int> &array);
    int is_visible(QPoint &p);

public:
    explicit canvas(QWidget *parent = nullptr);

    void set_draw_color(QColor &color);
    void clear(void);
    void draw_main_algo(surface_data &surf, transform_params &p);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // MY_PAINTWIDGET_H
