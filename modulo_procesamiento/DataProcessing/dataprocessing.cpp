#include "dataprocessing.h"
#include <QXmlStreamReader>


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
            if(namefile.contains(".xml"))
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
    bool overflow = false;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << namefile.toStdString() << std::endl;
        return;
    }

    // Get Fragment with the same Date and Time (it contents some rubbish data, but it is not important).
    do{
        pos -= 4000;
        //it is not a empty file.
        if(pos < 0){
            overflow = true;
            pos = 0;
        }
        file.seek(pos);
        fileContent = file.readAll();

        if(rxNewOrigen.indexIn(fileContent) != -1)
            found = true;
    }while(found == false && overflow == false);

    // if is overflowed return.
    if(found == false){
        //cerr
        return;
    }
    //Look for parameters into log file.
    origen.setOriginID(FindParameterOriginID(fileContent).toStdString());
    origen.setOriginDate(QDate::fromString(FindParameterOriginDate(fileContent),"yyyy-MM-dd"));
    origen.setOriginTime(QTime::fromString(FindParameterOriginTime(fileContent),"hh:mm:ss.z"));
    origen.setLatitude(FindParameterOriginLatitude(fileContent).toDouble());
    origen.setLongitude(FindParameterOriginLongitude(fileContent).toDouble());
    //origen.setMagnitude(FindParameterOriginLongitude(fileContent).toDouble());
    //origen.setStations(FindParameterOriginLongitude(fileContent).toDouble());
}
//hay que renombrar la funcion xq pilla cosas de las stations
void DataProcessing::ProcessOriginFromFileXml(const QString &namefile){
    std::set<Station> mystations;
    QStringList networkID, stationID;
    QString originID;
    QString eventID;
    QString magnitudeID;
    long double originLatitude, originLongitude;
    double originMagnitude=0.0;
    QString dateTime;
    QDate originDate;
    QTime originTime;
    QRegExp rxDate("\\d+-\\d+-\\d+");
    QRegExp rxTime("\\d+:\\d+:\\d+.\\d\\d?\\d?");
    QDomElement element;

    QDomDocument doc(namefile);
    QFile file(namefile);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();


    // Getting the pick values:
    QDomNodeList picks = doc.elementsByTagName("pick");
    for (int i = 0; i < picks.size(); i++) {
        QDomNode element = picks.item(i);
        QDomElement waveformID = element.firstChildElement("waveformID");
        if(waveformID.hasAttribute("stationCode"))
            stationID << waveformID.attribute("stationCode");
        if(waveformID.hasAttribute("networkCode"))
            networkID << waveformID.attribute("networkCode");
    }

    // Getting the Origin Value:
    QDomNodeList origins = doc.elementsByTagName("origin");
    for (int i = 0; i < origins.size(); i++) {
        QDomNode origin = origins.item(i);
        if(origin.isElement()){
            element = origin.toElement();
            if(element.hasAttribute("publicID"))
            originID = element.attribute("publicID");
        }
        originLatitude = origin.firstChildElement("latitude").firstChildElement("value").text().toDouble();
        originLongitude = origin.firstChildElement("longitude").firstChildElement("value").text().toDouble();
    }

    //Get Event values:
    QDomNodeList events = doc.elementsByTagName("event");
    for (int i = 0; i < events.size(); i++) {
        QDomNode event = events.item(i);
        if(event.isElement()){
            element = event.toElement();
            if(element.hasAttribute("publicID")){
                eventID = element.attribute("publicID");
            }
            dateTime = event.firstChildElement("creationInfo").firstChildElement("creationTime").text();
            if(rxDate.indexIn(dateTime) != -1)
                originDate = QDate::fromString(rxDate.cap(0),"yyyy-MM-dd");
            if(rxTime.indexIn(dateTime) != -1)
                originTime = QTime::fromString(rxTime.cap(0),"hh:mm:ss.zzz");
            magnitudeID = event.firstChildElement("preferredMagnitudeID").text();
        }
    }

    // Getting the magnitudes values
    QDomNodeList magnitudes = doc.elementsByTagName("magnitude");
    for (int i = 0; i < magnitudes.size(); i++) {
        QDomNode magnitude = magnitudes.item(i);
        if(magnitude.hasAttributes() && magnitude.isElement()){
            element = magnitude.toElement();
            if(element.hasAttribute("publicID"))
                if(element.attribute("publicID") == magnitudeID)
                    if(!magnitude.firstChildElement("magnitude").hasAttributes())
                        originMagnitude = magnitude.firstChildElement("magnitude").firstChildElement("value").text().toDouble();
        }
    }

    // PRINTING AND SETTING ORIGIN AND STATIONS.

        origen.setOriginID(originID.toStdString());
        origen.setLatitude(originLatitude);
        origen.setLongitude(originLongitude);
        origen.setOriginDate(originDate);
        origen.setOriginTime(originTime);
        for(int i=0; i<stationID.size(); i++){
            std::set<Station>::iterator it = stations.find(Station(stationID.at(i).toStdString()));
            if(it != stations.end()){
                Station st(*it);
                st.setNetworkID(networkID.at(i).toStdString());
                stations.erase(it);
                stations.insert(st);
                mystations.insert(st);
            }
        }
        origen.setStations(mystations);
        origen.setMagnitude(originMagnitude);

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
    bool overflowed = false;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << namefile.toStdString() << std::endl;
        return;
    }

    // Get Eficiently the last Date in File.
    do{
        pos -= 100;
        //File empty.
        if(pos < 0){
            pos = 0;
            overflowed = true;
        }

        file.seek(pos);
        fileContent = file.readAll();
    }while(rxLastDateTime.lastIndexIn(fileContent) == -1 && overflowed == false);

    // if it is not overflowed.
    if(rxLastDateTime.lastIndexIn(fileContent) != -1)
        lastTime = rxLastDateTime.cap(0);
    else{
        //cerr
        return;
    }
    found = false;
    overflowed = false;

    // Get Fragment with the same Date and Time (it contents some rubbish data, but it is not important).
    do{
        pos -= 100;
        if(pos < 0){
            overflowed = true;
            pos = 0;
        }
        file.seek(pos);
        fileContent = file.readAll();
        if(rxLastDateTime.indexIn(fileContent) != -1)
            if(rxLastDateTime.cap() != lastTime)
                found = true;
    }while(found == false && overflowed == false);

        // Look for the Station Alert line, and take the values.
        QRegExp rxLineAlert(lastTime +"\tEstaci\\S+ \\S+\tNivel de Alerta:\\d");
        rxLineAlert.lastIndexIn(fileContent);
        if(rxLineAlert.lastIndexIn(fileContent) != -1){
            QRegExp rxStationRm("\n\\d\\d\\d\\d-\\d+-\\d+ \\d+:\\d+:\\d+.\\d\tEstaci\\S+ ");
            QRegExp rxColourRm("Nivel de Alerta:");
            QStringList parameters = rxLineAlert.cap().remove(rxStationRm).remove(rxColourRm).split("\t");
            // Look for a station with the same ID and extract it
            std::set<Station>::iterator it = stations.find(Station(parameters.at(0).toStdString()));
            if(it != stations.end()){
                Station st(*it);
                // Change color and update this value.
                st.setColor(parameters.at(1).toInt());
                stations.erase(it);
                stations.insert(st);
            }
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
    QRegExp rx("origen->latitud = \\S+[(]");
    QRegExp rx2("origen->latitud = ");
    if(rx.lastIndexIn(originString) != -1){
        return rx.cap(0).remove(rx2).remove("(");
    }
    return QString();
}

QString DataProcessing::FindParameterOriginLongitude(const QString &originString){
    QRegExp rx("origen->longitug = \\S+[(]");
    QRegExp rx2("origen->longitug = ");
    if(rx.lastIndexIn(originString) != -1)
        return rx.cap(0).remove(rx2).remove("(");
    return QString();
}

// There is not any REGEXP to find the magnitude:

//QString DataProcessing::FindParameterOriginMagnitude(const QString &originString){
//    // @TODO: FALTA POR IMPLEMENTAR A ESPERA DE INFORMACION.
//    return QString("0");
//}


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

