#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QRegExp>
#include <iostream>
#include <set>
#include <QFile>
#include <QStringList>
#include "origin.h"
#include "station.h"

class DataProcessing
{
public:
    DataProcessing();
    Origin ProcessOriginFromFile(const QString &namefile);
    std::set<Station> ProcessStationsFromFile(const QString &namefile);
    QString ProcessColorStationsFromFile(const QString &namefile);
//private:
    Origin origen;
    std::set<Station> stations;

    QString FindParameterOriginID(const QString &originString);
    QString FindParameterOriginDate(const QString &originString);
    QString FindParameterOriginTime(const QString &originString);
    QString FindParameterOriginLatitude(const QString &originString);
    QString FindParameterOriginLongitude(const QString &originString);
    QString FindParameterOriginMagnitude(const QString &originString);
    std::vector<QStringList> FindParameterOriginStations(const QString &originString);

    std::vector<QStringList> FindParameterStations(const QString &stationsString);
};

#endif // DATAPROCESSING_H
