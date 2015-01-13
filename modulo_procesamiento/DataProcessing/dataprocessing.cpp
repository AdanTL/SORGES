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
    std::cout << fileContent.toStdString() << std::endl;

}

std::set<Station> DataProcessing::ProcessStationsFromFile(const QString &namefile){

return std::set<Station>();
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
    return QString("0");
}

// 2014/02/17 02:39:54 [info/Autoloc/origin/info] 1   PFVI  PM   1  1.60  54 02:39:30.3   -0.0 A   P      XXL   82.3 5.42 - 1.00 3.61 1.50
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
    QStringList eachStationsFile = stationsString.split("\\n");
    QRegExp xr("\n");

    for (int i = 0; i < eachStationsFile.size(); ++i)
            StationParameters.push_back(QString(eachStationsFile.at(i)).remove(xr).split("\t"));

    return StationParameters;
}
