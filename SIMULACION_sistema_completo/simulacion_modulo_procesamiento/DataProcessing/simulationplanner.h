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

signals:

public slots:
    void prueba();

private:
    QString requiredEventName;
    QDir requiredEventDir;
    QList<QPair<QString,int> > picksBlocks;
    QList<QPair<QString,int> > originsBlocks;
    int cont;
    QTimer* timer;

};

#endif // SIMULATIONPLANNER_H
