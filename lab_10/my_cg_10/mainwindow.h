#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QPalette>
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

private slots:

    void on_draw_surface_pushButton_clicked();

    void on_clear_pushButton_clicked();

    void on_pushButton_clicked();

    void on_right_pushButton_pressed();

    void on_right_pushButton_released();

    void on_left_pushButton_pressed();

    void on_left_pushButton_released();

    void on_up_pushButton_pressed();

    void on_up_pushButton_released();

    void on_down_pushButton_pressed();

    void on_down_pushButton_released();

    void on_oz_pushButton_pressed();

    void on_oz_pushButton_released();

private:
    Ui::MainWindow *ui;
    canvas *myscene;
    Function functionT;
    transform_params params;
    surface_data cur_data;
    void rotate(void);
    bool key_down_pressed;
    bool key_up_pressed;
    bool key_right_pressed;
    bool key_left_pressed;
    bool key_z_pressed;
};

#endif // MAINWINDOW_H
