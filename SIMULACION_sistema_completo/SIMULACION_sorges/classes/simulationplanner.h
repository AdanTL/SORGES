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
    SimulationPlanner();
    SimulationPlanner(QString event, QDir eventDir, QList<QPair<QString,int> > picks,
                      QList<QPair<QString,int> > origins, int duration, QObject *parent = 0);

public slots:
    void sendPick();
    void sendOrigin();
    void sendEvent();

private:
    QString requiredEventName;
    QDir requiredEventDir;
    QList<QPair<QString,int> > picksBlocks;
    QList<QPair<QString,int> > originsBlocks;
    int simulationDuration;
    QTimer* eventTimer;
    QTimer* picksTimer;
    QTimer* originsTimer;
    int picksCounter;
    int originsCounter;

};

#endif // SIMULATIONPLANNER_H

