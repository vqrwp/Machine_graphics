#include "my_paintwidget.h"
#include "iostream"
#include "math_module.h"

using namespace std;

my_paintwidget::my_paintwidget(QWidget *parent) : QWidget(parent)
{
    my_image = QImage(X_max, Y_max, QImage::Format_RGB32);
    my_image.fill(color_bg);
    first_point_seg = true;
}

void my_paintwidget::draw_point()
{
    QPainter painter(&my_image);
    if (first_point_seg)
    {
        painter.setPen(color_segments);
        painter.drawPoint(f_point_seg);
    }
    if (first_point_cut)
    {
        painter.setPen(color_cut);
        painter.drawPoint(f_point_cut);
    }
}

void draw_lines(QVector<segment_type> &arr, QImage *img, QPen &pen)
{
    QPainter painter(img);
    painter.setPen(pen);
    for (int i = 0; i < arr.size(); i++)
        painter.drawLine(arr[i].p1, arr[i].p2);
}

void my_paintwidget::draw_segments()
{
    QPen pen(color_segments);
    draw_lines(segments, &my_image, pen);
}

void my_paintwidget::draw_cut()
{
    if (cut_params.empty())
        return;
    QPen pen(color_cut);
    draw_lines(cut_params, &my_image, pen);
}

void my_paintwidget::draw_result()
{
    QPen pen(color_result);
    pen.setWidth(2);
    draw_lines(result, &my_image, pen);
}

void my_paintwidget::draw_image()
{
    my_image.fill(color_bg);
    draw_segments();
    draw_result();
    draw_cut();
}


void my_paintwidget::calculate_normals()
{
    vector_type tmp;
    for (int i = 0; i < cut_params.size(); i++)
    {
        tmp = vector_type(cut_params[i]);
        if (direction == 1) // если направлено вправо по часовой
        {
            normals.push_back(vector_type(-tmp.y, tmp.x));
        }
        else // если направлено влево против часовой
        {
            normals.push_back(vector_type(tmp.y, -tmp.x));
        }
    }
}

// seg - текущий отрезок
void my_paintwidget::Kirus_Beck(const segment_type seg)
{
    int flag = 1;
    double tn = 0, tv = 1; // начальные значения, как будто отрезок видим
    double t_cur;
    segment_type cur_edge; // QPoint p1; QPoint p2;

    vector_type D = vector_type(seg); // D = P2 - P1

    vector_type W;
    int D_scalar, W_scalar; // будущее скалярное произведение

    // пока не пройдем все ребра многоугольника
    for (int i = 0; i < cut_params.size() && flag == 1; i++)
    {
        cur_edge = cut_params[i];

        W = vector_type(seg.p1, cur_edge.p1); // W = P1 - f(i)

        // скалярное произведение D и W на вектор внутренней нормали
        D_scalar = scalar_mult(D, normals[i]);
        W_scalar = scalar_mult(W, normals[i]);

        if (D_scalar == 0) // либо отрезок параллелен ребру, либо выродился в точку
        {
            if (W_scalar < 0) // точка находится вне окна, выходим
            {
                flag = 0;
            }
        }
        else
        {
            // вычисляем текущее t
            t_cur = - W_scalar / (double)D_scalar;
            if (D_scalar > 0) // поиск нижнего предела
            {
                if (t_cur > 1) // не входит в наш интервал (от 0 до 1)
                // если выйти за эти границы, то получится бесконечн. прямая
                {
                    flag = 0;
                }
                else
                {
                    // выбираем максимальное из т текущего и т нижнего
                    tn = (tn > t_cur) ? tn : t_cur;
                }
            }
            else // Д <= 0, поиск верхнего предела
            {
                if (t_cur < 0) // не входит в наш интервал (от 0 до 1)
                // если выйти за эти границы, то получится бесконечн. прямая
                {
                    flag = 0;
                }
                else
                {
                    // выбираем минимальное из т текущего и т верхнего
                    tv = (tv < t_cur) ? tv : t_cur;
                }
            }
        }
    }
    if (flag != 0)
    {
        if (tn <= tv) // если т нижнее меньше или равно т верхнего,
        // то отрезок частично или полностью видим
        {
            // вычисляем точки отрезка по параметрическому уравнению
            QPoint p1 = calculate_P(seg.p1, seg.p2, tn);
            QPoint p2 = calculate_P(seg.p1, seg.p2, tv);

            segment_type res = segment_type(p1, p2);

            // пушим в список результирующих отрезков
            result.push_back(res);
        }
    }
}

void my_paintwidget::calculate_results()
{
    calculate_normals();
    for (int i = 0; i < segments.size(); i++)
    {
        Kirus_Beck(segments[i]);
    }
}

void my_paintwidget::add_segment(segment_type segment)
{
    first_point_seg = false;
    segments.append(segment);
    draw_segments();
}

void my_paintwidget::add_point_seg(QPoint p)
{
    f_point_seg = p;
    first_point_seg = true;
    draw_point();
}

void my_paintwidget::add_cut_seg(segment_type p)
{
    first_point_cut = false;
    cut_params.push_back(p);
    draw_cut();
}

void my_paintwidget::add_point_cut(QPoint p)
{
    f_point_cut = p;
    first_point_cut = true;
    draw_point();
}

void my_paintwidget::cut_result()
{
    calculate_results();
    draw_result();
}

void my_paintwidget::clear_all()
{
    segments.clear();
    cut_params.clear();
    result.clear();
    normals.clear();
    draw_image();
}

void my_paintwidget::clear_segments()
{
    segments.clear();
    result.clear();
    draw_image();
}

void my_paintwidget::clear_cutter()
{
    cut_params.clear();
    normals.clear();
    draw_image();
}

bool my_paintwidget::cutter_is_empty()
{
    return (cut_params.isEmpty());
}

segment_type my_paintwidget::get_edge(int index)
{
    return cut_params[index];
}

int my_paintwidget::find_closest(QPoint &p)
{
    double min_distance = distance_to_point(cut_params[0].p1, cut_params[0].p2, p);
    int min_index = 0;
    double tmp;
    for (int i = 0; i < cut_params.size(); i++)
    {
        tmp = distance_to_point(cut_params[i].p1, cut_params[i].p2, p);
        if (tmp < min_distance)
        {
            min_distance = tmp;
            min_index = i;
        }
    }
    return min_index;
}


int check_direction(QVector<int> signs)
{
    int size = signs.size();
    int CountOfPositive = 0;

    for (int i = 0; i < size; i++)
    {
        if (signs[i] >= 0)
        {
            CountOfPositive++;
        }
    }
    if (CountOfPositive == size) // если все знаки векторных
    // произведений положительные
    {
        return 1; // Right
    }
    else
    {
        return -1; // Left
    }
}

// проверяем одинаковые ли знаки у векторных произведений
bool check_signs(QVector<int> signs)
{
    int size = signs.size();
    int curr;
    int prev = signs[0];
    int CountOfZeroSigns = 0;

    if (prev == 0) // векторное произведение равно 0 когда стороны параллельны
    {
        CountOfZeroSigns++;
    }
    bool flag = true; // все знаки одинаково направлены

    for (int i = 1; i < size; i++)
    {
        curr = signs[i];
        if (curr != prev && curr != 0 && prev != 0)
        {
            flag = false; // знаки разные
        }
        else if (curr == 0)
        {
            CountOfZeroSigns++;
        }
        prev = curr;
    }
    if (CountOfZeroSigns == size)
    {
        flag = false; // all of signs are zero
    }
    return flag;
}

int sign(int VectorMult)
{
    if (VectorMult == 0)
    {
        return 0;
    }
    else
    {
        return VectorMult / abs(VectorMult);
    }
}

bool my_paintwidget::is_convex()
{
    int size = cut_params.size(); // количество ребер
    QVector<int> signs; // массив знаков векторных произведений

    bool flag = false;

    vector_type mult_res;
    if (size > 2) // у выпуклого многоуг. минимум 3 стороны
    {
        for (int i = 1; i < size - 1; i++)
        {
            vector_type a = vector_type(cut_params[i - 1]);
            vector_type b = vector_type(cut_params[i]);

            vector_mult(a, b, mult_res); // векторное произведение смежных сторон

            int MultSign = sign(mult_res.z); // запоминаем его знак
            signs.append(MultSign);
        }
        // не забываем про последнюю вершину

        vector_type a = vector_type(cut_params[size - 1]);
        vector_type b = vector_type(cut_params[0]);
        vector_mult(a, b, mult_res);
        int MultSign = sign(mult_res.z);
        signs.append(MultSign); // запоминаем знак последнего вект. произ-ия

        flag = check_signs(signs); // и проверяем, совпадают ли все знаки

        // если знаки совпадают, то вычисляем направление
        if (flag == true)
        {
            direction = check_direction(signs); // 1 (вправо) если положительные, 0 (лево) если отриц.
        }
    }
    return flag;
}

void my_paintwidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, my_image);
}
