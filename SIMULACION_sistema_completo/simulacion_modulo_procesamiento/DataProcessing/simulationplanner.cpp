#include "simulationplanner.h"
#include <iostream>
#include <QTime>
#include <QTextStream>
#include <QFile>

SimulationPlanner::SimulationPlanner(QString event, QDir eventDir,
                                     QList<QPair<QString,int> > picks,
                                     QList<QPair<QString,int> > origins,
                                     QObject *parent) :
    QObject(parent),
    requiredEventName(event),
    requiredEventDir(eventDir),
    picksBlocks(picks),
    originsBlocks(origins),
    cont(0),
    timer(new QTimer(this))
{
    int waitTime=5000;
    connect (timer,SIGNAL(timeout()),this,SLOT(prueba()));
    timer->start(waitTime);
}

void SimulationPlanner::prueba(){
    QFile file("/home/felipe/Escritorio/test.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    out << endl << picksBlocks[cont].first;
    file.close();
    std::cout<<"imprimiendo "<<QTime::currentTime().toString("hh:mm:ss.z").toStdString ()
             <<" "<<picksBlocks[cont].second<<std::endl;
    cont++;
    if (cont == picksBlocks.size ())
        timer->stop ();
    else
        timer->start(picksBlocks[cont].second);
}
