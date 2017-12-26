#-------------------------------------------------
#
# Project created by QtCreator 2017-12-14T20:12:27
#
#-------------------------------------------------

QT += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vc820dmm
TEMPLATE = app


SOURCES += \
    vc820win.cpp \
    vc820main.cpp

HEADERS  += \
    vc820win.h

FORMS    += \
    vc820win.ui
