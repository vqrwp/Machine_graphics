#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "my_paintwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void add_seg_point(QPoint &p);
    void add_cut_point(QPoint &p);
    void find_closest_edge(QPoint &p);
    void calculate_parallel_point(QPoint &p);
    void close_poly();

private slots:
    void on_add_segment_pushButton_clicked();

    void on_add_segment_radioButton_clicked();

    void on_add_cutter_radioButton_clicked();

    void on_clear_segments_pushButton_clicked();

    void on_clear_all_pushButton_clicked();

    void on_clear_cutter_pushButton_clicked();

    void on_cut_pushButton_clicked();

    void on_pushButton_clicked();

    void on_default_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    my_paintwidget *myscene;
    segment_type closest_edge;
    QPoint p_start;
    QPoint previous_point;
    bool new_point;
    bool shift_pressed;
    bool parallel_segment;

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
