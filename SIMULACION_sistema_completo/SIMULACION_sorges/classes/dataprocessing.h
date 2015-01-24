#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QObject>
#include <QDateTime>
#include <QRegExp>
#include <iostream>
#include <set>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QSettings>
#include <QtXml/QDomDocument>
#include "origin.h"
#include "station.h"

class DataProcessing : public QObject
{
    Q_OBJECT

public:
    DataProcessing(bool simulationMode, QObject* parent = 0);
    void dumpOriginXml();
    void dumpStationXml();

signals:
    void stationsLoaded(std::set<Station> stations);
    void stationColorReceived(std::set<Station> changedStation);
    void originReceived(Origin origin);
    void eventReceived(Origin origin);

public slots:
    void init();
    /**************SIMULATION***********************/
    void initSimulation(QDateTime simulationDateTime);
    /**************SIMULATION***********************/

private slots:
    void fileChangedSlot(QString path);
    void processStationsFromFile(const QString &namefile);
    std::set<Station> processColorStationsFromFile(const QString &namefile);
    void processOriginFromFileLog(const QString &namefile);
    void processOriginFromFileXml(const QString &namefile);


private:
    bool simulationMode;
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

    /**************SIMULATION***********************/
    QDateTime getDateTimeFromEvent(QDir eventFiles,QString eventName);
    QDateTime getDateTimeFirstPick(QDir eventFiles,QString eventName);
    QDateTime getLastDateTimeFromEvent(QDir eventFiles,QString eventName);
    QString getBlockPick(const QDateTime& firstdatetime, const QDateTime& lastdatetime);
    QString getBlockOrigin(const QDateTime& firstdatetime, const QDateTime& lastdatetime);
    int getPositionBegin(const QDateTime& firstdatetime, const QString& namefile);
    int getPositionEnd(const QDateTime& lastdatetime, const QString& namefile);
    /**************SIMULATION***********************/
};

#endif // DATAPROCESSING_H
