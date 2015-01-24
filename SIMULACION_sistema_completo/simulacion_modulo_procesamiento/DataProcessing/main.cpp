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
    std::set<DATEBLOCK> blockDateTime = processing.getDateTimeBlocks(block);
    QList<ANIMATIONBLOCK> blockSecuence = processing.getSecuence(blockDateTime);
    for(int i=0; i<blockSecuence.size(); i++)
        std::cout << blockSecuence.at(i).first.toStdString() << "\n --------- \n" << blockSecuence.at(i).second << "\n --------- \n";
    return a.exec();
}
