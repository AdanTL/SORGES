#include <QCoreApplication>
#include "dataprocessing.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;

    //First instrucction.
    processing.ProcessStationsFromFile(":/testFiles/station.txt");

    // Those instruction can be concurrent called.
    processing.ProcessAnyFile(":/testFiles/scalertes_picks.log");
    processing.ProcessAnyFile(":/testFiles/origin.txt");
    processing.ProcessAnyFile(":/testFiles/gfz2014dibx.last.xml");
    processing.getOrigin().setStations(processing.getStations());
    std::cout << processing.getOrigin() << std::endl;
    return a.exec();
}
