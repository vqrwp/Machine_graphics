#include "canvas.h"
#include "QDebug"

canvas::canvas(QWidget *parent) : QWidget(parent)
{
    my_image = QImage(X_MAX, Y_MAX, QImage::Format_RGB32);
    my_image.fill(color_bg);
}

void canvas::set_draw_color(QColor &color)
{
    color_draw = color;
}

void canvas::clear()
{
    top.clear();
    bottom.clear();
    my_image.fill(color_bg);
}

QPoint canvas::rotate_n_scale(point_3d &point, transform_params &angles)
{
    rotate_point(point, angles); // поворот координат точки поинт по градусам
    int x = int(point.x * angles.k); // масштабирование
    int y = int(point.y * angles.k); // масштабирование
    return QPoint(x, y);
}

void center_point(QPoint &p)
{
    p.setX(p.x() + X_CENTER);
    p.setY(p.y() + Y_CENTER);
}

// эта подпрограмма использует линейную интерполяцию для заполнения
// массивов горизонтов между X1 и Х2
void canvas::horizon(QPoint start, QPoint end, int flag)
{
    QPainter painter(&my_image);
    painter.setPen(QPen(color_draw));

    if (start.x() > end.x())
        qSwap(start, end);

    int dx = end.x() - start.x();
    int dy = end.y() - start.y();

    if (dx == 0) // чтобы не столкнуться с делением на 0
    // обрабатываем этот случай отдельно
    {
        // заполняем массив горизонтов
        top[start.x()] = qMax(start.y(), top[start.x()]);
        bottom[start.x()] = qMin(start.y(), bottom[start.x()]);

        if (flag)
        {
            center_point(start);
            center_point(end);
            painter.drawLine(start, end);
        }
    }
    else
    {
        QPoint prev(start.x(), start.y());
        center_point(prev); // отцентровываем точку

        double steep = (dy) / double (dx); // вычисляем наклон линии
        for (int i = start.x(); i <= end.x(); i++)
        {
            // вычисляем соответствующий игрек
            int y = int(steep * (i - start.x()) + start.y());

            // заполняем массив горизонтов
            // если текущая координата y больше большей или меньше меньшей,
            // то запоминнаем её
            top[i] = qMax(top[i], y);
            bottom[i] = qMin(bottom[i], y);

            QPoint cur(i, y);
            center_point(cur); // отцентровываем точку

            if (flag)
            {
                painter.drawLine(prev, cur);
            }
            prev = cur;
        }
    }
}

// вычисляем пересечение текущей прямой с горизонтом
// prev - начальная точка прямой, cur - конечная, horizont - горизонт
QPoint canvas::calculate_cross(QPoint &prev, QPoint &cur, QHash<int, int> &horizont)
{
    int res_x = prev.x(),
        res_y = prev.y();

    int hor_y_prev = horizont.value(prev.x()), // выбираем из хэша горизонта игрек соотв. предыдущему иксу
        hor_y_cur = horizont.value(cur.x()); // выбираем из хэша горизонта игрек соотв. текущему иксу

    int dx = cur.x() - prev.x();

    int dy_cur = cur.y() - prev.y(); // приращение отрезка по игреку
    int dy_prev = hor_y_cur - hor_y_prev; // приращение горизонта по игреку

    if (dx == 0) // если нет приращения по икс запоминаем как рез-т текущие точки
    {
        res_x = cur.x();
        res_y = horizont.value(res_x);
    }
    else if (prev.y() == hor_y_prev && cur.y() == hor_y_cur) // если координаты предыдущего игрека
    // были горизонтом
    {
        res_x = prev.x(); // то запоминаем предыдущие координаты (первая вершина отрезка)
        res_y = prev.y();
    }
    else
    {
        double steep = dy_cur / double(dx); // угол наклона отрезка
        if (dy_cur != dy_prev) // если приращение горизонта и отрезка не равны
        {
            // вычисляем точку пересечения
            res_x = prev.x() - int(((prev.y() - hor_y_prev) * dx / double(dy_cur - dy_prev)));
            res_y = int((res_x - prev.x()) * steep + prev.y());  // dx_new * steep + prev.y = d_y_new + prev.y
        }
    }
    return QPoint(res_x, res_y);
}

void canvas::draw_main_algo(surface_data &surface, transform_params &p)
{
    this->clear();
    for (double i = -X_CENTER; i <= X_CENTER; i++)
    {
        // иксовая координата это как бы индекс
        top.insert(int(i), -Y_CENTER); // по умолчанию кладем в каждые верхние границы горизонта отриц.
        // отступ до цента по игреку
        bottom.insert(int(i), Y_CENTER); // по умолчанию кладем в каждые нижние границы горизонта
        // отступ до цента по игреку
    }

    QPoint intersection; // точка пересечения
    QPoint previous; // предыдущая рассматриваемая точка
    QPoint current; // текущая рассматриваемая точка
    QPoint left(-1, -1); // значения дл левого бокового ребра
    QPoint right(-1, -1); // значения для правого бокового ребра

    bool flag_first_l = true;
    bool flag_first_r = true;
    int prev_visible; // видимость предыдущей точки
    int cur_visible; // видимость текущей точки

    double ax = p.angle_x;

    for (double z = surface.z_max; z >= surface.z_min; z -= surface.d_z)
    {
        double nz = z;

        double x_prev = surface.x_min;
        double y_prev = surface.func(x_prev, nz);
        point_3d prev = {x_prev, y_prev, nz};
        previous = rotate_n_scale(prev, p); // поворот и масштабирование первой точки

        // ОБРАБОТКА ЛЕВОГО БОКОВОГО РЕБРА
        if (!flag_first_l) // если это не первая точка
        {
            // эта подпрограмма использует линейную интерполяцию для заполнения
            // массивов горизонтов между X1 и Х2 (left and previous)
            horizon(left, previous, 1); // то обрабатываем горизонт
        }

        flag_first_l = false; // теперь у нас обрабатывается не первая точка
        left.setX(previous.x());
        left.setY(previous.y());

        // является ли точка видимой отн. максимума или минимума
        // -1 если меньше ИЛИ РАВНО мин,
        // 1 если больше ИЛИ РАВНО макс, 0 если нет
        prev_visible = is_visible(previous);

        double x, y;
        for (x = surface.x_min; x <= surface.x_max; x += surface.d_x)
        {
            // вычисляем координаты текущей точки, поворачиваем и масштабирууем ее
            y = surface.func(x, nz);
            point_3d cur = {x, y, nz};
            current = rotate_n_scale(cur, p);

            cur_visible = is_visible(current);

            // если точки имеют одинаковую видимость и они видимы
            if (prev_visible == cur_visible && (prev_visible == -1 || prev_visible == 1))
                horizon(previous, current, 1); // то соединим их
            else
            {
                if (cur_visible == 0) // если текущая точка невидима
                {
                    if (prev_visible != 0) // а предыдущая видима
                    {
                        // то в зависимости от того, видима она сверху или снизу,
                        // найти пересечение текущей прямой с горизонтом
                        intersection = (prev_visible == 1) ? calculate_cross(previous, current, top) :
                                                             calculate_cross(previous, current, bottom);
                        horizon(previous, intersection, 1); // и соединить предыдщую точку
                        // с пересечением
                    }
                }
                else if (cur_visible == 1) // если текущая точка видима сверху
                {
                    if (prev_visible == 0) // а предыдущая не видима
                    {
                        // соединяем пересечение максимума предыдущей с текущей
                        intersection = calculate_cross(previous, current, top);
                        horizon(intersection, current, 1);
                    }
                    else // и предыдущая тоже видима
                    {
                        // то соединяем пересечение линии с горизонтом с двух сторон
                        intersection = calculate_cross(previous, current, bottom);
                        horizon(previous, intersection, 1);
                        intersection = calculate_cross(previous, current, top);
                        horizon(intersection, current, 1);
                    }
                }
                else // если текущая точка видима снизу
                {
                    if (prev_visible == 0) // а предыдущая не видима
                    {
                        // соединяем пересечение предыдущей с секущей снизу
                        intersection = calculate_cross(previous, current, bottom);
                        horizon(intersection, current, 1);
                    }
                    else // если предыдущая видима
                    {
                        // соединяем с двух сторон
                        intersection = calculate_cross(previous, current, top);
                        horizon(previous, intersection, 1);
                        intersection = calculate_cross(previous, current, bottom);
                        horizon(intersection, current, 1);
                    }
                }
            }
            // теперь текущая точка - это предыдущая
            prev_visible = cur_visible;
            previous.setX(current.x());
            previous.setY(current.y());
        }

        // ОБРАБОТКА ПРАВОГО БОКОВОГО РЕБРА
        if (!flag_first_r)
        {
            horizon(right, current, 1);
        }
        flag_first_r = false;
        right.setX(current.x());
        right.setY(current.y());
    }
}

void canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, my_image);
}
