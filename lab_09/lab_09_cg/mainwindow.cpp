#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>

#include "normal_funcs.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->add_cutter_radioButton->setChecked(true);
    myscene = new canvas(ui->draw_widget);
    myscene->setMinimumSize(X_max, Y_max);

    new_point = true;
    shift_pressed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_poly_point(QPoint &p)
{
    if (new_point)
    {
        p_start = p;
        new_point = false;
    }
    myscene->add_point_poly(p);
    previous_point = p;
    myscene->repaint();
}

void MainWindow::add_cut_point(QPoint &p)
{
    if (new_point)
    {
        p_start = p;
        new_point = false;
    }
    myscene->add_point_cut(p);
    previous_point = p;
    myscene->repaint();
}

void MainWindow::find_closest_edge(QPoint &p)
{
    int index = myscene->find_closest(p);
    closest_edge = myscene->get_edge(index);
}

void MainWindow::close_poly()
{
    if (new_point)
    {
        return;
    }
    if (ui->add_cutter_radioButton->isChecked())
    {
        myscene->add_point_cut(p_start);
    }
    if (ui->add_poly_radioButton->isChecked())
    {
        myscene->add_point_poly(p_start);
    }
    new_point = true;
    myscene->repaint();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint p = myscene->mapFromGlobal(QCursor::pos());
    if (p.x() < X_max && p.x() > X_min && p.y() > Y_min && p.y() < Y_max)
    {
        int dx = abs(p.x() - previous_point.x());
        int dy = abs(p.y() - previous_point.y());
        if (event->button() == Qt::LeftButton)
        {
            if (ui->add_poly_radioButton->isChecked())
            {
                if (shift_pressed)
                {
                    find_closest_edge(p);
                    QPoint tmp = normal_point(closest_edge.p1, closest_edge.p2, p);
                    p.setX(tmp.x());
                    p.setY(tmp.y());
                }
                add_poly_point(p);
            }
            if (ui->add_cutter_radioButton->isChecked())
            {
                if (shift_pressed)
                {
                    if (dy > dx)
                        p.setX(previous_point.x());
                    else
                        p.setY(previous_point.y());
                }
                add_cut_point(p);
            }
        }
        else
            close_poly();
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

void MainWindow::on_clear_poly_pushButton_clicked()
{
    new_point = true;
    myscene->clear_poly();
    myscene->repaint();
}

void MainWindow::on_clear_all_pushButton_clicked()
{
    new_point = true;
    myscene->clear_all();
    myscene->repaint();
}

void MainWindow::on_clear_cutter_pushButton_clicked()
{
    new_point = true;
    myscene->clear_cutter();
    myscene->repaint();
}

void MainWindow::on_cut_pushButton_clicked()
{
    if (!myscene->is_convex())
        QMessageBox::warning(this, "Ошибка отсекателя", "Введенный отсекатель не выпуклый. "
                                                        "Отсечение не может быть произведено.");
    else
    {
        myscene->cut_result();
        myscene->repaint();
    }
}

void MainWindow::on_close_pushButton_clicked()
{
    close_poly();
}


void MainWindow::on_add_point_pushButton_clicked()
{
    bool ok_1 = true, ok_2 = true;
    int x = ui->x_edit->text().toInt(&ok_1);
    int y = ui->y_edit->text().toInt(&ok_2);
    if (!ok_1 || !ok_2)
        QMessageBox::warning(this, "Ошибка ввода", "Вводите, пожалуйста, только целые числа.");
    else
    {
        QPoint cur(x, y);
        if (ui->add_poly_radioButton->isChecked())
            add_poly_point(cur);
        if (ui->add_cutter_radioButton->isChecked())
            add_cut_point(cur);
        myscene->repaint();
    }
}

void MainWindow::on_add_poly_radioButton_clicked()
{
    close_poly();
    if (myscene->cutter_is_empty())
        return;
    if (!myscene->is_convex())
        QMessageBox::warning(this, "Ошибка отсекателя", "Введенный отсекатель не выпуклый. "
                                                        "Отсечение не может быть произведено.");
}

void MainWindow::on_add_cutter_radioButton_clicked()
{
    new_point = true;
}
