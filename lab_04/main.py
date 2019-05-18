from tkinter import *
from tkinter import messagebox
from math import sqrt, radians, cos, sin, acos
import matplotlib.pyplot as plt
import time


def draw_point(x, y):
    canvas.create_line(x, y, x + 1, y + 1, fill=line_color)


def draw_all_points(x, y, xc, yc):
    draw_point(xc + x, yc + y)
    draw_point(xc - x, yc + y)
    draw_point(xc + x, yc - y)
    draw_point(xc - x, yc - y)


def middle_dot_circle(xc, yc, R):
    xc = round(xc)
    yc = round(yc)
    R = round(R)

    p = int(1 - R)
    x = 0
    y = R

    draw_all_points(x, y, xc, yc)
    draw_all_points(y, x, xc, yc)

    while y > x:
        x += 1
        if p < 0:
            p += 2 * x + 1
        else:
            y -= 1
            p += 2 * (x - y) + 1

        draw_all_points(x, y, xc, yc)
        draw_all_points(y, x, xc, yc)


def canon_circle(xc, yc, R):
    xc = round(xc)
    yc = round(yc)
    R = round(R)
    r2 = R * R
    x_max = round(R / sqrt(2))

    x = 0
    while x <= x_max: # до этой точки приращение больше по х
        y = round(sqrt(r2 - x * x)) # собственно каноническое уравнение окр-ти
        draw_all_points(x, y, xc, yc)
        x += 1

    y = round(sqrt(r2 - x_max * x_max))
    while y >= 0: # до этой точки приращение больше по у
        x = round(sqrt(r2 - y * y))
        draw_all_points(x, y, xc, yc) # собственно каноническое уравнение окр-ти
        y -= 1


def param_cicrle(xc, yc, r):
    xc = round(xc)
    yc = round(yc)
    R = round(r)
    l = round(3.14 * r / 2) # длина окружн-ти / 4, кол-во пикселей, кот-ое нам нужно закрасить
    for i in range(0, l + 1, 1):
        x = round(r * cos(i / r)) # i changes between 0 and pi / 2 - thats why we divide
        y = round(r * sin(i / r))
        draw_all_points(x, y, xc, yc)


def bresenham_circle(xc, yc, R):
    xc = round(xc)
    yc = round(yc)
    R = round(R)

    x = 0 # начальный икс
    y = R # начальный игрек
    delta = 2 * (1 - R) # начальная ошибка, расстояние от центра окр-ти до диаг. пик-ля

    while y >= 0:
        draw_all_points(x, y, xc, yc)
        if delta < 0: # inside the circle, choose between horizontal and diagonal
            delta_1 = 2 * delta + 2 * y - 1 # расстояние от горизонт. пикселя до диаг.
            if delta_1 <= 0: # choose horizontal
                x += 1
                delta += 2 * x + 1 # recount where in the area of the circle is the next dot
            else: # choose diagonal
                x += 1
                y -= 1
                delta += 2 * (x - y + 1) # recount where in the area of the circle is the next dot
        elif delta > 0: # outside the circle, choose between vertical and diagonal
            delta_2 = 2 * delta - 2 * x - 1 # расстояние от диаг. пикселя до верт.
            if delta_2 <= 0: # choose diagonal
                x += 1
                y -= 1
                delta += 2 * (x - y + 1) # recount where in the area of the circle is the next dot
            else: # choose vertical
                y -= 1
                delta += -2 * y + 1 # recount where in the area of the circle is the next dot
        else: # on the circle, definitely diagonal
            x += 1
            y -= 1
            delta += 2 * (x - y + 1)


def middle_dot_ellipse(xc, yc, a, b):
    a = round(a)
    b = round(b)
    x0 = (round(xc))
    y0 = (round(yc))

    x = 0
    y = b

    # we make this counts before the cycle to make it faster
    aa = a * a
    bb = b * b

    aa2 = 2 * aa
    bb2 = 2 * bb

    x_max = int(round(aa / sqrt(aa + bb))) # dot to change interval

    df_pr = 0
    delta_f_pr = -aa2 * y # korrektirovka

    f = bb - aa * y + aa * 0.25 # probnaya functisa

    while x < x_max:
        draw_all_points(x0, y0, x, y)
        draw_all_points(x0, y0, -x, -y)
        draw_all_points(x0, y0, -x, y)
        draw_all_points(x0, y0, x, -y)

        if f > 0: # if we choose B
            y -= 1
            delta_f_pr += aa2 # corrections

            f += delta_f_pr

        # if we choose A and in casual case
        x += 1

        df_pr += bb2

        f += df_pr + bb

    delta_f_pr = bb2 * x
    f += 0.75 * (aa - bb) - aa * y - bb * x # probnaya functisa
    df_pr = -aa2 * y

    while y >= 0:
        draw_all_points(x0, y0, x, y)
        draw_all_points(x0, y0, -x, -y)
        draw_all_points(x0, y0, -x, y)
        draw_all_points(x0, y0, x, -y)

        if f < 0: # choose B
            x += 1
            delta_f_pr += bb2

            f += delta_f_pr

        # if we choose A and in casual case
        df_pr += aa2

        f += df_pr + aa
        y -= 1


def canon_ellipse(xc, yc, a, b):
    a = (round(a))
    b = (round(b))
    xc = (round(xc))
    yc = (round(yc))

    x_max = (round(a * a / sqrt(a * a + b * b)))
    y_max = (round(b * b / sqrt(a * a + b * b)))

    # FIRST interval, where increment by x > increment by y
    for x in range(-1, x_max, 1):
        y = (round(sqrt(b * b * (1 - x * x / (a * a)))))

        draw_all_points(xc, yc, x, y)
        draw_all_points(xc, yc, -x, -y)
        draw_all_points(xc, yc, -x, y)
        draw_all_points(xc, yc, x, -y)

    # second interval, where increment by y > increment by x
    for y in range(y_max, -1, -1):
        x = (round(sqrt(a * a * (1 - y * y / (b * b)))))
        draw_all_points(xc, yc, x, y)
        draw_all_points(xc, yc, -x, -y)
        draw_all_points(xc, yc, -x, y)
        draw_all_points(xc, yc, x, -y)


def param_ellipse(xc, yc, a, b):
    a = round(a)
    b = round(b)
    xc = round(xc)
    yc = round(yc)

    y = b
    angle = 0
    h = 1.0 / a

    y_min = int(round(b * b / sqrt(a * a + b * b)))

    while y >= y_min: # here increment in x is larger
        x = round(a * sin(angle))
        y = round(b * cos(angle))

        draw_all_points(xc, yc, x, y)
        draw_all_points(xc, yc, -x, -y)
        draw_all_points(xc, yc, -x, y)
        draw_all_points(xc, yc, x, -y)

        angle += h

    h = 1.0 / b

    while y >= 0: # here increment in x is larger
        x = round(a * sin(angle))
        y = round(b * cos(angle))

        draw_all_points(xc, yc, x, y)
        draw_all_points(xc, yc, -x, -y)
        draw_all_points(xc, yc, -x, y)
        draw_all_points(xc, yc, x, -y)

        angle += h


def bresenham_ellipse(xc, yc, a, b):
    x = 0
    y = b

    aa = a * a
    bb = b * b

    # подставляем ур-ие эллипса в ур-ие окр-ти, потом в него начальные коорд-тыы
    delta = aa + bb - 2 * aa * y # a * a + b * b - 2 * a * a * y (b)

    while y >= 0:
        draw_all_points(xc, yc, x, y)
        draw_all_points(xc, yc, -x, -y)
        draw_all_points(xc, yc, -x, y)
        draw_all_points(xc, yc, x, -y)

        if delta < 0: # выбираем между диаг и горизонт
            delta_m = 2 * delta + 2 * aa * y - aa
            x += 1

            if delta_m < 0: # horizontal
                delta += 2 * bb * x + bb

            else:
                y -= 1 # diagonal
                delta += 2 * x * bb - 2 * y * aa + aa + bb

        elif delta > 0: # выбираем между диаг и верт
            delta_m = 2 * delta - 2 * bb * x - bb
            y -=1

            if delta_m > 0: # vertical
                delta += -2 * y * aa + aa
            else:
                x += 1 # diagonal
                delta += 2 * x * bb - 2 * y * aa + aa + bb

            continue
        else:# // if (d == 0)
            x += 1
            y -= 1
            delta += 2 * x * bb - 2 * y * aa + aa + bb


def one_circle(choise):
    xc, yc, r = xc_b.get(), yc_b.get(), r_b.get()
    if not xc or not yc or not r:
        messagebox.showwarning('Ошибка ввода',
                               'Какие-то параметры не заданы!')
    else:
        try:
            xc, yc, r = round(float(xc)), round(float(yc)), round(float(r))
            if r != 0:
                if choise[0] != 4:
                    funcs_c[choise[0]](xc, yc, r)
                else:
                    canvas.create_oval(xc - r, yc - r, xc + r, yc + r, outline=line_color)
            else:
                messagebox.showwarning('Ошибка ввода',
                                       'Радиус равен 0!')
        except:
            messagebox.showwarning('Ошибка ввода',
                                   'Нечисловое значение для параметров отрезка!')


def sev_circles(choise):
    r, delta, num = rad_b.get(), delta_b.get(), num_b.get()
    if not r or not delta or not num:
        messagebox.showwarning('Ошибка ввода',
                               'Какие-то параметры не заданы!')
    else:
        try:
            r, delta, num = round(float(r)), round(float(delta)), round(float(num))
            if r != 0 and num != 0:
                if choise[0] != 4:
                    d = 0
                    for i in range(num):
                        funcs_c[choise[0]](350, 200, r + d)
                        d += delta
                else:
                    d = 0
                    for i in range(num):
                        canvas.create_oval(350 - r - d, 200 - r - d, 350 + r + d, 200 + r + d)
                        d += delta
                    canvas.create_oval(350 - r, 200 - r, 350 + r, 200 + r)
            else:
                messagebox.showwarning('Ошибка ввода',
                                       'Радиус или количество равны 0!')
        except:
            messagebox.showwarning('Ошибка ввода',
                                   'Нечисловое значение для параметров отрезка!')

def one_elli(choise):
    c_x, c_y, a, b = c_x_b.get(), c_y_b.get(), a_b.get(), b_b.get()
    if not c_x or not c_y or not a or not b:
        messagebox.showwarning('Ошибка ввода',
                               'Какие-то параметры не заданы!')
    else:
        try:
            c_x, c_y, a, b = round(float(c_x)), round(float(c_y)), round(float(a)), round(float(b))
            if a != 0 and b != 0:
                if choise[0] != 4:
                    funcs_e[choise[0]](c_x, c_y, a, b)
                else:
                    canvas.create_oval(c_x - a, c_y - b, c_x + a, c_y + b, outline=line_color)
            else:
                messagebox.showwarning('Ошибка ввода',
                                       'a и b не могут быть нулевыми!')
        except:
            messagebox.showwarning('Ошибка ввода',
                                   'Нечисловое значение для параметров отрезка!')

def sev_elli(choise):
    a, b, delta, num = ae_b.get(), be_b.get(), del_b.get(), am_b.get()
    if not a or not b or not delta or not num:
        messagebox.showwarning('Ошибка ввода',
                               'Какие-то параметры не заданы!')
    else:
        try:
            a, b, delta, num = round(float(a)), round(float(b)), round(float(delta)), round(float(num))
            if a != 0 and b != 0 and num != 0:
                xc = 350
                yc = 200
                if choise[0] != 4:
                    d = 0
                    c = b / a
                    for i in range(num):
                        funcs_e[choise[0]](xc, yc, a + d, (b + d) * c)
                        d += delta
                else:
                    d = 0
                    c = b / a
                    for i in range(num):
                        canvas.create_oval(xc - a - d, yc - (b + d) * c, xc + a + d, yc + (b + d) * c, outline=line_color)
                        d += delta
            else:
                messagebox.showwarning('Ошибка ввода',
                                       'Радиус равен 0!')
        except:
            messagebox.showwarning('Ошибка ввода',
                                   'Нечисловое значение для параметров отрезка!')

# Получение параметров для отрисовки
def draw(test_mode):
    choise = method_list.curselection()
    if len(choise) == 1:
        if test_mode == 0:
            one_circle(choise)
        if test_mode == 1:
            sev_circles(choise)
        if test_mode == 2:
            one_elli(choise)
        if test_mode == 3:
            sev_elli(choise)
    elif not len(choise):
        messagebox.showwarning('Ошибка ввода',
                               'Не выбран метод построения отрезка!')
    else:
        messagebox.showwarning('Ошибка ввода',
                               'Выбрано более одного метода простроения отрезка!')


# Получение параметров для анализа
def analyze(mode):
    try:
        length = 100
        if not mode:
            time_bar(length)
        else:
            ind = method_list.curselection()
            if ind:
                if ind[-1] != 5:
                    smoth_analyze(ind, length)
                else:
                    messagebox.showwarning('Предупреждение',
                                           'Стандартный метод не может '
                                           'быть проанализирован на ступенчатость!')
            else:
                messagebox.showwarning('Предупреждение',
                                       'Не выбрано ни одного'
                                       'метода построения отрезка!')
    except:
        messagebox.showwarning('Предупреждение',
                               'Введено нечисловое значение для длины отрезка!')


# замер времени
def test(flag, ind, method, angle, pb, pe):
    global line_color
    total = 0
    steps = int(360 // angle)
    for i in range(steps):
        cur1 = time.time()
        if flag == 0:
            method(pb, pe)
        else:
            method(pb, pe, fill=line_color)
        cur2 = time.time()
        turn_point(radians(angle), pe, pb)
        total += cur2 - cur1
    return total / steps


# гистограмма времени
def time_bar(length):
    close_plt()
    plt.figure(2, figsize=(9, 7))
    times = []
    angle = 1
    pb = [center[0], center[1]]
    pe = [center[0] + 100, center[1]]
    for i in range(5):
        times.append(test(0, i, test_funcs_c[i], angle, pb, pe))
    clean()
    Y = range(len(times))
    L = ('Digital\ndifferential\nanalyzer', 'Bresenham\n(real coeffs)',
         'Bresenham\n(int coeffs)', 'Bresenham\n(smooth)', 'Wu')
    plt.bar(Y, times, align='center')
    plt.xticks(Y, L)
    plt.ylabel("Work time in sec. (line len. " + str(length) + ")")
    plt.show()

# Поворот точки для сравнения ступенчатости
def turn_point(angle, p, center):
    x = p[0]
    p[0] = round(center[0] + (x - center[0]) * cos(angle) + (p[1] - center[1]) * sin(angle))
    p[1] = round(center[1] - (x - center[0]) * sin(angle) + (p[1] - center[1]) * cos(angle))

# Анализ ступечатости
def smoth_analyze(methods, length):
    close_plt()
    names = ('Digital\ndifferential\nanalyzer', 'Bresenham\n(real coeffs)',
             'Bresenham\n(int coeffs)', 'Bresenham\n(smooth)', 'Wu')
    plt.figure(1)
    plt.title("Stepping analysis")
    plt.xlabel("Angle")
    plt.ylabel("Number of steps(line length " + str(length) + ")")
    plt.grid(True)
    plt.legend(loc='best')

    for i in methods:
        max_len = []
        nums = []
        angles = []
        angle = 0
        step = 2
        pb = [center[0], center[1]]
        pe = [center[0] + length, center[1]]

        for j in range(90 // step):
            stairs = funcs_e[i](pb, pe, line_color)
            turn_point(radians(step), pe, pb)
            if stairs:
                max_len.append(max(stairs))
            else:
                max_len.append(0)
            nums.append(len(stairs))
            angles.append(angle)
            angle += step
        clean()
        plt.figure(1)
        plt.plot(angles, nums, label=names[i])
        plt.legend()
    plt.show()

# Оси координат
def draw_axes():
    color = 'gray'
    canvas.create_line(0, 2, 750, 2, fill="darkred", arrow=LAST)
    canvas.create_line(2, 0, 2, 400, fill="darkred", arrow=LAST)
    for i in range(50, 750, 50):
        canvas.create_text(i, 10, text=str(abs(i)), fill=color)
        canvas.create_line(i, 0, i, 5, fill=color)

    for i in range(25, 400, 25):
        canvas.create_text(15, i, text=str(abs(i)), fill=color)
        canvas.create_line(0, i, 5, i, fill=color)

# очистка канваса
def clean():
    canvas.delete("all")
    draw_axes()

# Список методов прорисовки отрезка
def fill_list(lst):
    lst.insert(END, "Брезенхем")
    lst.insert(END, "Алгоритм средней точки")
    lst.insert(END, "Параметрическое уравнение")
    lst.insert(END, "Каноническое уравнение")
    lst.insert(END, "Алгоритм Ткинтера")

def set_bgcolor(color):
    global bg_color
    bg_color = color
    canvas.configure(bg=bg_color)


def set_linecolor(color):
    global line_color
    line_color = color
    lb_lcolor.configure(bg=line_color)

def close_plt():
    plt.figure(1)
    plt.close()
    plt.figure(2)
    plt.close()

def close_all():
    if messagebox.askyesno("Выход", "Вы действительно хотите завершить программу?"):
        close_plt()
        root.destroy()

root = Tk()
root.geometry('760x600')
root.resizable(0, 0)
root.title('Лабораторная работа №3')
color_menu = "#FFF0F5"

one_circle_frame = Frame(root, bg=color_menu, height=200, width=150)
one_circle_frame.place(x=0, y=0)

many_cir_frame = Frame(root, bg=color_menu, height=200, width=150)
many_cir_frame.place(x=150, y=0)

one_ell_frame = Frame(root, bg=color_menu, height=200, width=150)
one_ell_frame.place(x=300, y=0)

many_ellipse_frame = Frame(root, bg=color_menu, height=200, width=150)
many_ellipse_frame.place(x=450, y=0)

menu_frame = Frame(root, bg=color_menu, height=200, width=160)
menu_frame.place(x=600, y=0)

canv = Canvas(root, width=760, height=400, bg='white')
canvas = canv
canvas_test = canv
canv.place(x=0, y=200)
center = (375, 200)

# Список Алгоритмов
method_list = Listbox(many_cir_frame, selectmode=EXTENDED)
method_list.place(x=10, y=110, width=130, height=85)
fill_list(method_list)

funcs_c = (bresenham_circle, middle_dot_circle, param_cicrle,
           canon_circle)
funcs_e = (bresenham_ellipse, middle_dot_ellipse, param_ellipse,
           canon_ellipse)
#test_funcs_c = (kan_test_c, par_test_c, bres_test_c, sr_test_c)
#test_funcs_e = (kan_test_e, par_test_e, bres_test_e, sr_test_e)

# ONE CIRCLE

lb1 = Label(one_circle_frame, bg=color_menu, text='Одна окружность:')
lb1.place(x=25, y=2)

lbx1 = Label(one_circle_frame, bg=color_menu, text='Центр Х:')
lbx1.place(x=15, y=20)
lby1 = Label(one_circle_frame, bg=color_menu, text='Центр Y:')
lby1.place(x=15, y=40)
lbx2 = Label(one_circle_frame, bg=color_menu, text='Радиус:')
lbx2.place(x=15, y=60)

xc_b = Entry(one_circle_frame, bg="white")
xc_b.place(x=105, y=20, width=35)
yc_b = Entry(one_circle_frame, bg="white")
yc_b.place(x=105, y=40, width=35)
r_b = Entry(one_circle_frame, bg="white")
r_b.place(x=105, y=60, width=35)

btn_draw1 = Button(one_circle_frame, text=u"Построить", command=lambda: draw(0), width=140, height=25)
btn_draw1.place(x=40, y=85, width=70, height=20)

# SEVERAL CIRCLES

lb2 = Label(many_cir_frame, bg=color_menu, text='Концентр. окр-ти:')
lb2.place(x=25, y=2)

lbx3 = Label(many_cir_frame, bg=color_menu, text='Радиус:')
lbx3.place(x=15, y=20)
lby2 = Label(many_cir_frame, bg=color_menu, text='delta:')
lby2.place(x=15, y=40)
lbx4 = Label(many_cir_frame, bg=color_menu, text='Кол-во:')
lbx4.place(x=15, y=60)

rad_b = Entry(many_cir_frame, bg="white")
rad_b.place(x=105, y=20, width=35)
delta_b = Entry(many_cir_frame, bg="white")
delta_b.place(x=105, y=40, width=35)
num_b = Entry(many_cir_frame, bg="white")
num_b.place(x=105, y=60, width=35)

btn_draw2 = Button(many_cir_frame, text=u"Построить", command=lambda: draw(1), width=140, height=25)
btn_draw2.place(x=40, y=85, width=70, height=20)

# ONE ELLIPSE

lb3 = Label(one_ell_frame, bg=color_menu, text='Один эллипс:')
lb3.place(x=25, y=2)

lbx5 = Label(one_ell_frame, bg=color_menu, text='Центр Х:')
lbx5.place(x=15, y=20)
lby3 = Label(one_ell_frame, bg=color_menu, text='Центр Y:')
lby3.place(x=15, y=40)
lbx6 = Label(one_ell_frame, bg=color_menu, text='а:')
lbx6.place(x=15, y=60)
lbx7 = Label(one_ell_frame, bg=color_menu, text='b:')
lbx7.place(x=15, y=80)

c_x_b = Entry(one_ell_frame, bg="white")
c_x_b.place(x=105, y=20, width=35)
c_y_b = Entry(one_ell_frame, bg="white")
c_y_b.place(x=105, y=40, width=35)
a_b = Entry(one_ell_frame, bg="white")
a_b.place(x=105, y=60, width=35)
b_b = Entry(one_ell_frame, bg="white")
b_b.place(x=105, y=80, width=35)

btn_draw3 = Button(one_ell_frame, text=u"Построить", command=lambda: draw(2), width=140, height=25)
btn_draw3.place(x=40, y=110, width=70, height=20)

# MANY ELLIPSE

lb4 = Label(many_ellipse_frame, bg=color_menu, text='Концентр. эллипсы:')
lb4.place(x=25, y=2)

lbx8 = Label(many_ellipse_frame, bg=color_menu, text='a:')
lbx8.place(x=15, y=20)
lby4 = Label(many_ellipse_frame, bg=color_menu, text='b:')
lby4.place(x=15, y=40)
lbx9 = Label(many_ellipse_frame, bg=color_menu, text='delta:')
lbx9.place(x=15, y=60)
lbx10 = Label(many_ellipse_frame, bg=color_menu, text='Кол-во:')
lbx10.place(x=15, y=80)

ae_b = Entry(many_ellipse_frame, bg="white")
ae_b.place(x=105, y=20, width=35)
be_b = Entry(many_ellipse_frame, bg="white")
be_b.place(x=105, y=40, width=35)
del_b = Entry(many_ellipse_frame, bg="white")
del_b.place(x=105, y=60, width=35)
am_b = Entry(many_ellipse_frame, bg="white")
am_b.place(x=105, y=80, width=35)

btn_draw3 = Button(many_ellipse_frame, text=u"Построить", command=lambda: draw(3), width=140, height=25)
btn_draw3.place(x=40, y=110, width=70, height=20)

# выбор цветов

line_color = 'black'
bg_color = 'white'

size = 15
white_line = Button(menu_frame, bg="white", activebackground="white",
                    command=lambda: set_linecolor('white'))
white_line.place(x=15, y=30, height=size, width=size)
black_line = Button(menu_frame, bg="yellow", activebackground="black",
                    command=lambda: set_linecolor("yellow"))
black_line.place(x=30, y=30, height=size, width=size)
red_line = Button(menu_frame, bg="orange", activebackground="orange",
                  command=lambda: set_linecolor("orange"))
red_line.place(x=45, y=30, height=size, width=size)
orange_line = Button(menu_frame, bg="red", activebackground="red",
                     command=lambda: set_linecolor("red"))
orange_line.place(x=60, y=30, height=size, width=size)
yellow_line = Button(menu_frame, bg="purple", activebackground="purple",
                     command=lambda: set_linecolor("purple"))
yellow_line.place(x=75, y=30, height=size, width=size)
green_line = Button(menu_frame, bg="darkblue", activebackground="darkblue",
                    command=lambda: set_linecolor("darkblue"))
green_line.place(x=90, y=30, height=size, width=size)
doger_line = Button(menu_frame, bg="darkgreen", activebackground="darkgreen",
                    command=lambda: set_linecolor("darkgreen"))
doger_line.place(x=105, y=30, height=size, width=size)
blue_line = Button(menu_frame, bg="black", activebackground="black",
                   command=lambda: set_linecolor("black"))
blue_line.place(x=120, y=30, height=size, width=size)


white_bg = Button(menu_frame, bg="white", activebackground="white",
                  command=lambda: set_bgcolor("white"))
white_bg.place(x=15, y=70, height=size, width=size)
black_bg = Button(menu_frame, bg="yellow", activebackground="yellow",
                  command=lambda: set_bgcolor("yellow"))
black_bg.place(x=30, y=70, height=size, width=size)
red_bg = Button(menu_frame, bg="orange", activebackground="orange",
                command=lambda: set_bgcolor("orange"))
red_bg.place(x=45, y=70, height=size, width=size)
orange_bg = Button(menu_frame, bg="red", activebackground="red",
                   command=lambda: set_bgcolor("red"))
orange_bg.place(x=60, y=70, height=size, width=size)
yellow_bg = Button(menu_frame, bg="purple", activebackground="purple",
                   command=lambda: set_bgcolor("purple"))
yellow_bg.place(x=75, y=70, height=size, width=size)
green_bg = Button(menu_frame, bg="darkblue", activebackground="darkblue",
                  command=lambda: set_bgcolor("darkblue"))
green_bg.place(x=90, y=70, height=size, width=size)
dodger_bg = Button(menu_frame, bg="darkgreen", activebackground="darkgreen",
                   command=lambda: set_bgcolor("darkgreen"))
dodger_bg.place(x=105, y=70, height=size, width=size)
blue_bg = Button(menu_frame, bg="black", activebackground="black",
                 command=lambda: set_bgcolor("black"))
blue_bg.place(x=120, y=70, height=size, width=size)

lb_line = Label(menu_frame, bg=color_menu, text='Цвет линии (текущий:       ): ')
lb_line.place(x=2, y=5)
lb_lcolor = Label(menu_frame, bg=line_color)
lb_lcolor.place(x=135, y=9, width=12, height=12)
lb_bg = Label(menu_frame, bg=color_menu, text='Цвет фона: ')

# MENU

btn_clean = Button(menu_frame, text=u"Очистить экран", command=clean)
btn_clean.place(x=3, y=100, width=140, height=40)

btn_time = Button(menu_frame, text=u"Сравнение времени\nпостроения", command=lambda: analyze(0))
btn_time.place(x=3, y=150, width=140, height=40)
lb_bg.place(x=2, y=50)

draw_axes()
root.mainloop()