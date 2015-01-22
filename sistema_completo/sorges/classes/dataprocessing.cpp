#include <QSettings>
#include <QDir>
#include <QXmlStreamReader>
#include "dataprocessing.h"

DataProcessing::DataProcessing(QObject* parent):
    QObject(parent),
    watcher(this),
    config(new QSettings(":/sorges.conf",QSettings::NativeFormat)),
    lastDateTime(QDateTime::currentDateTime())
    {
    //SETTINGS CONFIGURATION
    //cargar de resources mientras compilamos (ver lista de inicializacion)
    //new QSettings(":/sorges.conf",QSettings::NativeFormat)
    //una vez se entregue el producto compilado, probar esto añadiendolo a lista inicialización
    //new QSettings(QDir::currentPath().toStdString()+"/config/sorges.conf",QSettings::NativeFormat)

    if (!watcher.addPath(config->value("filepaths/stations").toString()))
        std::cerr << "Problem to find the file: "
                  << config->value("filepaths/stations").toString().toStdString()
                  << std::endl;

    if (!watcher.addPath(config->value("filepaths/picks").toString()))
        std::cerr << "Problem to find the file: "
                  << config->value("filepaths/picks").toString().toStdString()
                  << std::endl;

    if (!watcher.addPath(config->value("filepaths/origins").toString()))
        std::cerr << "Problem to find the file: "
                  << config->value("filepaths/origins").toString().toStdString()
                  << std::endl;

    if (!watcher.addPath(config->value("filepaths/events").toString()))
        std::cerr << "Problem to find the file: "
                  << config->value("filepaths/events").toString().toStdString()
                  << std::endl;

    connect(&watcher,SIGNAL(fileChanged(QString)),this,SLOT(fileChangedSlot(QString)));

}

void DataProcessing::init(){
    //List of stations loaded at the start of the system with dataProcessor.init()
    processStationsFromFile(config->value("filepaths/stations").toString());
    if (!this->stations.empty())
        emit stationsLoaded(this->stations);
}

void DataProcessing::fileChangedSlot(QString path)
  {
    //keep the watch on the file
    watcher.addPath(path);

    if (path == config->value(("filepaths/stations"))) {
        processStationsFromFile(path);
        if (!this->stations.empty())
            emit stationsLoaded(this->stations);
    }

    else if (path == config->value(("filepaths/picks"))){
        std::set<Station> changedStation = processColorStationsFromFile(path);
        if (!changedStation.empty()){
            emit stationColorReceived(changedStation);
            dumpStationXml();
        }
    }

    else if (path == config->value(("filepaths/origins"))){
        processOriginFromFileLog(path);
        if(this->origin.getOriginID().length() > 0){
            emit originReceived(this->origin);
            dumpOriginXml();
        }
    }

    else if (path == config->value(("filepaths/events"))){
        processOriginFromFileXml(path);
        if(this->origin.getOriginID().length() > 0){
            emit eventReceived(this->origin);
            dumpOriginXml();
        }
    }

    else std::cerr<<"Unrecognized file: "<<path.toStdString()<<std::endl;

  }

void DataProcessing::processStationsFromFile(const QString &namefile){
    stations.clear();

    QString fileContent;
    QFile file(namefile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: "
                  << namefile.toStdString() << std::endl;
    }
    fileContent = file.readAll();
    file.close();
    std::vector<QStringList> stationsParameters =
                                             findParameterStations(fileContent);

    std::string StationId;
    long double StationLatitude;
    long double StationLongitude;

    for(size_t i=0; i<stationsParameters.size(); i++){
        StationId = stationsParameters.at(i).at(stationsParameters.at(i)
                                                .size()-1).toStdString();
        std::string StationIdNetwork = "";
                //stationsParameters.at(i).at(//N).toStdString();
        StationLatitude = stationsParameters.at(i).at(1).toDouble();
        StationLongitude = stationsParameters.at(i).at(0).toDouble();
        stations.insert(Station(StationId,StationIdNetwork,
                                StationLatitude,StationLongitude));
    }
}

void DataProcessing::processOriginFromFileLog(const QString &namefile){
    QRegExp rxNewOrigen("NUEVO ORIGEN -");
    QString fileContent;
    QFile file(namefile);
    int pos=file.size();
    bool found = false;
    bool overflow = false;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: "
                  << namefile.toStdString() << std::endl;
    }
    else{
        // Get Fragment with the same Date and Time (it contents some rubbish data).
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
            std::cerr<<"NO ORIGIN FOUND AT "
                       +QDateTime::currentDateTime().toString().toStdString()
                     <<std::endl;
        }
        else {
            QDateTime dateTimeOrigin = QDateTime::fromString(findParameterOriginDate(fileContent)+" "
                             +findParameterOriginTime(fileContent),"yyyy-MM-dd hh:mm:ss.z");
            if(dateTimeOrigin >= lastDateTime){
                //Look for parameters into log file.
                origin.setOriginID(findParameterOriginID(fileContent).toStdString());
                origin.setOriginDate(QDate::fromString(
                                     findParameterOriginDate(fileContent),"yyyy-MM-dd"));
                origin.setOriginTime(QTime::fromString(
                                     findParameterOriginTime(fileContent),"hh:mm:ss.z"));
                origin.setLatitude(findParameterOriginLatitude(fileContent).toDouble());
                origin.setLongitude(findParameterOriginLongitude(fileContent).toDouble());
            }
        }
    }

}

std::set<Station> DataProcessing::processColorStationsFromFile(const QString &namefile){
    std::set<Station> stationChanged;
    QRegExp rxLastDateTime("\n\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
    QString fileContent;
    QString lastTime;
    QFile file(namefile);
    int pos=file.size();
    bool found = false;
    bool overflowed = false;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: "
                  << namefile.toStdString() << std::endl;
    }

    else{
        // Get Eficiently the last DateTime in File.
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

        // if it is overflowed.
        if(rxLastDateTime.lastIndexIn(fileContent) == -1){
            std::cerr<<"NO PICK FOUND AT "
                       +QDateTime::currentDateTime().toString().toStdString()
                    <<std::endl;
        }
        else {
            lastTime = rxLastDateTime.cap(0);
            //avoid getting data from the past before app runs
            if (QDateTime::fromString
                    (lastTime,"\nyyyy-MM-dd hh:mm:ss.z") >= this->lastDateTime){

                found = false;
                overflowed = false;

                // Get Fragment with the same Date and Time (it contains some rubbish data).
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

                if(found){
                    // Look for the Station Alert line, and take the values.
                    QRegExp rxLineAlert(lastTime
                                        +"\tEstaci\\S+ \\S+\tNivel de Alerta:\\d");
                    rxLineAlert.lastIndexIn(fileContent);
                    if(rxLineAlert.lastIndexIn(fileContent) != -1){
                        QRegExp rxStationRm("\n\\d\\d\\d\\d-\\d+-\\d+ \\d+:\\d+:\\d+.\\d\tEstaci\\S+ ");
                        QRegExp rxColourRm("Nivel de Alerta:");
                        QStringList parameters = rxLineAlert.cap()
                                                 .remove(rxStationRm)
                                                 .remove(rxColourRm).split("\t");
                        // Look for a station with the same ID and extract it
                        std::set<Station>::iterator it = stations
                                                        .find(Station(parameters.at(0)
                                                                      .toStdString()));
                        if(it != stations.end()){
                            Station st(*it);
                            // Change color and update this value.
                            st.setColor(parameters.at(1).toInt());
                            stations.erase(it);
                            stations.insert(st);

                            stationChanged.insert(st);
                        }
                    }
                }
            }
        }
    }
    return stationChanged;
}

void DataProcessing::processOriginFromFileXml(const QString &namefile){
    std::set<Station> mystations;
    QStringList networkID, stationID;
    QString originID;
    QString eventID;
    QString magnitudeID;
    long double originLatitude=0, originLongitude=0;
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
            QDomElement element = origin.toElement();
            if(element.hasAttribute("publicID"))
            originID = element.attribute("publicID");
        }
        originLatitude = origin.firstChildElement("latitude")
                                   .firstChildElement("value").text().toDouble();
        originLongitude = origin.firstChildElement("longitude")
                                   .firstChildElement("value").text().toDouble();
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
            dateTime = event.firstChildElement("creationInfo")
                                        .firstChildElement("creationTime").text();
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
                        originMagnitude = magnitude.firstChildElement("magnitude")
                                          .firstChildElement("value").text().toDouble();
        }
    }

    // PRINTING AND SETTING ORIGIN AND STATIONS.

        origin.setOriginID(eventID.toStdString()+"\t"+originID.toStdString());
        origin.setLatitude(originLatitude);
        origin.setLongitude(originLongitude);
        origin.setOriginDate(originDate);
        origin.setOriginTime(originTime);
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
        origin.setStations(mystations);
        origin.setMagnitude(originMagnitude);

}


std::vector<QStringList> DataProcessing::findParameterStations(const QString &stationsString){
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

QString DataProcessing::findParameterOriginID(const QString &originString){
     QRegExp rx("Origin#\\S+");
     if(rx.indexIn(originString) != -1)
        return rx.cap(0);
     return QString();
}

QString DataProcessing::findParameterOriginDate(const QString &originString){
    QRegExp rx("CreationTime= \\d+-\\d+-\\d+");
    QRegExp rx1("CreationTime= ");
    if(rx.indexIn(originString) != -1)
        return rx.cap(0).remove(rx1);
    return QString();
}

QString DataProcessing::findParameterOriginTime(const QString &originString){
    QRegExp rx("CreationTime= \\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
    QRegExp rx1("CreationTime= \\d+-\\d+-\\d+ ");
    if(rx.indexIn(originString) != -1)
        return rx.cap(0).remove(rx1);
    return QString();
}

QString DataProcessing::findParameterOriginLatitude(const QString &originString){
    QRegExp rx("origen->latitud = \\S+[(]");
    QRegExp rx2("origen->latitud = ");
    if(rx.lastIndexIn(originString) != -1){
        return rx.cap(0).remove(rx2).remove("(");
    }
    return QString();
}

QString DataProcessing::findParameterOriginLongitude(const QString &originString){
    QRegExp rx("origen->longitug = \\S+[(]");
    QRegExp rx2("origen->longitug = ");
    if(rx.lastIndexIn(originString) != -1)
        return rx.cap(0).remove(rx2).remove("(");
    return QString();
}


void DataProcessing::dumpOriginXml(){

    QDir current(QDir::currentPath());
    current.mkdir("backup");

    QFile file(
          QString::fromStdString(QDir::currentPath().toStdString ()
                                 +"/backup/"+origin.getOriginID()+".xml"));
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        std::cerr << "Problem to create origin backup file: " <<  "xml" << std::endl;
        return;
    }

    QTextStream out(&file);
    out << QString::fromStdString("<?xml version=\"1.0\" encoding=\"UTF-8\"?> \n"+origin.toStringXml());
    file.close();

}

void DataProcessing::dumpStationXml(){

    QDir current(QDir::currentPath());
    current.mkdir("backup");

    QFile file(
          QString::fromStdString(QDir::currentPath().toStdString()
                                 +"/backup/Stations#"+QDateTime::currentDateTime()
                                 .toString("yyyyMMddhhmmss.zzz")
                                 .toStdString()+".xml"));
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        std::cerr << "Problem to create station backup file: " <<  "xml" << std::endl;
        return;
    }

    QTextStream out(&file);
    out << QString::fromStdString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Stations>\n"
                                  +Station::stationsToStringXml(stations,"\t"))+"</Stations>";
    file.close();

}
