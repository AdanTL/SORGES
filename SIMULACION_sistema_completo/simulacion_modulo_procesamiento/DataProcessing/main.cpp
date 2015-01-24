#include <QCoreApplication>
#include "dataprocessing.h"
#include <QDateTime>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    QDateTime datetime = QDateTime::fromString("2015-01-20 11:30:02.6" ,"yyyy-MM-dd hh:mm:ss.z");
    processing.getPositionEnd(datetime, ":/testFiles/scalertes_picks.log");

    return a.exec();
}
