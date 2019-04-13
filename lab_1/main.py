# На плоскости дано множество точек. Найти такой треугольник с вершинами в этих точках,
# для которого разность количества точек этого множества, попавших внутрь треугольника
# и за его пределы, но врутри описанной окружности, минимальна

from tkinter import *
from tkinter import messagebox
from math import *
import numpy as np

def compar(a, b):
    if fabs(a - b) < 0.000001:
        return 0
    if a > b:
        return 1
    return -1

# длина отрезка
def seg_len(x, y):
    return sqrt((x[0] - y[0]) * (x[0] - y[0]) + (x[1] - y[1]) * (x[1] - y[1]))

# знаковая площадь треугольника
def tr_area(a, b, c):
    print("a =", a, "b =", b, "c =", c)
    s = (b[0] - a[0]) * (c[1] - a[1]) - (c[0] - a[0]) * (b[1] - a[1])
    print(s / 2)
    return s / 2

# количество точек в окружности (с контуром)
def num_points_circle(r, xy, centre):
    r = r * r
    res = 0
    for i in range(len(xy)):
        l = (xy[i][0] - centre[0]) * (xy[i][0] - centre[0]) + \
               (xy[i][1] - centre[1]) * (xy[i][1] - centre[1])
        print("l = ", l)
        if compar(l, r) <= 0:  # для включения контура
            res += 1
    return res

# количество точек в треугольнике (c контуром)
def num_points_triangle(a, b, c, xy):
    res = 0
    s = fabs(tr_area(a, b, c))
    print("S = ", s)
    for i in range(len(xy)):
        tmp = fabs(tr_area(a, b, xy[i])) + fabs(tr_area(xy[i], b, c)) \
              + fabs(tr_area(a, xy[i], c))
        print("tmp = ", tmp)
        if compar(tmp, s) == 0:
            res += 1
    return res

# поиск разности кол-ва точек внутри тр-ка и внутри окр-ти
def search_diff(a, b, c, xy, centre):
    s = tr_area(a, b, c)
    if compar(s, 0) == 0:
        print("Проверяемые 3 точки лежат на одной прямой")
        return 1000 # точки лежат на одной прямой, но не все, а те которые проверяют
    search_circle_centre(a, b, c, centre)
    r = seg_len(a, centre)
    num_in_tr = num_points_triangle(a, b, c, xy)
    num_in_circle = num_points_circle(r, xy, centre)
    inn = num_in_tr
    outt = num_in_circle - num_in_tr + 3
    return abs(inn - outt)

# поиск центра окружности
def search_circle_centre(a, b, c, centre):
    xc = -1 / 2 * (a[1] * (b[0] ** 2 + b[1] ** 2 - c[0] ** 2 - c[1] ** 2) + b[1] *
        (c[0] ** 2 + c[1] ** 2 - a[0] ** 2 - a[1] ** 2) +
        c[1] * (a[0] ** 2 + a[1] ** 2 - b[0] ** 2 - b[1] ** 2)) / \
        (a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) +
        c[0] * (a[1] - b[1]))
    yc = 1 / 2 * (a[0] * (b[0] ** 2 + b[1] ** 2 - c[0] ** 2 - c[1] ** 2) + b[0] *
        (c[0] ** 2 + c[1] ** 2 - a[0] ** 2 - a[1] ** 2) +
        c[0] * (a[0] ** 2 + a[1] ** 2 - b[0] ** 2 - b[1] ** 2)) / (a[0] * (b[1] - c[1]) + b[0] *
        (c[1] - a[1]) + c[0] * (a[1] - b[1]))
    centre.clear()
    centre.append(xc)
    centre.append(yc)

def form_xy(ax, ay):
    xy = []
    for i in range(len(ax)):
        xy.append([0] * 2)
    for i in range(len(ax)):
        xy[i][0] = ax[i]
        xy[i][1] = ay[i]
    return xy

def get_coords(ans):
    # левая координата
    x1 = ans[3][0] - ans[4] * 1.40
    y1 = ans[3][1]
    # нижняя координата
    x2 = ans[3][0]
    y2 = ans[3][1] + ans[4] * 1.40
    # правая координата
    x3 = ans[3][0] + ans[4] * 1.40
    y3 = ans[3][1]
    # верхняя координата
    x4 = ans[3][0]
    y4 = ans[3][1] - ans[4] * 1.40

    min_x = min(x1, x2, x3, x4)
    min_y = min(y1, y2, y3, y4)
    p = q = 0
    # централизируем все иксы
    if min_x <= 0:
        p = min_x
        x1 -= p
        x2 -= p
        x3 -= p
        x4 -= p

    # централизируем все игреки
    if min_y <= 0:
        q = min_y
        y1 -= q
        y2 -= q
        y3 -= q
        y4 -= q

    k = 500 / max(x1, x2, x3, x4)

    # высчитываем коэффициент масштабирования
    if k * max(y1, y2, y3, y4) > 400:
        k = 400 / max(y1, y2, y3, y4)
    return k, p, q

def close_all(dot, top):
    dot.destroy()
    top.destroy()

def delete_dots(dot, top):
    dot.destroy()
    top.destroy()
    main()

def column(matrix, i):
    return [x[i] for x in matrix]

def deleting(dot, top, ind, xy, new):
    del xy[int(ind) - 1]
    dot.destroy()
    top.destroy()
    x = y = []
    x = column(xy, 0)
    y = column(xy, 1)
    new.destroy()
    create_paper(x, y)

def changing(dot, top, ind, xy, new, newx, newy):
    if not (ind and newx and newy):
        messagebox.showinfo("Ошибка", "Все поля должны быть заполнены!")
    elif int(ind) > len(xy):
        messagebox.showinfo("Ошибка", "Максимальный индекс " + str(len((xy))))
    else:
        xy[int(ind) - 1][0] = float(newx)
        xy[int(ind) - 1][1] = float(newy)
        dot.destroy()
        top.destroy()
        x = y = []
        x = column(xy, 0)
        y = column(xy, 1)
        new.destroy()
        create_paper(x, y)

def delete_by_index(xy, dot, top):
    new = Tk()
    new.title("Ответ")
    index = Label(new, text='Введите индекс, который хотите удалить: ')
    index.grid(column=2)

    get_ind = Entry(new)
    get_ind.grid(column=2)
    try:
        butt = Button(new, text="Ввод", command=lambda: deleting(dot, top, get_ind.get(), xy, new))
        butt.grid(column=2)
    except:
        messagebox.showinfo('Ошибка', 'Некорректный ввод')


def change_by_index(xy, dot, top):
    new = Tk()
    new.title("Ответ")
    index = Label(new, text='Введите индекс, который хотите изменить: ')
    index.grid(column=2)

    get_ind = Entry(new)
    get_ind.grid(column=2)

    valueX = Label(new, text='Введите координату х ')
    valueX.grid(column=2)
    x_en = Entry(new, width=20)
    x_en.grid(column=2)

    valueY = Label(new, text='Введите координату y', anchor='w')
    valueY.grid(column=2)
    y_en = Entry(new, width=20)
    y_en.grid(column=2)
    try:
        butt = Button(new, text="Ввод", command=lambda:
        changing(dot, top, get_ind.get(), xy, new, x_en.get(), y_en.get()))
        butt.grid()
    except:
        messagebox.showinfo('Ошибка', 'Некорректный ввод')

def menu(centre, ans, diff, r, top, xy):
    dot = Tk()
    dot.title("Ответ")
    centre[0] = round(centre[0], 2)
    centre[1] = round(centre[1], 2)
    dots = Label(dot, text="Координаты вершин треугольника:\n" + str(ans[0]) + "\n" + str(ans[1]) +
        "\n" + str(ans[2]) + "\nРазница между точками внутри тр-ка и круга: " + str(diff) +
                           "\nЦентр: " + str(centre) + "\nРадиус круга: " + str(round(r, 2)))
    dots.grid(row=1, column=2)

    box_label = Label(dot, text="Список всех точек:")
    box_label.grid(row=3, column=2)
    lbox = Listbox(dot, selectmode=EXTENDED)
    lbox.grid(column=2)
    for i in range(len(xy)):
        lbox.insert(END, "№" + str(i + 1) + "    " + str(xy[i]))

    m = Menu(dot)
    dot.config(menu=m)

    cm = Menu(m)
    m.add_cascade(label="Меню", menu=cm)
    cm.add_command(label="Удалить все точки, начать заново", command=lambda: delete_dots(dot, top))
    cm.add_command(label="Удалить одну точку по индексу, построить рисунок",
                   command=lambda: delete_by_index(xy, dot, top))
    cm.add_command(label="Изменить одну точку по индексу", command=lambda: change_by_index(xy, dot, top))
    cm.add_command(label="Завершить все", command=lambda: close_all(dot, top))

def draw(ans, paper, xy, diff, centre, circle, r, top):
    k, p, q = get_coords(ans)
    menu(centre, ans, diff, r, top, xy)

    # рисуем стороны треугольника
    paper.create_line((ans[0][0] - p) * k, 500 - (ans[0][1] - q) * k,
                      (ans[1][0] - p) * k, 500 - (ans[1][1] - q) * k, fill='green')

    paper.create_line((ans[1][0] - p) * k, 500 - (ans[1][1] - q) * k,
                      (ans[2][0] - p) * k, 500 - (ans[2][1] - q) * k, fill='green')

    paper.create_line((ans[2][0] - p) * k, 500 - (ans[2][1] - q) * k,
                      (ans[0][0] - p) * k, 500 - (ans[0][1] - q) * k, fill='green')

    # рисуем точки, которые вошли в треугольник
    for i in range(3):
        paper.create_oval([(ans[i][0] - p) * k - 3, 500 - (ans[i][1] - q) * k + 3],
                          [(ans[i][0] - p) * k + 3, 500 - (ans[i][1] - q) * k - 3], fill='light blue')

    circle.clear()
    for t in np.arange(0, 2 * pi, 0.1):
        x = ans[3][0] + ans[4] * cos(t)
        y = ans[3][1] + ans[4] * sin(t)
        circle.append([(x - p) * k, 500 - (y - q) * k])

    # рисуем круг
    paper.create_polygon(circle, fill='', outline='black')

    alpha = ['A', 'B', 'C', 'D', 'F', 'E', 'G', 'K', 'L', 'M', 'N']
    # рисуем точки, которые не вошли в треугольник
    for i in range(len(xy)):
        paper.create_oval([(xy[i][0] - p) * k - 3, 500 - (xy[i][1] - q) * k + 3],
                          [(xy[i][0] - p) * k + 3, 500 - (xy[i][1] - q) * k - 3], fill='white')
        paper.create_text((xy[i][0] - p) * k, 500 - (xy[i][1] - q) * k, text=alpha[i] + str(xy[i]),
                          anchor="n", justify=LEFT, fill='dark blue')

    ans.clear()

def find_trngl(ax, ay, paper, top):
    xy = form_xy(ax, ay)
    n = len(ax)
    centre, a, b, c, ans, circle = [], [], [], [], [], []
    diff = 1000  # начальная разница
    for i in range(n):
        for j in range(i + 1, n):
            for k in range(j + 1, n):
                t = search_diff(xy[i], xy[j], xy[k], xy, centre)
                if t < diff:
                    a = xy[i]
                    b = xy[j]
                    c = xy[k]
                    diff = t
    if diff == 1000:
        return -1
    search_circle_centre(a, b, c, centre)
    r = seg_len(a, centre)

    ans.append(a)
    ans.append(b)
    ans.append(c)
    ans.append(centre)
    ans.append(r)
    ans.append(diff)

    draw(ans, paper, xy, diff, centre, circle, r, top)
    return 0

def create_paper(x, y):
    print("x:", x)
    print("y:", y)
    top = Tk()
    top.title("Рисунок")

    paper = Canvas(top, bg='white', width=500, height=500)

    if paper.winfo_height() < top.winfo_height():
        paper.config(height=top.winfo_height())
    if paper.winfo_width() < top.winfo_width():
        paper.config(width=top.winfo_width())

    paper.delete('all')
    flag = find_trngl(x, y, paper, top)
    if flag == -1 or len(x) < 3:
        messagebox.showinfo("Ошибка", "Все точки лежат на одной прямой")
        top.destroy()
    else:
        paper.grid()

def coords(ax, ay, x, y, num, root):
    try:
        for i in range(len(x)):
            ax.append(float(eval(x[i].get())))
            ay.append(float(eval(y[i].get())))
    except:
        messagebox.showinfo("Ошибка", "Некорректный ввод")
    if len(ax) == num:
        root.destroy()
        create_paper(ax, ay)

def entering_dots(root, num):
    i = 4
    j = 0
    x_en, y_en = [], []
    coords_x, coords_y = [], []
    k = 0
    try:
        k = int(eval(num))
        if k < 3:
            messagebox.showinfo("Ошибка", "Точек должно быть больше 2ух")
    except:
        messagebox.showinfo("Ошибка", "Некорректный ввод")
    if k >= 3:
        while i < k * 3 + 4:
            numer = Label(root, width=10, text='Номер точки')
            numer.grid(row=i, column=0, columnspan=1)
            dot = Label(root,  width=10, text=str(j + 1))
            dot.grid(row=i + 1, column=0, columnspan=1)

            Input = Label(root, text='Введите координату х ')
            Input.grid(row=i, column=1)
            x_en.append(Entry(root,  width=20))
            x_en[j].grid(row=i + 1, column=1)

            valueY = Label(root, text='Введите координату y', anchor='w')
            valueY.grid(row=i, column=2)
            y_en.append(Entry(root,  width=20))
            y_en[j].grid(row=i + 1, column=2)

            j += 1
            i += 3
        end = Button(root, text="Закончить ввод координвт",
                    command=lambda: coords(coords_x, coords_y, x_en, y_en, k, root))
        end.grid()

def main():
    root = Tk()
    root.title("Лабораторная №1")
    #root.geometry('400x300+550+100')
    q = Label(root, text="Вариант №13\n\n\n"
                    "На плоскости дано множество точек.\n "
                     "Найти такой треугольник с вершинами в этих точках, \n"
                     "для которго разность количеств точек этого множества, \n"
                     " попавших внутрь треугольникаи за его пределы, \n"
                     "но внутри описанной окружности, минимальна.\n\n\n"
                    "Мищенко Маргарита, группа ИУ7-45Б")
    q.grid()

    num = Label(root, text='Введите количество точек (минимум три): ')
    num.grid()

    num_en = Entry(root)
    num_en.grid()
    #print(eval(num_en.get()))

    butt = Button(root, text="Ввод", command=lambda: entering_dots(root, num_en.get()))
    butt.grid()


    root.mainloop()

main()