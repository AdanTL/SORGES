#-------------------------------------------------
#
# PROYECTO SORGES
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sorges
TEMPLATE = app


SOURCES += main.cpp\
           widgets/mapwidget.cpp \
           widgets/origindatawidget.cpp \
           widgets/stationsdatawidget.cpp \
           classes/origin.cpp \
           classes/station.cpp

HEADERS  += widgets/mapwidget.h \
			widgets/origindatawidget.h \
			widgets/stationsdatawidget.h \
            classes/origin.h \
            classes/station.h \
            config/mapdefinition.h

FORMS    += ui/mapwidget.ui \
			ui/origindatawidget.ui \
			ui/stationsdatawidget.ui

RESOURCES += images/map.qrc \
    images/logo.qrc
