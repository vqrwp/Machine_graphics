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
    change_widget_color(ui->w_color_bound, QColor(Qt::black));
    change_widget_color(ui->w_color_fill, QColor(Qt::magenta));
    myscene = new canvas(ui->draw_widget);
    myscene->setMinimumSize(640, 600);
    new_bound = true;
    shift_pressed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
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
    QColor color = QColorDialog::getColor(Qt::black, this, "Выберете цвет");
    if (color.isValid())
    {
        change_widget_color(ui->w_color_bound, color);
        myscene->set_color_bound(color);
        //myscene->repaint();
    }
}

void MainWindow::on_button_color_fill_clicked()
{
    QColor color = QColorDialog::getColor(Qt::magenta, this, "Выберете цвет");
    if (color.isValid())
    {
        change_widget_color(ui->w_color_fill, color);
        myscene->set_color_fill(color);
        //myscene->repaint();
    }
}

void MainWindow::on_button_color_bg_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберете цвет");
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
    if (p.x() < 640 && p.x() > 0 && p.y() > 0 && p.x() < 600)
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
            add_point(p);
        }
        else
        {
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
    myscene->pre_fill_polygon(false);
}

void MainWindow::on_button_fill_slow_clicked()
{
    close_poly();
    myscene->pre_fill_polygon(true);
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
        add_point(tmp);
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
    myscene->repaint();
    myscene->set_color_fill(tmp);
}
