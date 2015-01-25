#include "simulationplanner.h"
#include <iostream>
#include <QTime>
#include <QTextStream>
#include <QFile>

SimulationPlanner::SimulationPlanner(){}

SimulationPlanner::SimulationPlanner(QString event, QDir eventDir,
                                     QList<QPair<QString,int> > picks,
                                     QList<QPair<QString,int> > origins,
                                     int duration,
                                     QObject *parent) :
    QObject(parent),
    requiredEventName(event),
    requiredEventDir(eventDir),
    picksBlocks(picks),
    originsBlocks(origins),
    simulationDuration(duration),
    eventTimer(new QTimer(this)),
    picksTimer(new QTimer(this)),
    originsTimer(new QTimer(this)),
    picksCounter(0),
    originsCounter(0)
{
    //to start the simulation with a bit of delay in order to set everything
    int waitTime=2000;
    std::cout<<"Starting planner in 2 seconds"<<std::endl;

    connect (eventTimer,SIGNAL(timeout()),this,SLOT(sendEvent()));
    connect (originsTimer,SIGNAL(timeout()),this,SLOT(sendOrigin()));
    connect (picksTimer,SIGNAL(timeout()),this,SLOT(sendPick()));

    std::cout<<"Starting time triggers for data"<<std::endl;
    picksTimer->start(waitTime);
    originsTimer->start(waitTime);
    eventTimer->start(simulationDuration+waitTime);
}


void SimulationPlanner::sendPick(){
    QFile file("/home/felipe/Escritorio/testPicks.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    out << endl << picksBlocks[picksCounter].first;
    file.close();
    picksCounter++;
    if (picksCounter == picksBlocks.size())
        picksTimer->stop ();
    else
        picksTimer->start(picksBlocks[picksCounter].second);
}

void SimulationPlanner::sendOrigin(){
    QFile file("/home/felipe/Escritorio/testOrigins.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    out << endl << originsBlocks[originsCounter].first;
    file.close();
    originsCounter++;
    if (originsCounter == originsBlocks.size())
        originsTimer->stop ();
    else
        originsTimer->start(originsBlocks[picksCounter].second);
}

void SimulationPlanner::sendEvent (){

}

