#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QRegExp>
#include <iostream>
#include <set>
#include <QFile>
#include <QStringList>
#include <QPair>
#include <QList>
#include <QTimer>
#include <QTextStream>

// ADD into Configure file .h or Qsetting object.
#define FILE_INICIAL_STATIONS ":/testFiles/station.txt"

#define FILE_LOG_ORIGIN ":/testFiles/scalertes_origenes.log"
#define FILE_LOG_PICKS ":/testFiles/scalertes_picks.log"
#define FILE_XML_ORIGIN ":/testFiles/gfz2014dibx.last.xml"
#define DATEBLOCK QPair<QStringList,QDateTime>
#define ANIMATIONBLOCK QPair<QString,int>


class DataProcessing
{
signals:
    void writeNextSecuence(const QList<ANIMATIONBLOCK>& secuence, int i=0);
public slots:
    void writeSecuence(const QString &block);
public:
    DataProcessing();
    QString getBlockPick(const QDateTime& firstdatetime, const QDateTime& lastdatetime);
    QString getBlockOrigin(const QDateTime& firstdatetime, const QDateTime& lastdatetime);
    int getPositionBegin(const QDateTime& firstdatetime, const QString& namefile);
    int getPositionEnd(const QDateTime& lastdatetime, const QString& namefile);
    std::set<DATEBLOCK> getDateTimeBlocks(const QString& block);
    QList<ANIMATIONBLOCK> getSecuence(const std::set<DATEBLOCK>&blocks);
private:
    QList<ANIMATIONBLOCK> secuence;
    int n=-1;
    QTimer *printSecuence;

};

#endif // DATAPROCESSING_H
