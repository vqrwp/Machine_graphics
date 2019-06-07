#include "canvas.h"
#include "normal_funcs.h"
#include "iostream"
#include "QDebug"

canvas::canvas(QWidget *parent) : QWidget(parent)
{
    picture = QImage(X_max, Y_max, QImage::Format_RGB32);
    picture.fill(color_bg);
    first_point_poly = true;
    first_point_cut = true;
}

void canvas::draw_point()
{
    QPainter painter(&picture);
    if (!first_point_poly)
    {
        painter.setPen(color_poly);
        painter.drawPoint(f_point_poly);
    }
    if (!first_point_cut)
    {
        painter.setPen(color_cut);
        painter.drawPoint(f_point_cut);
    }
}

void draw_polygon(QVector<QPoint> &arr, QImage *img, QPen &pen)
{
    QPainter painter(img);
    painter.setPen(pen);
    painter.drawPolygon(QPolygon(arr));
}

void canvas::draw_poly()
{
    if (polygon.empty())
        return;
    QPen pen(color_poly);
    draw_polygon(polygon, &picture, pen);
}

void canvas::draw_cut()
{
    if (cutter.empty())
        return;
    QPen pen(color_cut);
    draw_polygon(cutter, &picture, pen);
}

void canvas::draw_result()
{
    if (P.empty())
        return;
    QPen pen(color_result);
    pen.setWidth(2);
    draw_polygon(P, &picture, pen);
}

void canvas::draw_image()
{
    picture.fill(color_bg);
    draw_cut();
    draw_poly();
    draw_result();
}


void copy_arr(QVector<QPoint> &fst, const QVector<QPoint> &snd)
{
    fst.clear();
    for (int i = 0; i < snd.size(); i++)
        fst.append(QPoint(snd[i].x(), snd[i].y()));
}

// определение видимости метод из роджерса (третий)
int canvas::is_visible(QPoint &p, int &index)
{
    vector_type P1F1(p, cutter[index]); // вектор от точки р до первой точки ребра
    vector_type F1F2(cutter[index + 1], cutter[index]); // ребро
    vector_type res;
    if (direction == -1) // если направление обхода влево
    {
        // меняем местами чтобы поменять знак
        vector_mult(P1F1, F1F2, res); // то видимо если положительно
    }
    else // если направление обхода вправо
    {
        vector_mult(F1F2, P1F1, res); // то видимо если положительно
    }
    return SIGN(res.z);
}

// пересекаются ли отрезок AB и ребро под индексом
bool canvas::is_crossing(QPoint &A, QPoint &B, int &index)
{
    int visA = is_visible(A, index); // проверяем видимость точки А отн. ребра
    int visB = is_visible(B, index); // проверяем видимость точки В отн. ребра

    // если знаки векторных произведений разные, то пересекаются
    if ((visA > 0 && visB < 0) || (visA < 0 && visB > 0))
    {
        return true;
    }
    return false;
}

// подпрограмма вычисления точки пересечения двух отрезков - путем решения СЛАУ
// приравниваем икс и игрек компоненты двух параметрических ураавнений и решаем
// решаем уравнение например 3s = 3 - 3 * t, 2 * s = 2 * t; решение s = t = 1 / 2
QPoint calculate_crossing_point(QPoint &P1, QPoint &P2, QPoint &Q1, QPoint &Q2)
{
    double A1 = P2.x() - P1.x();
    double B1 = -(Q2.x() - Q1.x());
    double C1 = Q1.x() - P1.x();
    double A2 = P2.y() - P1.y();
    double B2 = -(Q2.y() - Q1.y());
    double C2 = Q1.y() - P1.y();
    double det = A1 * B2 - B1 * A2;
    double t = (C1 * B2 - B1 * C2) / det;
    return make_param(P1, P2, t);
}

void canvas::sutherland_hodgman()
{
    copy_arr(P, polygon);
    P.removeLast(); // массив вершин исходного многоугольника
    QVector<QPoint> Q; // массив вершин результирующего многоугольника
    QVector<QPoint> W; // массив вершин отсекающего окна
    copy_arr(W, cutter);

    QPoint A; // текущая точка многоугольника
    QPoint F; // первая точка проверемого отрезка
    QPoint S; // вторая точка проверяемого отрезка

    bool crossing;
    int rc = WORK;

    // проходимся по всем вершинам отсекателя
    for (int i = 0; i < W.size() - 1 && rc == WORK; i++)
    {
        Q.clear();

        // проходимся по всем вершинам многоугольника
        for (int j = 0; j < P.size(); j++)
        {
            A = P[j]; // запоминаем вершину
            if (j == 0) // если это первая вершина, запоминаем в F
            {
                F = A;
            }
            else // если не первая
            {
                // проверяем пересекаются ли ребро отсекателя под индексом i (i + 1, i)
                // и многоугольник SA
                crossing = is_crossing(S, A, i);
                if (crossing) // если пересекается
                {
                    if (!Q.isEmpty())
                    {
                        // проверка на дупликат точки
                        if (Q.last() != calculate_crossing_point(S, A, W[i], W[i + 1]))
                        {
                            Q.append(calculate_crossing_point(S, A, W[i], W[i + 1]));
                        }
                    }
                    else
                    {
                        // добавляем в очередь
                        Q.append(calculate_crossing_point(S, A, W[i], W[i + 1]));
                    }
                }
            }
            S = A; // запоминаем предыдущую вершину
            if (is_visible(S, i) >= 0) // смотрим видимость второй точки относительно ребра
            {
                // тот случай, когда точка находится внутри видимой стороны
                if (!Q.isEmpty()) // проверка на дупликат
                {
                    if (Q.last() != S)
                    {
                        Q.append(S);
                    }
                }

                else
                {
                    Q.append(S);
                }
            }
        }
        if (Q.size() == 0) // если не нужных точек
        {
            rc = EXIT;
        }
        else // не забываем обработать первую вершину с последней
        {
            crossing = is_crossing(S, F, i);
            if (crossing) // если пересекает
            {
                if (!Q.isEmpty())
                {
                    // проверяем на дупликат точки
                    if (Q.last() != calculate_crossing_point(S, F, W[i], W[i + 1]))
                    {
                        Q.append(calculate_crossing_point(S, F, W[i], W[i + 1]));
                    }
                }
                // добавляем в массив результата
                else
                {
                    Q.append(calculate_crossing_point(S, F, W[i], W[i + 1]));
                }
            }
            copy_arr(P, Q); // перезаписываем точки, теперь когда мы будем идти
            // по вершинам отсекателя, мы будем проверять эти точки
            qDebug() << P;
        }
    }
}

void canvas::add_point_poly(QPoint p)
{
    if (first_point_poly)
    {
        first_point_poly = false;
        f_point_poly = p;
        draw_point();
    }
    else
    {
        QPainter painter(&picture);
        painter.setPen(color_poly);
        painter.drawLine(polygon.last(), p);
    }
    polygon.append(p);
}

void canvas::add_point_cut(QPoint p)
{
    if (first_point_cut)
    {
        first_point_cut = false;
        f_point_cut = p;
        draw_point();
    }
    else
    {
        QPainter painter(&picture);
        painter.setPen(color_cut);
        painter.drawLine(cutter.last(), p);
    }
    cutter.append(p);
}

void canvas::check_false_edges()
{
    if (P.empty())
        return;
    for (int i = 0; i < P.size() - 1; i++)
    {
        for (int j = 0; j < P.size() - 1; j++)
        {
            if (j != i)
            {
                vector_type A(P[i], P[i + 1]);
                vector_type B(P[j], P[j + 1]);
                int k = j - 1;
                if (is_crossing(P[i], P[i + 1], k))
                {
                    if (((A.x / B.x) == (A.y / B.y)) && ((A.y / B.y) == (A.z / B.z)))
                    {
                        if (A.x > B.x && A.y > B.y)
                        {
                            P.remove(i);
                        }
                        else
                        {
                            P.remove(j);
                        }
                    }
                }
            }
        }
    }
}

void canvas::cut_result()
{
    sutherland_hodgman();
    //check_false_edges();
    draw_result();
}

void canvas::clear_all()
{
    polygon.clear();
    cutter.clear();
    P.clear();
    draw_image();
    first_point_cut = true;
    first_point_poly = true;
}

void canvas::clear_poly()
{
    polygon.clear();
    P.clear();
    draw_image();
    first_point_poly = true;
}

void canvas::clear_cutter()
{
    cutter.clear();
    P.clear();
    draw_image();
    first_point_cut = true;
}

bool canvas::cutter_is_empty()
{
    return (cutter.isEmpty());
}

segment_type canvas::get_edge(int index) // ??
{
    return segment_type(cutter[index], cutter[index + 1]);
}

// когда нажимают ПКМ
int canvas::find_closest(QPoint &p)
{
    if (cutter.size() < 2)
    {
        return 0;
    }
    double min_distance = distance_to_point(cutter[0], cutter[1], p);
    int min_index = 0;
    double tmp;
    for (int i = 1; i < cutter.size() - 1; i++)
    {
        tmp = distance_to_point(cutter[i], cutter[i + 1], p);
        if (tmp < min_distance)
        {
            min_distance = tmp;
            min_index = i;
        }
    }
    return min_index;
}

// метод - проход по смежным ребрам
bool canvas::is_convex()
{
    if (cutter.size() < 3) // если меньше трех вершин то точно не замкнут
    {
        direction = 0;
        return false;
    }
    vector_type a = vector_type(cutter[1], cutter[0]); // запоминаем первое ребро
    vector_type b;
    vector_type mult_res;
    int sign = 0;

    for (int i = 1; i < cutter.size() - 1; i++)
    {
        b = vector_type(cutter[i + 1], cutter[i]); // запоминаем следующее ребро
        vector_mult(a, b, mult_res);

        if (sign == 0) // если это первый проход либо все знаки были нулями
        {
            sign = SIGN(mult_res.z); // запоминаем знак векторного произв.
        }

        int tmp = SIGN(mult_res.z); // текущий знак
        if (sign != tmp && mult_res.z)
        // если знаки не совпадают и вект. пр. != 0
        {
            direction = 0;
            return false; // то отсекатель невыпуклый
        }
        a = b; // запоминаем текущее ребро как предыдущее
    }
    direction = sign;
    return true;
}

void canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, picture);
}
