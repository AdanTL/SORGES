#include "dataprocessing.h"

DataProcessing::DataProcessing()
{
}


Origin DataProcessing::ProcessOriginFromFile(const QString &namefile){
    QString fileContent;
    QFile file(namefile);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << namefile.toStdString() << std::endl;
        return Origin();
    }

    fileContent = file.readAll();
    file.close();
    std::string originID = FindParameterOriginID(fileContent).toStdString();
    QDate originDate = QDate::fromString(FindParameterOriginDate(fileContent),"yyyy-MM-dd");
    QTime originTime = QTime::fromString(FindParameterOriginTime(fileContent), "hh:mm:ss.z");
    long double originLatitude = FindParameterOriginLatitude(fileContent).toDouble();
    long double originLongitude = FindParameterOriginLongitude(fileContent).toDouble();
    double originMagnitude = FindParameterOriginMagnitude(fileContent).toDouble();
    std::set<Station> originStations;

    std::vector<QStringList> stationsParameters = FindParameterOriginStations(fileContent);
        for(size_t i=0; i<stationsParameters.size(); i++){
            auto  it = stations.find(Station(stationsParameters[i].at(0).toStdString()));
            Station originOneStation = Station(*it);
            originOneStation.setColor((stationsParameters.at(i)).at(2).toInt());
            originStations.insert(originOneStation);
        }

    Origin origen(originID,originDate,originTime,originLatitude,originLongitude,originMagnitude,originStations);
    return origen;
}

std::set<Station> DataProcessing::ProcessStationsFromFile(const QString &namefile){
    stations.clear();

    QString fileContent;
    QFile file(namefile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << namefile.toStdString() << std::endl;
        return stations;
    }
    fileContent = file.readAll();
    file.close();
    std::vector<QStringList> stationsParameters = FindParameterStations(fileContent);

    std::string StationId;
    long double StationLatitude;
    long double StationLongitude;

    for(size_t i=0; i<stationsParameters.size(); i++){
        StationId = stationsParameters.at(i).at(stationsParameters.at(i).size()-1).toStdString();
        std::string StationIdNetwork = "TEST";
                //stationsParameters.at(i).at(//N).toStdString();
        StationLatitude = stationsParameters.at(i).at(1).toDouble();
        StationLongitude = stationsParameters.at(i).at(0).toDouble();
        stations.insert(Station(StationId,StationIdNetwork,StationLatitude,StationLongitude));
    }
    return stations;
}

QString DataProcessing::FindParameterOriginID(const QString &originString){
     QRegExp rx("Origin#\\S+");
     if(rx.indexIn(originString) != -1)
        return rx.cap(0);
     return QString();
}

QString DataProcessing::FindParameterOriginDate(const QString &originString){
    QRegExp rx("\\d+-\\d+-\\d+");
    if(rx.indexIn(originString) != -1)
        return rx.cap(0);
    return QString();
}

QString DataProcessing::FindParameterOriginTime(const QString &originString){
    QRegExp rx("\\d+:\\d+:\\d+.\\d");
    if(rx.indexIn(originString) != -1)
        return rx.cap(0);
    return QString();
}

QString DataProcessing::FindParameterOriginLatitude(const QString &originString){
    QRegExp rx(":\\d+.\\d  \\d+.\\d+");
    QRegExp rx2(":\\d+.\\d  ");
    if(rx.indexIn(originString) != -1)
        return rx.cap(0).remove(rx2);
    return QString();
}

QString DataProcessing::FindParameterOriginLongitude(const QString &originString){
    QRegExp rx(":\\d+.\\d  \\d+.\\d+  -?\\d+.\\d");
    QRegExp rx2(":\\d+.\\d  \\d+.\\d+  ");
    if(rx.indexIn(originString) != -1)
        return rx.cap(0).remove(rx2);
    return QString();
}

QString DataProcessing::FindParameterOriginMagnitude(const QString &originString){
    // @TODO: FALTA POR IMPLEMENTAR A ESPERA DE INFORMACION.
    return QString("0");
}


std::vector<QStringList> DataProcessing::FindParameterOriginStations(const QString &originString){
    std::vector<QStringList> originStationParameters;
    QRegExp rx("/info\\D +\\d+ +\\S+ +\\S+ +\\d");
    QRegExp rx2("/info\\D +\\d+ +");
    QRegExp rx3(" +");
    QStringList originStations;
    int pos=0;

    while ((pos = rx.indexIn(originString, pos)) != -1) {
        originStations << rx.cap(0).remove(rx2).replace(rx3,"-");
        pos += rx.matchedLength();
    }

    for (int i = 0; i < originStations.size(); ++i)
        originStationParameters.push_back(QString(originStations.at(i)).split("-"));

    return originStationParameters;
}


std::vector<QStringList> DataProcessing::FindParameterStations(const QString &stationsString){
    std::vector<QStringList> StationParameters;
    QStringList eachStationsFile = stationsString.split("\n");
    QString oneStation;

    for (int i = 0; i < eachStationsFile.size(); ++i){
        oneStation = QString(eachStationsFile.at(i));
        if(!oneStation.isEmpty())
            StationParameters.push_back(oneStation.split("\t"));
    }
    return StationParameters;
}
