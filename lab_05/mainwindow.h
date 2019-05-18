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
    void change_widget_color(QWidget *wid, QColor color);
    void add_point(QPoint &p);
    void close_poly();
    ~MainWindow();

private slots:
    void on_button_color_bound_clicked();

    void on_button_color_fill_clicked();

    void on_button_color_bg_clicked();

    void on_button_color_sep_clicked();

    void on_button_fill_clicked();

    void on_button_fill_slow_clicked();

    void on_button_clear_clicked();

    void on_button_inp_point_clicked();

    void on_button_close_p_clicked();

    void on_button_clear_fill_clicked();

private:
    Ui::MainWindow *ui;
    canvas *myscene;
    QPoint bound_start_point;
    QPoint previous_point;
    bool new_bound;
    bool shift_pressed;

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};

#endif // MAINWINDOW_H
