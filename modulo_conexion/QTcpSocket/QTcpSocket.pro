QT       += core
QT       += network
QT       -= gui

TARGET = QTcpSocket
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mytcpsocket.cpp

HEADERS += \
    mytcpsocket.h
