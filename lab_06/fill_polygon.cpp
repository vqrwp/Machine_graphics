#include "my_paintwidget.h"
#include <stdio.h>
#include <dos.h>
#include <windows.h>


void my_paintwidget::fill_polygon_(QPainter &painter, bool slow)
{
    QPoint tmp_pix; // текущий пиксель
    int tmp_x; // иксовая координата текущего пикселя
    int x, y; // координаты текущего пикселя
    int x_r, x_l;

    painter.setPen(color_fill); // настраиваем цвет пера
    QStack <QPoint> pixel_stack; // стек затравочных пикселей
    pixel_stack.push(zatr_pix); // первым пушим в стек затравочный пиксель
    while (!pixel_stack.isEmpty()) // пока стек не пустой
    {
        tmp_pix = pixel_stack.pop(); // достаем из стека пиксель

        // запоминаем его координаты
        x = tmp_pix.x();
        y = tmp_pix.y();

        painter.drawPoint(tmp_pix); // закрашиваем первый пиксель

        tmp_x = x;
        x += 1;

        // идем по иксу вправо пока не достигнем границы холста или цвета границы рисунка
        while (x < X_max && my_image.pixelColor(x, y) != color_bound)
        {
            painter.drawPoint(x, y); // рисуем точку
            x += 1;
        }

        x_r = x - 1; // запоминаем последнюю закрашенную справа
        x = tmp_x; // возвращаемся в затравочный пиксель
        x -= 1;

        // идем по иксу влево пока не достигнем границы холста или цвета границы рисунка
        while (x > X_min && my_image.pixelColor(x, y) != color_bound)
        {
            painter.drawPoint(x, y); // рисуем точку
            x -= 1;
        }

        x_l = x + 1; // запоминаем последнюю закрашенную слева

        find_new_pixel(pixel_stack, x_l, y + 1, x_r); // ищем новые пиксели снизу
        find_new_pixel(pixel_stack, x_l, y - 1, x_r); // ищем новые пиксели сверху

        if (slow)
        {
            Sleep(DELAY_TIME);
            repaint();
        }
    }
}

void my_paintwidget::fill_polygon(bool slow)
{
    QPainter painter(&my_image);
    fill_polygon_(painter, slow);
    painter.end();
    this->repaint();
}
