#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QRegExp>
#include <iostream>
#include <set>
#include <QFile>
#include <QStringList>


// ADD into Configure file .h or Qsetting object.
#define FILE_INICIAL_STATIONS ":/testFiles/station.txt"

#define FILE_LOG_ORIGIN ":/testFiles/scalertes_origenes.log"
#define FILE_LOG_PICKS ":/testFiles/scalertes_picks.log"
#define FILE_XML_ORIGIN ":/testFiles/gfz2014dibx.last.xml"



class DataProcessing
{
public:
    DataProcessing();
    QString getBlockPick(const QDateTime firstdatetime, const QDateTime lastdatetime);
    QString getBlockOrigin(const QDateTime firstdatetime, const QDateTime lastdatetime);
    int getPositionPickBegin(const QDateTime firstdatetime, const QString& namefile);
    int getPositionEnd(const QDateTime lastdatetime, const QString& namefile);

};

#endif // DATAPROCESSING_H
