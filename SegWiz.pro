#-------------------------------------------------
#
# Project created by QtCreator 2018-02-18T11:38:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SegWiz
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Model/dataset.cpp \
    Model/Shape/shape.cpp \
    Model/Shape/circle.cpp \
    Model/drawingbuffer.cpp \
    View/annotationwidget.cpp \
    Model/Shape/square.cpp \
    Model/label.cpp

HEADERS += \
        mainwindow.h \
    Model/dataset.h \
    Model/Shape/shape.h \
    Model/Shape/circle.h \
    Model/drawingbuffer.h \
    View/annotationwidget.h \
    Model/Shape/square.h \
    Model/label.h

DISTFILES += \
    README.md \
    LICENSE
