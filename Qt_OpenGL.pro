#-------------------------------------------------
#
# Project created by QtCreator 2016-03-29T19:06:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_OpenGL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    geometryengine.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    geometryengine.h

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc
