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
    std::set<DATEBLOCK> blockDateTime = processing.getDateTimeBlocks(block);
    QList<ANIMATIONBLOCK> blockSecuence = processing.getSecuence(blockDateTime);
    /*for(int i=0; i< blockSecuence.size(); i++){
        timerWriter.start(blockSecuence[i].second);
        processing.writeSecuence(blockSecuence[i].first);
    }*/
    SimulationPlanner planner("prueba",QDir(QDir::home ()),blockSecuence,blockSecuence);
    a.exec ();
}
