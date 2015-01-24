#include <QCoreApplication>
#include "dataprocessing.h"
#include <QDateTime>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    QFile file(":/testFiles/origin.txt");
    file.open(QIODevice::ReadOnly);
    QString block = file.readAll();
    processing.getDateTimeBlocks(block);
    return a.exec();
}
