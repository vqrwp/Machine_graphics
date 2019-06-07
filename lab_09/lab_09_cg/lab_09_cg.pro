#-------------------------------------------------
#
# Project created by QtCreator 2019-05-19T19:27:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab_09_cg
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
    canvas.cpp \
    scalar_mult.cpp \
    vector_mult.cpp \
    vector_len.cpp \
    calculate_parametrical.cpp \
    distance.cpp \
    normal_funcs.cpp

HEADERS += \
    mainwindow.h \
    canvas.h \
    scalar_mult.h \
    vector_mult.h \
    vector_len.h \
    calculate_parametrical.h \
    distance.h \
    structs.h \
    normal_funcs.h

FORMS += \
    mainwindow.ui
