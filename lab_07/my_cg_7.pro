#-------------------------------------------------
#
# Project created by QtCreator 2019-04-19T22:07:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab_7_cg
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    message_box.cpp \
    min_n_max.cpp \
    get_summ.cpp \
    get_summ_mults.cpp \
    draw_line.cpp \
    draw_rect.cpp \
    set_bits.cpp

HEADERS += \
        mainwindow.h \
    message_box.h \
    min_n_max.h \
    get_summ.h \
    get_summ_mults.h \
    draw_line.h \
    draw_rect.h \
    set_bits.h

FORMS += \
        mainwindow.ui
