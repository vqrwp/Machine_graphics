#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QColorDialog>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <QGraphicsScene>

#define X_WIDTH 911
#define Y_WIDTH 421
#define OFFSET_X_MOUSE 20
#define OFFSET_Y_MOUSE 290
#define WIDTH 911
#define HEIGHT 421
#define WINDOW_BEGIN 0
#define NUM_BITS 4
#define ACCURACY sqrt(2) // точность - диагональ пикселя

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->draw_label->setPalette(QColor(Qt::white));
    this->setWindowTitle("Отсечение: метод половинного деления");

    scene = new QPixmap(X_WIDTH, Y_WIDTH);
    scene->fill(QColor(Qt::white));

    painter = new QPainter(scene);
    painter->setPen(Qt::red);

    ui->draw_label->setPixmap(*scene);
    painter->setPen(Qt::black);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);

    ui->line_color_label->setAutoFillBackground(true);
    ui->line_color_label->setPalette(Pal);

    Pal.setColor(QPalette::Background, Qt::magenta);
    ui->cutter_color_label->setAutoFillBackground(true);
    ui->cutter_color_label->setPalette(Pal);

    Pal.setColor(QPalette::Background, Qt::cyan);
    ui->outline_color_label->setAutoFillBackground(true);
    ui->outline_color_label->setPalette(Pal);

    line_color = Qt::black;
    cutter_color = Qt::magenta;
    outline_color = Qt::cyan;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_add_point_button_clicked()
{
    bool ok1 = true, ok2 = false;
    QString x_value = ui->entry_x->text();
    QString y_value = ui->entry_y->text();
    int x = x_value.toInt(&ok1);
    int y = y_value.toInt(&ok2);
    if (ok1 && ok2)
    {
        if (ui->line_radio->isChecked())
        {
            flag_line_exist = true;
            painter->setPen(QColor(line_color));
            draw_line(*painter, x, y, first_x, first_y, flag_first_touched, lines);
        }
        else if (ui->cutter_radio->isChecked())
        {
            if (flag_rect_set == false)
            {
                painter->setPen(QColor(cutter_color));
                draw_rect(*painter, x, y, first_rect_x, first_rect_y, flag_first_rect_touched, flag_rect_set, rect);
            }
        }
        ui->draw_label->setPixmap(*scene);
    }
    else
    {
        if (ok1 && !ok2)
        {
            message_box(QString("Координата по Y введена не корректно"));
        }
        else if (!ok1 && ok2)
        {
            message_box(QString("Координата по X введена не корректно"));
        }
        else
        {
            message_box(QString("Введены неверные Координаты точки"));
        }

    }
}

// рисование на холсте
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    x -= OFFSET_X_MOUSE;
    y -= OFFSET_Y_MOUSE;
    if (x >= WINDOW_BEGIN && y >= WINDOW_BEGIN && x <= WIDTH && y <= HEIGHT)
    {
        if (ui->line_radio->isChecked())
        {
            painter->setPen(QPen(line_color));
            if (event->button() == Qt::LeftButton)
            {
                if (event->modifiers() == Qt::ShiftModifier)
                {
                    if (std::abs(first_x - x) < std::abs(first_y - y))
                    {
                        x = first_x;
                    }
                    else
                    {
                        y = first_y;
                    }
                }
                flag_line_exist = true;
                painter->setRenderHint(QPainter::Antialiasing, true);
                draw_line(*painter, x, y, first_x, first_y, flag_first_touched, lines);
            }
        }
        else if (ui->cutter_radio->isChecked())
        {
            if (flag_rect_set == false)
            {
                painter->setRenderHint(QPainter::Antialiasing, true);
                painter->setPen(cutter_color);
                draw_rect(*painter, x, y, first_rect_x, first_rect_y, flag_first_rect_touched, flag_rect_set, rect);
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("Ошибка!");
                // Тип иконки сообщения
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setInformativeText("Отсекатель уже введен!");
                msgBox.exec();
            }
        }
        ui->draw_label->setPixmap(*scene);
    }

}

// само отсечение
void MainWindow::on_cut_button_clicked()
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    // если на холсте есть и отсекатель и отрезки
    if (flag_line_exist && flag_rect_set)
    {
        // пока не пройдем все отрезки
        for (int j = 0; j < lines.size(); j += 2)
        {
            // lines[i] = P1 and lines[i + 1] = P2 // P1, P2 - points

            int i = 1; // шаг отсечения

            QPoint P1 = lines[j];
            QPoint P2 = lines[j + 1];

            qDebug() << P1 << P2;

            int T1[NUM_BITS]; // = 4 битовый код первой точки
            int T2[NUM_BITS]; // = 4 битовый код второй точки

            int S1, S2; // Summ of BITS T1 and BITS T2
            while (1)
            {
                // вычисляем значения битовых кодов точек
                set_bits(rect, P1, T1);
                set_bits(rect, P2, T2);

                S1 = get_sum(T1, NUM_BITS);
                S2 = get_sum(T2, NUM_BITS);

                if (S1 == 0 && S2 == 0)
                {
                    qDebug() << "Отрезок тривиально видим.";
                    painter->setPen(QPen(outline_color, 2)); // цвет отсечения

                    // закрашиваем отрезок целиком
                    painter->drawLine(P1.x(), P1.y(), P2.x(), P2.y());
                    ui->draw_label->setPixmap(*scene);

                    // заканчиваем просмотр данного отрезка
                    break;
                }

                int P = logic_mult(T1, T2, NUM_BITS); //

                QPoint R;
                QPoint tmp;

                // если логическое побитовое произведение концевых точек равно нулю
                if (P == 0) // то имеют целесообразность дальнейшие проверки
                {
                    // i > 2 когда отрезок выродился в точку при проверке
                    if (i > 2)
                    {
                        // если зашло сюда, то точка точно видна, тк мы уже проверяли
                        qDebug() << P1 << P2;

                        painter->setPen(QPen(outline_color, 2));

                        painter->drawLine(P1.x(), P1.y(), P2.x(), P2.y());
                        ui->draw_label->setPixmap(*scene);

                        // заканчиваем просмотр отрезка
                        break;
                    }
                    R = P1; // запоминаем первую концевую точку
                    // если побитовый код второй точки 0, она лежит внутри
                    if (S2 == 0)
                    {
                        P1 = P2; // меняем точки местами, чтобы два раза не прописывать алгоритм
                        P2 = R; // R = old P1
                        i++;
                    }
                    else // if (S1 == 0) побитовый код первой точки 0, она лежит внутри
                    {
                        // пока отрезок не вырождается в точку (проверяем через диаг. пикселя)
                        while (abs(P1.x() - P2.x()) > ACCURACY || abs(P1.y() - P2.y()) > ACCURACY)
                        {
                            QPoint Pm;
                            // вычисляем координаты средней точки
                            Pm.setX((P1.x() + P2.x()) >> 1); // здесь побитовый сдвиг вправо
                            Pm.setY((P1.y() + P2.y()) >> 1); // это как деление на 2

                            tmp = P1; // запоминаем первую точку
                            P1 = Pm; // теперь Р1 как бы средняя точка, рассм. отрезок Pm P2

                            // битовый код средней точки
                            set_bits(rect, P1, T1);

                            int pr = logic_mult(T1, T2, NUM_BITS);

                            if (pr != 0) // если Рм && P2 != 0 то откидываем эту часть отрезка - она невидима
                            {
                                P1 = tmp; // возвращаем Р1
                                P2 = Pm; // теперь рассматриваем отрезок Р1 Рм
                            }
                        }
                        P1 = P2; // опять меняем точки местами ( чтобы покрыть оба интервала)
                        P2 = R; // R = old P1
                        i++; // увеличиваем шаг отсечения
                    }
                }
                // если логическое побитовое произведение кодов концевых точек не равно 0
                else
                {
                    qDebug() << "Отрезок тривиально невидим.";
                    break;
                }
            }
        }
    }
    else
    {
        if (!flag_rect_set && flag_line_exist)
        {
            message_box(QString("Вы не ввели отсекатель!"));
        }
        else if (!flag_line_exist && flag_rect_set)
        {
            message_box(QString("Вы не нарисовали линии!"));
        }
        else
        {
            message_box(QString("Вы не нарисовали линии и не ввели отсекатель!"));
        }

    }
}

// удаление отсекателя
void MainWindow::on_delete_cutter_button_clicked()
{
    delete painter;
    delete scene;
    ui->draw_label->clear();
    scene = new QPixmap(X_WIDTH, Y_WIDTH);
    scene->fill(QColor(Qt::white));
    painter = new QPainter(scene);
    // here i draw everything that was in scene besides cutter (lines)
    if (flag_rect_set)
    {
        painter->setPen(line_color);
        for (int i = 0; i < lines.size(); i += 2)
        {
            painter->drawLine(lines[i], lines[i + 1]);
        }
        ui->draw_label->setPixmap(*scene);
    }
    flag_rect_set = false;
    rect.clear();
    ui->draw_label->setPixmap(*scene);
}

// очистка экрана
void MainWindow::on_clear_button_clicked()
{
    delete painter;
    delete scene;
    ui->draw_label->clear();
    scene = new QPixmap(X_WIDTH, Y_WIDTH);
    scene->fill(QColor(Qt::white));
    painter = new QPainter(scene);
    ui->draw_label->setPixmap(*scene);
    prev_x = -1;
    prev_y = -1;
    flag_first_touched = false;
    flag_rect_set = false;
    flag_line_exist = false;
    rect.clear();
    lines.clear();
}

//работа с цветами - выбор нового цвета

void MainWindow::on_line_color_button_clicked()
{
    line_color = QColorDialog::getColor(line_color, this);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, line_color);
    ui->line_color_label->setAutoFillBackground(true);
    ui->line_color_label->setPalette(Pal);
    ui->line_color_label->show();
}

void MainWindow::on_outline_color_button_clicked()
{
    outline_color = QColorDialog::getColor(outline_color, this);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, outline_color);
    ui->outline_color_label->setAutoFillBackground(true);
    ui->outline_color_label->setPalette(Pal);
    ui->outline_color_label->show();
}

void MainWindow::on_cutter_color_button_clicked()
{
    cutter_color = QColorDialog::getColor(cutter_color, this);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, cutter_color);
    ui->cutter_color_label->setAutoFillBackground(true);
    ui->cutter_color_label->setPalette(Pal);
    ui->cutter_color_label->show();
}
