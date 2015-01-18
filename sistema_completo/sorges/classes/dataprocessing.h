#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QObject>
#include <QRegExp>
#include <iostream>
#include <set>
#include <QFile>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QSettings>
#include "origin.h"
#include "station.h"

class DataProcessing : public QObject
{
    Q_OBJECT

public:
    DataProcessing(QObject* parent = 0);

signals:
    void stationsLoaded(std::set<Station> stations);
    void originReceived(Origin origin);
    //void eventReceived(Origin origin); ???
    //void stationColorReceived(); ???

public slots:
    void init();

private slots:
    void fileChangedSlot(QString path);

    std::set<Station> ProcessStationsFromFile(const QString &namefile);

private:
    Origin origin;
    std::set<Station> stations;
    QFileSystemWatcher watcher;
    QSettings *config;


    std::vector<QStringList> FindParameterStations(const QString &stationsString);
};

#endif // DATAPROCESSING_H
