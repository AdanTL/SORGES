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
    QString blockOrigin = processing.getBlockOrigin(QDateTime::fromString("2015-01-20 11:30:34.1","yyyy-MM-dd hh:mm:ss.z"), QDateTime::fromString("2015-01-20 11:30:40.6","yyyy-MM-dd hh:mm:ss.z"));
    std::set<DATEBLOCK> blockDateTime = processing.getDateTimeBlocks(block);
    QList<ANIMATIONBLOCK> blockSecuence = processing.getSecuence(blockDateTime);
    std::cout << processing.getLastDate().toString("yyyy-MM-dd hh:mm:ss.z").toStdString() << " " << processing.getDurationToXml() << std::endl;
    SimulationPlanner planner("prueba",QDir(QDir::home ()),blockSecuence,blockSecuence);
    a.exec ();
}
