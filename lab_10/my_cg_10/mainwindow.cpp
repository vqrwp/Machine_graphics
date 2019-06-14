#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDoubleValidator>
#include <QKeyEvent>

/*
void change_widget_color(QWidget *widg, QColor color)
{
    QPalette pal = widg->palette();
    pal.setColor(QPalette::Window, color);
    widg->setPalette(pal);
}
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDoubleValidator *v = new QDoubleValidator(this);
    v->setLocale(QLocale::English);
    ui->x_start_lineEdit->setValidator(v);
    ui->x_end_lineEdit->setValidator(v);
    ui->delta_x_lineEdit->setValidator(v);
    ui->z_start_lineEdit->setValidator(v);
    ui->z_end_lineEdit->setValidator(v);
    ui->delta_z_lineEdit->setValidator(v);
    ui->rotate_ox_lineEdit->setValidator(v);
    ui->rotate_oy_lineEdit->setValidator(v);
    ui->rotate_oz_lineEdit->setValidator(v);
    ui->k_lineEdit->setValidator(v);

    ui->funcs->addItem(functionT.get_function_name(1));
    ui->funcs->addItem(functionT.get_function_name(2));
    ui->funcs->addItem(functionT.get_function_name(3));
    ui->funcs->addItem(functionT.get_function_name(4));
    ui->funcs->currentIndexChanged(1);

    myscene = new canvas(ui->draw_widget);
    myscene->setMinimumSize(X_MAX, Y_MAX);

    params.angle_x = 1;
    params.angle_y = 1;
    params.angle_z = 1;
    params.k = 40;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_draw_surface_pushButton_clicked()
{
    cur_data.x_min = ui->x_start_lineEdit->text().toDouble();
    cur_data.x_max = ui->x_end_lineEdit->text().toDouble();
    cur_data.d_x = ui->delta_x_lineEdit->text().toDouble();
    cur_data.z_min = ui->z_start_lineEdit->text().toDouble();
    cur_data.z_max = ui->x_end_lineEdit->text().toDouble();
    cur_data.d_z = ui->delta_z_lineEdit->text().toDouble();
    cur_data.func = functionT.get_function(ui->funcs->currentIndex());
    myscene->draw_main_algo(cur_data, params);
    myscene->repaint();
}


void MainWindow::on_clear_pushButton_clicked()
{
    myscene->clear();
    myscene->repaint();
    params.angle_x = 1;
    params.angle_y = 1;
    params.angle_z = 1;
    params.k = 40;
}

void correct_data(transform_params &p)
{
    if (p.angle_x > 360)
        p.angle_x -= 360;
    else if (p.angle_x < -360)
        p.angle_x += 360;
    if (p.angle_y > 360)
        p.angle_y -= 360;
    else if (p.angle_y < -360)
        p.angle_y += 360;
    if (p.angle_z > 360)
        p.angle_z -= 360;
    else if (p.angle_z < -360)
        p.angle_z += 360;
}

/*
void MainWindow::on_rotate_pushButton_clicked()
{
    params.angle_x += ui->rotate_ox_lineEdit->text().toDouble();
    params.angle_y += ui->rotate_oy_lineEdit->text().toDouble();
    params.angle_z += ui->rotate_oz_lineEdit->text().toDouble();
    correct_data(params);
    myscene->draw_main_algo(cur_data, params);
    myscene->repaint();
}
*/

void MainWindow::on_pushButton_clicked()
{
    double cur_k = ui->k_lineEdit->text().toDouble();
    if (params.k + cur_k != 0)
    {
        params.k += ui->k_lineEdit->text().toDouble();
        myscene->draw_main_algo(cur_data, params);
        myscene->repaint();
    }
    else
        QMessageBox::warning(this, "Вырождение в точку", "Дальнейшее масштабирование "
                                                         "сделает график слишком маленьким.");

}

void MainWindow::rotate()
{
    if (key_down_pressed)
        params.angle_x -= ui->rotate_ox_lineEdit->text().toDouble();
    if (key_up_pressed)
        params.angle_x += ui->rotate_ox_lineEdit->text().toDouble();
    if (key_left_pressed)
        params.angle_y -= ui->rotate_oy_lineEdit->text().toDouble();
    if (key_right_pressed)
        params.angle_y += ui->rotate_oy_lineEdit->text().toDouble();
    if (key_z_pressed)
        params.angle_z += ui->rotate_oz_lineEdit->text().toDouble();
    //correct_data(params);
    myscene->draw_main_algo(cur_data, params);
    myscene->repaint();
}

void MainWindow::on_right_pushButton_pressed()
{
    key_right_pressed = true;
    rotate();
}

void MainWindow::on_right_pushButton_released()
{
    key_right_pressed = false;
}

void MainWindow::on_left_pushButton_pressed()
{
    key_left_pressed = true;
    rotate();
}

void MainWindow::on_left_pushButton_released()
{
    key_left_pressed = false;
}

void MainWindow::on_up_pushButton_pressed()
{
    key_up_pressed = true;
    rotate();
}

void MainWindow::on_up_pushButton_released()
{
    key_up_pressed = false;
}

void MainWindow::on_down_pushButton_pressed()
{
    key_down_pressed = true;
    rotate();
}

void MainWindow::on_down_pushButton_released()
{
    key_down_pressed = false;
}

void MainWindow::on_oz_pushButton_pressed()
{
    key_z_pressed = true;
    rotate();
}

void MainWindow::on_oz_pushButton_released()
{
    key_z_pressed = false;
}
