#include <QCoreApplication>
#include <QXmlStreamReader>
#include "dataprocessing.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;

    //First instrucction.
    processing.ProcessStationsFromFile(":/testFiles/station.txt");


    //processing.ProcessOriginFromFileXml(":/testFiles/gfz2015bjph.last.xml");

    // Those instruction will be concurrent called.
    processing.ProcessColorStationsFromFile(":/testFiles/scalertes_picks.log");
    //processing.ProcessAnyFile(":/testFiles/scalertes_origenes.log");
    //processing.ProcessAnyFile(":/testFiles/gfz2015bjph.last.xml");
    processing.ProcessOriginFromFileXml(":/testFiles/gfz2015bjph.last.xml");

    //std::set<Station> stations(processing.getStations());
    std::cout << processing.getOrigin().toStdString() << "\n";

/*
    Origin origen;
    QDomDocument doc(":/testFiles/origin.txt");
    QFile file(":/testFiles/origin.txt");
    if (!file.open(QIODevice::ReadOnly))
        return  a.exec();
    if (!doc.setContent(&file)) {
        file.close();
        return  a.exec();
    }
    file.close();
    Origin::originsFromQDomDocument(doc);
  */
  return a.exec();
}
