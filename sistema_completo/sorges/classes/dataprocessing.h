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
    void stationColorReceived(std::set<Station> changedStation);
    void originReceived(Origin origin);
    void eventReceived(Origin origin);

public slots:
    void init();

private slots:
    void fileChangedSlot(QString path);
    void processStationsFromFile(const QString &namefile);
    std::set<Station> processColorStationsFromFile(const QString &namefile);
    void processOriginFromFileLog(const QString &namefile);
    //void processOriginFromFileXml(const QString &namefile);


private:
    Origin origin;
    std::set<Station> stations;
    QFileSystemWatcher watcher;
    QSettings *config;
    QDateTime lastDateTime;


    std::vector<QStringList> findParameterStations(const QString &stationsString);

    QString findParameterOriginID(const QString &originString);
    QString findParameterOriginDate(const QString &originString);
    QString findParameterOriginTime(const QString &originString);
    QString findParameterOriginLatitude(const QString &originString);
    QString findParameterOriginLongitude(const QString &originString);
    QString findParameterOriginMagnitude(const QString &originString);
};

#endif // DATAPROCESSING_H
