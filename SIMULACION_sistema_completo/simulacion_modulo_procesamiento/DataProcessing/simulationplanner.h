#ifndef SIMULATIONPLANNER_H
#define SIMULATIONPLANNER_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QString>
#include <QDir>
#include <QTimer>

class SimulationPlanner : public QObject
{
    Q_OBJECT
public:
    SimulationPlanner(QString event, QDir eventDir,QList<QPair<QString,int> > picks,
                      QList<QPair<QString,int> > origins,QObject *parent = 0);

public slots:
    void sendPick();
    void sendOrigin();
    void sendEvent();

private:
    QString requiredEventName;
    QDir requiredEventDir;
    QTimer* eventTimer;

    QList<QPair<QString,int> > picksBlocks;
    QTimer* picksTimer;
    int picksCounter;

    QList<QPair<QString,int> > originsBlocks;
    QTimer* originsTimer;
    int originsCounter;

};

#endif // SIMULATIONPLANNER_H
