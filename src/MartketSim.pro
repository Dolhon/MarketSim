#-------------------------------------------------
#
# Project created by QtCreator 2014-10-23T15:00:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MartketSim
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    tilanne.cpp \
    simulaatio.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    tilanne.h \
    simulaatio.h

FORMS    += mainwindow.ui

CONFIG += static
