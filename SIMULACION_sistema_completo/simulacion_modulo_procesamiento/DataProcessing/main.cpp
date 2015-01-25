#include <QCoreApplication>
#include "dataprocessing.h"
#include <QDateTime>
#include <QPair>
#include "simulationplanner.h"

int main(int argc, char *argv[])
{
    //QTimer timerWriter;
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    QFile file(":/testFiles/origin.txt");
    file.open(QIODevice::ReadOnly);
    QString block = file.readAll();
    QString blockOrigin = processing.getBlockPick(QDateTime::fromString("2015-01-25 11:30:54.5","yyyy-MM-dd hh:mm:ss.z"), QDateTime::fromString("2015-01-25 11:30:56.4","yyyy-MM-dd hh:mm:ss.z"));
    std::cout << blockOrigin.toStdString() << std::endl;
    std::set<DATEBLOCK> blockDateTime = processing.getDateTimeBlocks(block);
    QList<ANIMATIONBLOCK> blockSecuence = processing.getSecuence(blockDateTime);
    //SimulationPlanner planner("prueba",QDir(QDir::home ()),blockSecuence,blockSecuence);
    a.exec ();
}
