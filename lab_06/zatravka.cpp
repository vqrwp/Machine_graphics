#include "my_paintwidget.h"
#include <stdio.h>
#include <dos.h>
#include <windows.h>

// pixel_stack, x_l, y, x_r
void my_paintwidget::find_new_pixel(QStack<QPoint> &st, int xl, int y, int xr)
{
    if (y <= Y_min || y >= Y_max) // если вышло за границы холста - выход
        return;

    bool flag = false;
    QColor tmp_color;
    QPoint tmp_pix;

    int tmp_x; // иксовая координата текущего пикселя

    while (xl <= xr)
    {
        flag = false;
        tmp_color = my_image.pixelColor(xl, y); // запоминаем цвет фона - 1 пиксель незакрашенной строки

        // идем слева направо пока: либо не граница, либо не правый пиксель, либо не цвет границы, либо не цвет закраски
        while (xl < X_max  && xl <= xr && tmp_color != color_bound && tmp_color != color_fill)
        {
            if (!flag)
                flag = true; // на этой строке есть нужные пиксели

            xl += 1;
            tmp_color = my_image.pixelColor(xl, y); // запоминаем цвет фона - текущий пиксель незакрашенной строки
        }

        if (flag) // если мы продвинулись хотя бы на один пиксель вправо
        {
            tmp_color = my_image.pixelColor(xl, y);

            // если выход из вайла был по причине достижения правого пикселя
            if (xl == xr && tmp_color != color_bound && tmp_color != color_fill)
                tmp_pix.setX(xl); // запоминаем самую правую координату
            else
                tmp_pix.setX(xl - 1); // запоминаем ее минус 1

            tmp_pix.setY(y); // запоминаем ординату
            st.push(tmp_pix); // пушим в стек наш наденный пиксель
            flag = false;
        }

        tmp_x = xl; // запоминаем опять же самую правую координату
        tmp_color = my_image.pixelColor(xl, y); // запоминаем ее цвет

        // в этом цикле мы переходим к следующей области данной строки, в которой есть затравочные пиксели
        while (xl < X_max && xl < xr && (tmp_color == color_bound || tmp_color == color_fill))
        {
            xl += 1;
            tmp_color = my_image.pixelColor(xl, y);
        }

        if (xl == tmp_x) // если текущая абсцисса не изменилась
            xl += 1; // то прибавляем единицу
    }
}
