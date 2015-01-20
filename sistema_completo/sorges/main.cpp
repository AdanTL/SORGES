#include <QApplication>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include "classes/station.h"
#include "classes/origin.h"
#include "classes/dataprocessing.h"
#include "widgets/mapwidget.h"
#include "widgets/origindatawidget.h"
#include "widgets/stationsdatawidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DataProcessing dataProcessor;
    MapWidget mapW;
    OriginDataWidget originDataW;
    StationsDataWidget stationsDataW;

    /**SIGNAL -> SLOT mapping**/

    QObject::connect (&dataProcessor, SIGNAL(stationsLoaded(std::set<Station>)),
                      &stationsDataW, SLOT(showStationsData(std::set<Station>)));

    QObject::connect (&dataProcessor, SIGNAL(stationsLoaded(std::set<Station>)),
                      &mapW, SLOT(paintStations(std::set<Station>)));

    QObject::connect (&dataProcessor, SIGNAL(originReceived(Origin)),
                      &originDataW, SLOT(showOriginData(Origin)));

    QObject::connect (&dataProcessor, SIGNAL(originReceived(Origin)),
                      &mapW, SLOT(paintOrigin(Origin)));

    QObject::connect (&dataProcessor, SIGNAL(stationColorReceived(std::set<Station>)),
                      &stationsDataW, SLOT(changeStationsData(std::set<Station>)));

    QObject::connect (&dataProcessor, SIGNAL(stationColorReceived(std::set<Station>)),
                      &mapW, SLOT(changeStationsColors(std::set<Station>)));

    mapW.show();
    originDataW.show();
    stationsDataW.show();

    dataProcessor.init();

    return a.exec();
}
