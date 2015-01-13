#include <QCoreApplication>
#include "dataprocessing.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    QString fileStations = "-4.5771\t38.1673\tEADA\n"
                           "-7.0133\t38.7556\tEBAD\n"
                           "-5.4186\t38.0753\tECAB\n"
                           "-5.3768\t35.8978\tECEU";

    QString OriginStations = "2014/02/17 02:39:54 [info/Autoloc/origin/info] 1   PFVI  PM   1  1.60  54 02:39:30.3   -0.0 A   P      XXL   82.3 5.42 - 1.00 3.61 1.50"
                             "2014/02/17 02:39:54 [info/Autoloc/origin/info] 2   PCVE  PM   1  2.40  52 02:39:41.8    0.0 A   P      XXL   76.6 5.29 - 1.00 3.53 1.50"
                             "2014/02/17 02:39:54 [info/Autoloc/origin/info] 3   PNCL  PM   1  2.45  38 02:39:42.4   -0.0 A   P      XXL   43.6 4.36 - 1.00 2.91 1.50"
                             "2014/02/17 02:39:54 [info/Autoloc/origin/info] 4   PVAQ  PM   1  2.50  60 02:39:43.1   -0.0 A   P      XXL   62.0 4.93 - 1.00 3.29 1.50";


    //std::cout << processing.FindParameterOriginID("2014-02-17 02:39:54.9 [scalertes_origen] Origen asociado: Origin#20140217023954.809531.13965\n").toStdString();
    //std::cout << processing.FindParameterOriginDate("2014/02/17 02:39:54 [info/Autoloc/origin/id] 2014-02-17 02:39:01.8  36.192  -10.443  5.08\n").toStdString();
    //std::cout << processing.FindParameterOriginTime("2014/02/17 02:39:54 [info/Autoloc/origin/id] 2014-02-17 02:39:01.8  36.192  -10.443  5.08\n").toStdString();
    //std::cout << processing.FindParameterOriginLatitude("2014/02/17 02:39:54 [info/Autoloc/origin/id] 2014-02-17 02:39:01.8  36.192  -10.443  5.08\n").toStdString();
    //std::cout << processing.FindParameterOriginLongitude("2014/02/17 02:39:54 [info/Autoloc/origin/id] 2014-02-17 02:39:01.8  36.192  -10.443  5.08\n").toStdString();
    //std::cout << processing.FindParameterOriginMagnitude("2014/02/17 02:39:54 [info/Autoloc/origin/id] 2014-02-17 02:39:01.8  36.192  -10.443  5.08\n").toStdString();


    /*std::vector<QStringList> originStationList = processing.FindParameterOriginStations(OriginStations);
    for(int i=0; i<originStationList.size(); i++){
        for(int j=0; j<originStationList[i].size(); j++)
          std::cout << originStationList[i].at(j).toStdString() << "/";
        std::cout << std::endl;
    }
    */


    /*
    std::vector<QStringList> stationList = processing.FindParameterStations(fileStations);
    for(int i=0; i<stationList.size(); i++)
        for(int j=0; j<stationList[i].size(); j++)
          std::cout << stationList[i].at(j).toStdString();
    */

    std::set<Station> Stations = processing.ProcessStationsFromFile(":/testFiles/station.txt");
    Origin origen = processing.ProcessOriginFromFile(":/testFiles/origin.txt");

    std::cout << "++++++++++++++++++++++++++++ PRUEBA ORIGEN: +++++++++++++++++++++++++++++\n" << std::endl;
    std::cout << origen << std::endl;

    std::cout << "++++++++++++++++++++++++++++ PRUEBA ESTACIONES: +++++++++++++++++++++++++++++\n" << std::endl;
    for(std::set<Station>::iterator it=Stations.begin(); it!=Stations.end(); ++it)
        std::cout <<*it << "\n" ;

    return a.exec();
}
