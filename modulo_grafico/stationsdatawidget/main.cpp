#include "stationsdatawidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StationsDataWidget w;
    w.show();

    return a.exec();
}
