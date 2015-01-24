#include <QCoreApplication>
#include "dataprocessing.h"
#include <QDateTime>
#include <QPair>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    QFile file(":/testFiles/origin.txt");
    file.open(QIODevice::ReadOnly);
    QString block = file.readAll();
    std::set<DATABLOCK> BlockDateTime = processing.getDateTimeBlocks(block);
    for(std::set<DATABLOCK>::iterator i = BlockDateTime.begin(); i != BlockDateTime.end(); ++i)
        std::cout << (*i).second.toString("hh:mm:ss.z").toStdString() << std::endl;
    return a.exec();
}
