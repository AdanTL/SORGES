#include <QCoreApplication>
#include <QXmlStreamReader>
#include "dataprocessing.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    processing.ProcessOriginFromFileXml(":/testFiles/gfz2014dibx.last.xml");

    //First instrucction.
    //processing.ProcessStationsFromFile(":/testFiles/station.txt");

    // Those instruction will be concurrent called.
    //processing.ProcessAnyFile(":/testFiles/scalertes_picks.log");
    //processing.ProcessAnyFile(":/testFiles/origin.txt");
    //processing.ProcessAnyFile(":/testFiles/gfz2014dibx.last.xml");

    //std::set<Station> stations(processing.getStations());
    //Origin origen(processing.getOrigin());
    //origen.setStations(stations);
    //std::cout << origen << std::endl;
    return a.exec();
}
