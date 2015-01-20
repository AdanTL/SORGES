#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QRegExp>
#include <iostream>
#include <set>
#include <QFile>
#include <QStringList>
#include <QtXml/QDomDocument>
#include "origin.h"
#include "station.h"

// ADD into Configure file .h or Qsetting object.
#define FILE_INICIAL_STATIONS ":/testFiles/station.txt"

#define FILE_LOG_ORIGIN ":/testFiles/scalertes_origenes.log"
#define FILE_LOG_PICKS ":/testFiles/scalertes_picks.log"
#define FILE_XML_ORIGIN ":/testFiles/gfz2014dibx.last.xml"



class DataProcessing
{
public:
    DataProcessing();
    void ProcessAnyFile(const QString &namefile);
    void ProcessStationsFromFile(const QString &namefile);
    Origin getOrigin(){return origen;}
    std::set<Station> getStations(){return stations;}

//private:
    Origin origen;
    std::set<Station> stations;

    void ProcessOriginFromFileLog(const QString &namefile);
    void ProcessColorStationsFromFile(const QString &namefile);
    void ProcessOriginFromFileXml(const QString &namefile);

    QString FindParameterOriginID(const QString &originString);
    QString FindParameterOriginDate(const QString &originString);
    QString FindParameterOriginTime(const QString &originString);
    QString FindParameterOriginLatitude(const QString &originString);
    QString FindParameterOriginLongitude(const QString &originString);
    //QString FindParameterOriginMagnitude(const QString &originString);
    std::vector<QStringList> FindParameterOriginStations(const QString &originString);

    std::vector<QStringList> FindParameterStations(const QString &stationsString);
};

#endif // DATAPROCESSING_H
