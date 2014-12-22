#-------------------------------------------------
#
# Project created by QtCreator 2014-12-03T19:36:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sorges
TEMPLATE = app


SOURCES += main.cpp\
           widgets/mapwidget.cpp \
           classes/origin.cpp \
           classes/station.cpp

HEADERS  += widgets/mapwidget.h \
            classes/origin.h \
            classes/station.h \
            config/mapdefinition.h

FORMS    += ui/mapwidget.ui

RESOURCES += images/map.qrc
