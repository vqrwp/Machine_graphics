#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->add_cutter_radioButton->setChecked(true);
    myscene = new my_paintwidget(ui->draw_widget);
    myscene->setMinimumSize(X_max, Y_max);

    new_point = true;
    shift_pressed = false;
    parallel_segment = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_seg_point(QPoint &p)
{
    if (new_point)
    {
        p_start = p;
        myscene->add_point_seg(p_start);
        new_point = false;
    }
    else
    {
        segment_type seg(p_start, p);
        myscene->add_segment(seg);
        new_point = true;
    }
    previous_point = p;
    myscene->repaint();
}

void MainWindow::add_cut_point(QPoint &p)
{
    if (new_point)
    {
        p_start = p;
        myscene->add_point_cut(p_start);
        new_point = false;
    }
    else
    {
        segment_type seg(previous_point, p);
        myscene->add_cut_seg(seg);
    }
    previous_point = p;
    myscene->repaint();
}

void MainWindow::find_closest_edge(QPoint &p)
{
    int index = myscene->find_closest(p);
    closest_edge = myscene->get_edge(index);
}

void MainWindow::calculate_parallel_point(QPoint &p)
{
    int dx = closest_edge.p2.x() - closest_edge.p1.x();
    if (dx == 0)
        p.setX(p_start.x());
    else
    {
        int dy = closest_edge.p2.y() - closest_edge.p1.y();
        p.setY(round((p.x() - p_start.x()) * dy / dx) + p_start.y());
    }
}

void MainWindow::close_poly()
{
    if (new_point)
        return;
    segment_type tmp = {previous_point, p_start};
    myscene->add_cut_seg(tmp);
    new_point = true;
    myscene->repaint();
}

void MainWindow::on_add_segment_pushButton_clicked()
{
    bool ok_1 = true, ok_2 = true, ok_3 = true, ok_4 = true;
    int x1 = ui->x_start_edit->text().toInt(&ok_1);
    int y1 = ui->y_start_edit->text().toInt(&ok_2);
    int x2 = ui->x_end_edit->text().toInt(&ok_3);
    int y2 = ui->y_end_edit->text().toInt(&ok_4);
    if (!ok_1 || !ok_2 || !ok_3 || !ok_4)
        QMessageBox::warning(this, "Ошибка ввода", "Вводите, пожалуйста, только целые числа.");
    else
    {
        segment_type elem {QPoint(x1, y1), QPoint(x2, y2)};
        if (ui->add_segment_radioButton->isChecked())
            myscene->add_segment(elem);
        if (ui->add_cutter_radioButton->isChecked())
            myscene->add_cut_seg(elem);
        myscene->repaint();
    }
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
            if (shift_pressed)
            {
                if (dy > dx)
                    p.setX(previous_point.x());
                else
                    p.setY(previous_point.y());
            }
            if (ui->add_segment_radioButton->isChecked())
            {
                if (parallel_segment)
                {
                    if (!new_point)
                        calculate_parallel_point(p);
                }
                add_seg_point(p);
            }
            if (ui->add_cutter_radioButton->isChecked())
                add_cut_point(p);
        }
        else
        {
            if (ui->add_cutter_radioButton->isChecked())
                close_poly();
            else
            {
                if (parallel_segment)
                    parallel_segment = false;
                else
                {
                    find_closest_edge(p);
                    parallel_segment = true;
                }
            }
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

void MainWindow::on_add_segment_radioButton_clicked()
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

void MainWindow::on_clear_segments_pushButton_clicked()
{
    new_point = true;
    myscene->clear_segments();
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

void MainWindow::on_pushButton_clicked()
{
    close_poly();
}

void MainWindow::on_default_pushButton_clicked()
{
    myscene->add_cut_seg(segment_type(QPoint(200, 295), QPoint(345, 150)));
    myscene->add_cut_seg(segment_type(QPoint(345, 150), QPoint(490, 295)));
    myscene->add_cut_seg(segment_type(QPoint(490, 295), QPoint(345, 440)));
    myscene->add_cut_seg(segment_type(QPoint(345, 440), QPoint(200, 295)));
    myscene->repaint();
}
