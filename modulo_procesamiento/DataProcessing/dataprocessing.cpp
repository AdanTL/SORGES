#include "dataprocessing.h"


DataProcessing::DataProcessing()
{
}


void DataProcessing::ProcessAnyFile(const QString &namefile){
    if(namefile ==  FILE_LOG_ORIGIN)
        ProcessOriginFromFileLog(namefile);
    else
        if(namefile == FILE_LOG_PICKS)
            ProcessColorStationsFromFile(namefile);
        else
            if(namefile == FILE_XML_ORIGIN)
                ProcessOriginFromFileXml(namefile);
}

//CHANGES:
    //set latitude and longitude.
void DataProcessing::ProcessOriginFromFileLog(const QString &namefile){
    //std::cout << "ProcessOriginFromFileLog" << namefile.toStdString() << std::endl;
    QRegExp rxNewOrigen("NUEVO ORIGEN -");
    QString fileContent;
    QFile file(namefile);
    int pos=file.size();
    bool found = false;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << namefile.toStdString() << std::endl;
        return;
    }

    // Get Fragment with the same Date and Time (it contents some rubbish data, but it is not important).
    do{
        pos -= 10;
        file.seek(pos);
        fileContent = file.readAll();
        if(rxNewOrigen.indexIn(fileContent) != -1)
            found = true;
    }while(found == false);

    //Look for parameters into log file.
    origen.setOriginID(FindParameterOriginID(fileContent).toStdString());
    origen.setOriginDate(QDate::fromString(FindParameterOriginDate(fileContent),"yyyy-MM-dd"));
    origen.setOriginTime(QTime::fromString(FindParameterOriginTime(fileContent),"hh:mm:ss.z"));
    //origen.setLatitude(FindParameterOriginLatitude(fileContent).toDouble());
    //origen.setLongitude(FindParameterOriginLongitude(fileContent).toDouble());
    //origen.setMagnitude(FindParameterOriginLongitude(fileContent).toDouble());
    //origen.setStations(FindParameterOriginLongitude(fileContent).toDouble());
}

void DataProcessing::ProcessOriginFromFileXml(const QString &namefile){
// @TODO: QUEDA POR IMPLEMENTAR:
    //std::cout << "ProcessOriginFromFileXml" << namefile.toStdString() << std::endl;

}

// FINAL VERSION (BY THE MOMENT).
void DataProcessing::ProcessStationsFromFile(const QString &namefile){
    stations.clear();

    QString fileContent;
    QFile file(namefile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << namefile.toStdString() << std::endl;
        return;
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
}

// FINAL VERSION (BY THE MOMENT).
void DataProcessing::ProcessColorStationsFromFile(const QString &namefile){
    //std::cout << "ProcessColorStationsFromFile" << namefile.toStdString() << std::endl;
    QRegExp rxLastDateTime("\n\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
    QString fileContent;
    QString lastTime;
    QFile file(namefile);    
    int pos=file.size();
    bool found = false;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << namefile.toStdString() << std::endl;
        return;
    }

    // Get Eficiently the last Date in File.
    do{
        pos -= 100;
        file.seek(pos);
        fileContent = file.readAll();
    }while(rxLastDateTime.lastIndexIn(fileContent) == -1);
    lastTime = rxLastDateTime.cap(0);

    // Get Fragment with the same Date and Time (it contents some rubbish data, but it is not important).
    do{
        pos -= 100;
        file.seek(pos);
        fileContent = file.readAll();
        if(rxLastDateTime.indexIn(fileContent) != -1)
            if(rxLastDateTime.cap() != lastTime)
                found = true;
    }while(found == false);

    // Look for the Station Alert line, and take the values.
    QRegExp rxLineAlert(lastTime +"\tEstaci\\S+ \\S+\tNivel de Alerta:\\d");
    rxLineAlert.lastIndexIn(fileContent);
    if(rxLineAlert.lastIndexIn(fileContent) != -1){
        QRegExp rxStationRm("\n\\d\\d\\d\\d-\\d+-\\d+ \\d+:\\d+:\\d+.\\d\tEstaci\\S+ ");
        QRegExp rxColourRm("Nivel de Alerta:");
        QStringList parameters = rxLineAlert.cap().remove(rxStationRm).remove(rxColourRm).split("\t");
        // Look for a station with the same ID and extract it
        std::set<Station>::iterator it = stations.find(Station(parameters.at(0).toStdString()));
        Station st(*it);
        // Change color and update this value.
        st.setColor(parameters.at(1).toInt());
        stations.erase(it);
        stations.insert(st);
    }
}



QString DataProcessing::FindParameterOriginID(const QString &originString){
     QRegExp rx("Origin#\\S+");
     if(rx.indexIn(originString) != -1)
        return rx.cap(0);
     return QString();
}

QString DataProcessing::FindParameterOriginDate(const QString &originString){
    QRegExp rx("CreationTime= \\d+-\\d+-\\d+");
    QRegExp rx1("CreationTime= ");
    if(rx.indexIn(originString) != -1)
        return rx.cap(0).remove(rx1);
    return QString();
}

QString DataProcessing::FindParameterOriginTime(const QString &originString){
    QRegExp rx("CreationTime= \\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
    QRegExp rx1("CreationTime= \\d+-\\d+-\\d+ ");
    if(rx.indexIn(originString) != -1)
        return rx.cap(0).remove(rx1);
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
    QRegExp rx(":\\d+.\\d  \\d+.\\d+  -?\\d+.\\d+");
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
