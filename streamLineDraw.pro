#-------------------------------------------------
#
# Project created by QtCreator 2016-08-04T15:27:48
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Streamlines
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    vecfield.cpp \
    load.cpp \
    orieglwidget.cpp \
    ordered.cpp \
    legglwidget.cpp \
    scalarfield.cpp \
    colourmap.cpp \
    choosemap.cpp \
    clearpoint.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    vecfield.h \
    load.h \
    orieglwidget.h \
    ordered.h \
    legglwidget.h \
    scalarfield.h \
    colourmap.h \
    choosemap.h \
    clearpoint.h

FORMS    += mainwindow.ui \
    load.ui \
    choosemap.ui

LIBS += -lglut -lGLU
