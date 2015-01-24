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
    eventTimer(new QTimer(this)),
    picksTimer(new QTimer(this)),
    originsTimer(new QTimer(this)),
    picksCounter(0),
    originsCounter(0)
{
    //to start the simulation with a bit of delay in order to set everything
    int waitTime=3000;

    connect (eventTimer,SIGNAL(timeout()),this,SLOT(sendEvent()));
    connect (originsTimer,SIGNAL(timeout()),this,SLOT(sendOrigin()));
    connect (picksTimer,SIGNAL(timeout()),this,SLOT(sendPick()));

    picksTimer->start(waitTime);
    originsTimer->start(waitTime);
    //eventTimer->start(/*como sabemos el tiempo de duración?*/);
}


void SimulationPlanner::sendPick(){
    QFile file("/home/felipe/Escritorio/testPicks.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    out << endl << picksBlocks[picksCounter].first;
    file.close();
    std::cout<<"imprimiendo pick"<<QTime::currentTime().toString("hh:mm:ss.z").toStdString ()
             <<" "<<picksBlocks[picksCounter].second<<std::endl;
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
    std::cout<<"imprimiendo origin"<<QTime::currentTime().toString("hh:mm:ss.z").toStdString ()
             <<" "<<originsBlocks[originsCounter].second<<std::endl;
    originsCounter++;
    if (originsCounter == originsBlocks.size())
        originsTimer->stop ();
    else
        originsTimer->start(originsBlocks[picksCounter].second);
}

void SimulationPlanner::sendEvent (){

}
