from tkinter import *
from tkinter import messagebox
from math import *
import numpy as np

points_prev, frame_prev, points, frame = [], [], [], []
a = 1  # коэффициенты: условие b / a - целое число
b = 3

def main():
    points, def_points, frame, def_frame = [], [], [], []

    root = Tk()
    root.title("Лабораторная номер 2: Эпициклоида, вариант 13")
    root.geometry('1000x600+100+100')

    canv = Canvas(root, width=700, height=600, bg='white')
    canv.pack(side='right')

    menu = Frame(root)

    trans_f = Frame(menu)
    trans_f.grid(row=0, column=0, columnspan=2)
    scale_f = Frame(menu)
    scale_f.grid(row=4, column=0, columnspan=2)
    turn_f = Frame(menu)
    turn_f.grid(row=8, column=0, columnspan=2)

    # Перенос
    Label(trans_f, text='Перенос').grid(row=0, column=0, columnspan=4)
    Label(trans_f, text='dX').grid(row=1, column=0)
    Label(trans_f, text='dY').grid(row=1, column=2)

    # Масштабирование
    Label(scale_f, text='Масштабирование').grid(row=0, column=0, columnspan=4)
    Label(scale_f, text='Xc').grid(row=1, column=0)
    Label(scale_f, text='Yc').grid(row=1, column=2)
    Label(scale_f, text='KX').grid(row=2, column=0)
    Label(scale_f, text='KY').grid(row=2, column=2)

    # Поворот
    Label(turn_f, text='Поворот').grid(row=0, column=0, columnspan=4)
    Label(turn_f, text='X').grid(row=1, column=0)
    Label(turn_f, text='Y').grid(row=1, column=2)
    Label(turn_f, text='Угол (в градусах)').grid(row=2, column=0, columnspan=2)

    # Перенос
    dx = Entry(trans_f, width=5)
    dy = Entry(trans_f, width=5)
    dx.grid(row=1, column=1)
    dy.grid(row=1, column=3)

    # Масштабирование
    c_dx = Entry(scale_f, width=5)
    c_dy = Entry(scale_f, width=5)
    kx = Entry(scale_f, width=5)
    ky = Entry(scale_f, width=5)
    c_dx.grid(row=1, column=1)
    c_dy.grid(row=1, column=3)
    kx.grid(row=2, column=1)
    ky.grid(row=2, column=3)

    # Поворот
    t_dx = Entry(turn_f, width=5)
    t_dy = Entry(turn_f, width=5)
    angle = Entry(turn_f, width=10)
    t_dx.grid(row=1, column=1)
    t_dy.grid(row=1, column=3)
    angle.grid(row=2, column=2, columnspan=2)

    def_points, def_frame = create_default()
    draw(canv)

    transfer_button = Button(menu, text='Перенос',
                            command=lambda:
                            transfering(dx, dy, canv), width=8)
    transfer_button.grid(row=1, column=0, columnspan=2)
    scale_button = Button(menu, text='Масштаб',
                         command=lambda:
                         scaling(c_dx, c_dy, kx, ky, canv), width=8)
    scale_button.grid(row=5, column=0, columnspan=2)
    turn_button = Button(menu, text='Поворот',
                        command=lambda:
                        turning(t_dx, t_dy, angle, canv), width=8)
    turn_button.grid(row=9, column=0, columnspan=2)
    prev_button = Button(menu, text='Назад',
                        command=lambda:
                        one_step_back(canv), width=12, height=1)
    prev_button.grid(row=12, column=0, columnspan=4)
    def_button = Button(menu, text='В начало',
                        command=lambda:
                         def_state(def_points, def_frame, canv), width=12, height=1)
    def_button.grid(row=13, column=0, columnspan=4)
    Label(menu, text='Центр в точке (350, 300)').grid(row=20, column=0, columnspan=4)
    Label(menu, text='Группа ИУ7-45Б\n'
                     'Мищенко Маргарита').grid(row=21, column=0, columnspan=4)

    menu.pack(side='top')
    canv.pack()

    root.mainloop()

def create_default():
    global a, b, points, frame
    dx, dy = 350, 300 # смещение в центр
    kx = ky = 10 # масштабирование
    for t in np.arange(0, 2 * pi, 0.001):
        x = (a + b) * cos(t) * kx - a * cos((a + b) / a * t) * kx + dx
        y = (a + b) * sin(t) * ky - a * sin((a + b) / a * t) * ky + dy
        points.append([x, y])
    start_points = points.copy()

    frame = [[250, 200], [450, 200], [450, 400], [250, 400]]
    frame_start = frame.copy()
    #print(len(points))
    return start_points, frame_start

def draw(canv):
    global points, frame
    canv.delete('all')

    canv.create_polygon(frame, fill='green', outline='green') # штриховка
    canv.create_polygon(points, fill='lightblue', outline='lightblue') # эпициклоида

    canv.create_line(0, 3, 800, 3)
    i = 100
    while i < 700:
        canv.create_text(i, 10, text=str(i))
        i += 100
    canv.create_text(690, 10, text='X')
    canv.create_line(3, 0, 3, 600)
    i = 100
    while i < 600:
        canv.create_text(16, i, text=str(i))
        i += 100
    canv.create_text(12, 585, text='Y')
    canv.create_text(12, 10, text='0')
    canv.create_line(0, 596, 800, 596)
    canv.create_line(801, 0, 801, 600)

def transfering(get_x, get_y, canv):
    global points_prev, frame_prev, points, frame
    flag = 0
    dx, dy = 0, 0
    try:
        dx = float(get_x.get())
        dy = float(get_y.get())
    except ValueError:
        messagebox.showerror('Ошибка', 'Некорректный ввод коэффициентов перемещения.')
        flag = 1

    if flag == 0:
        points_prev.clear()
        points_prev = points.copy()
        points.clear()

        for i, element in enumerate(points_prev):
            x = element[0] + dx
            y = element[1] + dy
            points.append([x, y])

        frame_prev.clear()
        frame_prev = frame.copy()
        frame.clear()

        for i, element in enumerate(frame_prev):
            x = element[0] + dx
            y = element[1] + dy
            frame.append([x, y])
        #print(frame)

        draw(canv)

def scaling(xc_get, yc_get, kx_get, ky_get, canv):
    global points_prev, frame_prev, points, frame
    flag, kx, ky, xc, yc = 0, 0, 0, 0, 0
    try:
        xc = float(xc_get.get())
        yc = float(yc_get.get())
    except ValueError:
        messagebox.showerror('Ошибка', 'Некорректный ввод центра масштабирования.')
        flag = 1
    try:
        kx = float(kx_get.get())
        ky = float(ky_get.get())
    except ValueError:
        messagebox.showerror('Ошибка', 'Некорректный ввод коэффициентов масштабирования.')
        flag = 1

    if flag == 0:
        #print(kx, ky)
        points_prev.clear()
        points_prev = points.copy()
        points.clear()

        frame_prev.clear()
        frame_prev = frame.copy()
        frame.clear()

        for i, element in enumerate(points_prev):
            x = kx * element[0] + (1 - kx) * xc
            y = ky * element[1] + (1 - ky) * yc
            y = ky * element[1] + (1 - ky) * yc
            points.append([x, y])


        for i, element in enumerate(frame_prev):
            x = kx * element[0] + (1 - kx) * xc
            y = ky * element[1] + (1 - ky) * yc
            frame.append([x, y])

        draw(canv)

def turning(get_x, get_y, angle_get, canv):
    global points_prev, frame_prev, points, Square
    flag, alpha, xc, yc = 0, 0, 0, 0
    try:
        xc = float(get_x.get())
        yc = float(get_y.get())
    except ValueError:
        messagebox.showerror('Ошибка', 'Некорректный ввод центра поворота.')
        flag = 1

    try:
        alpha = int(angle_get.get())
    except ValueError:
        messagebox.showerror('Ошибка', 'Некорректный ввод угла поворота.')
        flag = 1

    if flag == 0:
        alpha = degrees(alpha)
        points_prev.clear()
        points_prev = points.copy()
        points.clear()

        for i, element in enumerate(points_prev):
            x = xc + (element[0] - xc) * cos(alpha) + (element[1] - yc) * sin(alpha)
            y = yc - (element[0] - xc) * sin(alpha) + (element[1] - yc) * cos(alpha)
            points.append([x, y])

        frame_prev.clear()
        frame_prev = frame.copy()
        frame.clear()

        for i, element in enumerate(frame_prev):
            x = xc + (element[0] - xc) * cos(alpha) + (element[1] - yc) * sin(alpha)
            y = yc - (element[0] - xc) * sin(alpha) + (element[1] - yc) * cos(alpha)
            frame.append([x, y])

        draw(canv)

def one_step_back(canv):
    global points_prev, frame_prev, points, frame

    points = points_prev.copy()
    frame = frame_prev.copy()
    draw(canv)

def def_state(def_points, def_frame, canv):
    global points_prev, frame_prev, points, frame

    points_prev = points.copy()
    frame_prev = frame.copy()
    points = def_points.copy()
    frame = def_frame.copy()
    draw(canv)

main()