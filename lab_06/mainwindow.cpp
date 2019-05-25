#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QColorDialog>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    change_widget_color(ui->w_color_bg, QColor(Qt::white));
    change_widget_color(ui->w_color_bound, QColor(Qt::darkCyan));
    change_widget_color(ui->w_color_fill, QColor(Qt::magenta));

    ui->radioButton_point->setChecked(true);

    myscene = new my_paintwidget(ui->draw_widget);
    myscene->setMinimumSize(X_max, Y_max);
    new_bound = true;
    shift_pressed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
\

void MainWindow::change_widget_color(QWidget *wid, QColor color)
{
    QPalette pal = wid->palette();
    pal.setColor(QPalette::Window, color);
    wid->setPalette(pal);
}

void MainWindow::add_point(QPoint &p)
{
    if (new_bound)
    {
        bound_start_point = p;
        new_bound = false;
        myscene->add_new_point(p);
    }
    else
    {
        edge_type tmp = {previous_point, p};
        myscene->add_new_edge(tmp);
    }
    previous_point = p;
    myscene->repaint();
}

void MainWindow::close_poly()
{
    if (new_bound)
        return;
    edge_type tmp = {previous_point, bound_start_point};
    myscene->add_new_edge(tmp);
    new_bound = true;
    myscene->repaint();
}

void MainWindow::on_button_color_bound_clicked()
{
    QColor color = QColorDialog::getColor(Qt::darkCyan, this, "Поменять цвет");
    if (color.isValid())
    {
        change_widget_color(ui->w_color_bound, color);
        myscene->set_color_bound(color);
        //myscene->repaint();
    }
}

void MainWindow::on_button_color_fill_clicked()
{
    QColor color = QColorDialog::getColor(Qt::magenta, this, "Поменять цвет");
    if (color.isValid())
    {
        change_widget_color(ui->w_color_fill, color);
        myscene->set_color_fill(color);
        //myscene->repaint();
    }
}

void MainWindow::on_button_color_bg_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Поменять цвет");
    if (color.isValid())
    {
        change_widget_color(ui->w_color_bg, color);
        myscene->set_color_bg(color);
        //myscene->repaint();
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint p = myscene->mapFromGlobal(QCursor::pos());
    if (p.x() < X_max && p.x() > X_min && p.y() > Y_min && p.x() < Y_max)
    {
        int dx = abs(p.x() - previous_point.x());
        int dy = abs(p.y() - previous_point.y());
        if (event->button() == Qt::LeftButton)
        {
            if (shift_pressed)
            {
                if (dy > dx)
                    p.setX(previous_point.x());
                else
                    p.setY(previous_point.y());
            }
            if (ui->radioButton_point->isChecked())
                add_point(p);
            if (ui->radioButton_pixel->isChecked())
            {
                myscene->add_first_pixel(p);
                myscene->repaint();
            }
        }
        else
        {
            close_poly();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (ui->radioButton_random->isChecked())
    {
        QPoint p = myscene->mapFromGlobal(QCursor::pos());
        if (p.x() < X_max && p.x() > X_min && p.y() > Y_min && p.x() < Y_max)
        {
            add_point(p);
            if (event->button() == Qt::RightButton)
                close_poly();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift)
        shift_pressed = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift)
        shift_pressed = false;
}

void MainWindow::on_button_fill_clicked()
{
    close_poly();
    if (!myscene->first_pixel)
        QMessageBox::information(this, "Затравочный пиксель", "Введите затравочный пиксель");
    else
        myscene->fill_polygon(false);
}

void MainWindow::on_button_fill_slow_clicked()
{
    close_poly();
    if (!myscene->first_pixel)
        QMessageBox::information(this, "Затравочный пиксель", "Введите затравочный пиксель");
    else
        myscene->fill_polygon(true);
}

void MainWindow::on_button_clear_clicked()
{
    myscene->clear();
    new_bound = true;
    //myscene->repaint();
}


void MainWindow::on_button_inp_point_clicked()
{
    bool ok_1 = true, ok_2 = true;
    int x = ui->x_edit->text().toInt(&ok_1);
    int y = ui->y_edit->text().toInt(&ok_2);
    if (!ok_1 || !ok_2)
        QMessageBox::warning(this, "Ошибка ввода", "Вводите, пожалуйста, только целые числа.");
    else
    {
        QPoint tmp(x, y);
        if (ui->radioButton_point->isChecked())
            add_point(tmp);
        if (ui->radioButton_pixel->isChecked())
        {
            myscene->add_first_pixel(tmp);
            myscene->repaint();
        }
    }
}

void MainWindow::on_button_close_p_clicked()
{
    close_poly();
}

void MainWindow::on_button_clear_fill_clicked()
{
    close_poly();
    QColor tmp = myscene->get_color_fill();
    QColor tmp_bg = myscene->get_color_bg();
    myscene->set_color_fill(tmp_bg);
    myscene->update_all();
    myscene->repaint();
    myscene->set_color_fill(tmp);
}

void MainWindow::on_radioButton_point_clicked()
{
    ui->button_close_p->setEnabled(true);
}

void MainWindow::on_radioButton_pixel_clicked()
{
    ui->button_close_p->setEnabled(false);
}

void MainWindow::on_radioButton_random_clicked()
{
    ui->button_close_p->setEnabled(true);
}
