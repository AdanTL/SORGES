#include <QCoreApplication>
#include "dataprocessing.h"
#include <QDateTime>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    QDateTime firstDate = QDateTime::fromString("2015-01-20 11:30:34.1" ,"yyyy-MM-dd hh:mm:ss.z");
    QDateTime lastDate = QDateTime::fromString("2015-01-20 11:30:35.0" ,"yyyy-MM-dd hh:mm:ss.z");

    std::cerr << processing.getBlockOrigin(firstDate,lastDate).toStdString() << std::endl;
    return a.exec();
}
