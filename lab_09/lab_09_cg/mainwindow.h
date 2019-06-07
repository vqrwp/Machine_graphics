#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void add_poly_point(QPoint &p);
    void add_cut_point(QPoint &p);
    void find_closest_edge(QPoint &p);
    void close_poly();

private slots:
    void on_add_cutter_radioButton_clicked();

    void on_clear_all_pushButton_clicked();

    void on_clear_cutter_pushButton_clicked();

    void on_cut_pushButton_clicked();

    void on_add_poly_radioButton_clicked();

    void on_close_pushButton_clicked();

    void on_clear_poly_pushButton_clicked();

    void on_add_point_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    canvas *myscene;
    segment_type closest_edge;
    QPoint p_start;
    QPoint previous_point;
    bool new_point;
    bool shift_pressed;

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
