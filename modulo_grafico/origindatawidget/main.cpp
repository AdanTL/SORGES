#include "origindatawidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OriginDataWidget w;
    w.show();

    return a.exec();
}
