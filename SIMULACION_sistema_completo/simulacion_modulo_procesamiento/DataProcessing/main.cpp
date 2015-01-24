#include <QCoreApplication>
#include "dataprocessing.h"
#include <QDateTime>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    QDateTime datetime = QDateTime::fromString("2015-01-20 11:30:55.9" ,"yyyy-MM-dd hh:mm:ss.z");
    processing.getPositionPickBegin(datetime, ":/testFiles/scalertes_picks.log");

    return a.exec();
}
