#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T18:22:34
#
#-------------------------------------------------

QT       += core
QT       += xml

QT       -= gui

TARGET = DataProcessing
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    dataprocessing.cpp \
    origin.cpp \
    station.cpp

HEADERS += \
    dataprocessing.h \
    origin.h \
    station.h

DISTFILES +=

RESOURCES += \
    fileresource.qrc
