#-------------------------------------------------
#
# Project created by QtCreator 2014-12-03T19:36:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mapwidget
TEMPLATE = app


SOURCES += main.cpp\
        mapwidget.cpp \
    origin.cpp \
    station.cpp

HEADERS  += mapwidget.h \
    origin.h \
    station.h \
    mapdefinition.h

FORMS    += mapwidget.ui

RESOURCES += \
    images/map.qrc
