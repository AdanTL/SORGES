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




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// SIMULATION METHODS
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void DataProcessing::initSimulation(QDateTime simulationDateTime){

    /*First, search for the closest event to the required DateTime*/
    /*Path from /home/$USERNAME/.seiscomp3/log/events*/

    QDir eventsDir(QDir::homePath()+"/.seiscomp3/log/events");

    if (! eventsDir.exists ()){
        std::cerr<<"NOT FOUND directory path ~/.seiscomp3/log/events"<<std::endl;
        std::cerr<<"EVENT FILES CANNOT BE FOUND"<<std::endl;
        exit(-1);
    }
    else{

        QString year = QString::number(simulationDateTime.date().year());

        QString month = QString::number(simulationDateTime.date().month());
        if (simulationDateTime.date().month()<10) month = "0"+month;

        QString day = QString::number(simulationDateTime.date().day());
        if (simulationDateTime.date().day()<10) day = "0"+day;

        /*Events organized in hierarchy yyyy/MM/dd/eventName */

        QString requiredEvent("");
        QDateTime requiredEventDateTime;
        double timeDiff = std::numeric_limits<double>::max();

        QDir eventsForDateTime(eventsDir.absolutePath ()+"/"+year+"/"+month+"/"+day);
        if (eventsForDateTime.exists()){
std::cout<<"la misma fecha existe"<<std::endl;
            /*Searching all the events in the same day month and year*/
            foreach(QString eventName,eventsForDateTime.entryList()){
                //avoiding unix directories elements . and ..
                if (eventName.toStdString() != "."
                                                && eventName.toStdString() != "..")
                {
                    QDir eventFiles(eventsForDateTime.absolutePath()+"/"+eventName);
                    QDateTime eventDateTime = getDateTimeFromEvent(eventFiles,eventName);

                    double range = abs(eventDateTime.msecsTo(simulationDateTime));
std::cout<<"rangos comparando fecha pedida: "<<range<<std::endl;
                    if (range<timeDiff){
std::cout<<"y este es menor que la diff: "<<timeDiff<<std::endl;
                        timeDiff = range;
                        requiredEvent = eventName;
std::cout<<"el evento es: "<<eventName.toStdString ()<<std::endl;
                        requiredEventDateTime  = eventDateTime;
                    }
                }
            }
        }

        else {
            std::cout<<"No events stored for the requested date"<<std::endl;
            std::cout<<"Searching previous and next day"<<std::endl;
        }

        /*In case of less difference with the nex ot previous days events*/
        QDateTime prevDayDateTime;
        prevDayDateTime.setDate(simulationDateTime.date().addDays(-1));
        prevDayDateTime.setTime(QTime(23,59,59));
        QDateTime nextDayDateTime;
        nextDayDateTime.setDate(simulationDateTime.date().addDays(1));
        nextDayDateTime.setTime(QTime(0,0,0));

        double timeDiffPrevDay = abs(simulationDateTime.msecsTo(prevDayDateTime));
        double timeDiffNextDay = abs(simulationDateTime.msecsTo(nextDayDateTime));

        if (timeDiffPrevDay < timeDiffNextDay){
std::cout<<"a buscar en el dia anterior:"<<std::endl;
            /*Searching in the prev day*/
            QString prevDayYear = QString::number(prevDayDateTime.date().year());

            QString prevDayMonth = QString::number(prevDayDateTime.date().month());
            if (prevDayDateTime.date().month()<10) prevDayMonth = "0"+prevDayMonth;

            QString prevDay = QString::number(prevDayDateTime.date().day());
            if (prevDayDateTime.date().day()<10) prevDay = "0"+prevDay;

            QDir eventsForPrevDayDateTime(eventsDir.absolutePath()
                                          +"/"+prevDayYear+"/"+prevDayMonth+"/"+prevDay);
            if (eventsForPrevDayDateTime.exists()){
std::cout<<"hay directorio del dia anterior:"<<std::endl;
                /*Searching all the events in the same day month and year*/
                foreach(QString eventName,eventsForPrevDayDateTime.entryList()){
                    //avoiding unix directories elements . and ..
                    if (eventName.toStdString() != "."
                                                    && eventName.toStdString() != "..")
                    {
                        QDir eventFiles(eventsForPrevDayDateTime.absolutePath()+"/"+eventName);
                        QDateTime eventDateTime = getDateTimeFromEvent(eventFiles,eventName);

                        double range = abs(eventDateTime.msecsTo(simulationDateTime));
std::cout<<"rangos comparando fecha pedida: "<<range<<std::endl;
                        if (range<timeDiff){
                            timeDiff = range;
std::cout<<"y este es menor que la diff: "<<timeDiff<<std::endl;
                            requiredEvent = eventName;
std::cout<<"el evento es: "<<eventName.toStdString ()<<std::endl;
                            requiredEventDateTime  = eventDateTime;
                        }
                    }
                }
            }
        }
        else {
std::cout<<"a buscar en el dia siguiente:"<<std::endl;
            /*Searching in the next day*/
            QString nextDayYear = QString::number(nextDayDateTime.date().year());

            QString nextDayMonth = QString::number(nextDayDateTime.date().month());
            if (nextDayDateTime.date().month()<10) nextDayMonth = "0"+nextDayMonth;

            QString nextDay = QString::number(nextDayDateTime.date().day());
            if (nextDayDateTime.date().day()<10) nextDay = "0"+nextDay;

            QDir eventsForNextDayDateTime(eventsDir.absolutePath()
                                          +"/"+nextDayYear+"/"+nextDayMonth+"/"+nextDay);
            if (eventsForNextDayDateTime.exists()){
std::cout<<"hay directorio del dia siguiente:"<<std::endl;
                /*Searching all the events in the same day month and year*/
                foreach(QString eventName,eventsForNextDayDateTime.entryList()){
                    //avoiding unix directories elements . and ..
                    if (eventName.toStdString() != "."
                                                    && eventName.toStdString() != "..")
                    {
                        QDir eventFiles(eventsForNextDayDateTime.absolutePath()+"/"+eventName);
                        QDateTime eventDateTime = getDateTimeFromEvent(eventFiles,eventName);

                        double range = abs(eventDateTime.msecsTo(simulationDateTime));
std::cout<<"rangos comparando fecha pedida: "<<range<<std::endl;
                        if (range<timeDiff){
std::cout<<"y este es menor que la diff: "<<timeDiff<<std::endl;
                            timeDiff = range;
                            requiredEvent = eventName;
std::cout<<"el evento es: "<<eventName.toStdString ()<<std::endl;
                            requiredEventDateTime  = eventDateTime;
                        }
                    }
                }
            }
        }

        /*Event found on required, previous or next day or not found in them*/
        if (requiredEvent == ""){
            std::cerr<<"NO EVENT FOUND CLOSE TO THE REQUESTED TIMESTAMP OR NEXT/PREVIOUS DAY"<<std::endl;
            exit(-1);
        }
        else{
            std::cout<<"Starting simulation of EVENT "
                       +requiredEvent.toStdString()<<std::endl;
            std::cout<<"Timestamp of the event: "
                       +requiredEventDateTime.toString("yyyy-MM-dd hh:mm:ss.z")
                       .toStdString ()<<std::endl;
        }

        QString requiredYear = QString::number(requiredEventDateTime.date().year());

        QString requiredMonth = QString::number(requiredEventDateTime.date().month());
        if (requiredEventDateTime.date().month()<10) requiredMonth = "0"+requiredMonth;

        QString requiredDay = QString::number(requiredEventDateTime.date().day());
        if (requiredEventDateTime.date().day()<10) requiredDay = "0"+requiredDay;

        QDir requiredEventDir(eventsDir.absolutePath ()
                              +"/"+requiredYear
                              +"/"+requiredMonth
                              +"/"+requiredDay
                              +"/"+requiredEvent);

        QDateTime firstDateTime = getDateTimeFirstPick(requiredEventDir,requiredEvent);
        QDateTime lastDateTime = getLastDateTimeFromEvent(requiredEventDir,requiredEvent);
        //in order to avoid losing data due to miliseconds accuracy in logs end lines
        lastDateTime.addMSecs(500);

        std::cout<<"First pick of the simulation at "
                   +firstDateTime.toString("yyyy-MM-dd hh:mm:ss.z")
                   .toStdString()<<std::endl;

        std::cout<<"End of gathering simulation data at "
                   +lastDateTime.toString("yyyy-MM-dd hh:mm:ss.z")
                   .toStdString()<<std::endl;

        /*requiredEvent sería el nombre del evento que hay que coger
         *requiredEventDir el directorio donde va a estar su xml
         *firstDateTime la fecha y hora del primer pick para leer los 2 logs
         *lastDateTime la fecha y hora ULTIMA de la que habría que leer logs
         * (hay que añadir unos milisegundos para asegurarnos de no perder datos)
         */
        /*AHORA HAY QUE VER CÓMO MANDAMOS ESTO A VUESTRAS FUNCIONES PARA ENLAZARLO*/

    }
}

QDateTime DataProcessing::getDateTimeFromEvent(QDir eventFiles,QString eventName){

    QString namefile = eventFiles.absoluteFilePath(eventName+".last.xml");
    QDomDocument xml(namefile);
    QFile file(namefile);
    if (!file.open(QIODevice::ReadOnly)){
        std::cerr << "Problem to find the file: "+eventName.toStdString()+".last.xml"<<std::endl;
        std::cerr << "No event information can be reached, simulation aborted"<<std::endl;
        exit(-1);
    }
    if (!xml.setContent(&file)) {
        std::cerr << "Problem to read the content: "+eventName.toStdString()+".last.xml"<<std::endl;
        std::cerr << "No event information can be reached, simulation aborted"<<std::endl;
        file.close();
        exit(-1);
    }
    file.close();

    QDomNodeList eventTagList = xml.elementsByTagName("event");
    QDomNode eventTag = eventTagList.item(0);
    QString originID = eventTag.firstChildElement("preferredOriginID").text();

    QString eventTimeString = originID.remove("Origin#");
    QStringList components = eventTimeString.split(".");
    QDateTime eventTime = QDateTime::fromString(components.at(0),"yyyyMMddhhmmss");
    eventTime.addMSecs(components.at(1).at(0).digitValue());

    return eventTime;
}

QDateTime DataProcessing::getDateTimeFirstPick(QDir eventFiles,QString eventName){

    QString namefile = eventFiles.absoluteFilePath(eventName+".last.xml");
    QDomDocument xml(namefile);
    QFile file(namefile);
    if (!file.open(QIODevice::ReadOnly)){
        std::cerr << "Problem to find the file: "+eventName.toStdString()+".last.xml"<<std::endl;
        std::cerr << "No event information can be reached, simulation aborted"<<std::endl;
        exit(-1);
    }
    if (!xml.setContent(&file)) {
        std::cerr << "Problem to read the content: "+eventName.toStdString()+".last.xml"<<std::endl;
        std::cerr << "No event information can be reached, simulation aborted"<<std::endl;
        file.close();
        exit(-1);
    }
    file.close();

    QDomNodeList eventParameters = xml.elementsByTagName("EventParameters");
    QDomNode eventParameterTag = eventParameters.item(0);
    QString pickFirstTimeString = eventParameterTag.firstChildElement("pick")
                                           .firstChildElement("time")
                                           .firstChildElement("value").text();
    pickFirstTimeString.replace(QChar('T'),QChar(' '));
    pickFirstTimeString.remove(QChar('Z'));
    QStringList components = pickFirstTimeString.split(".");
    QDateTime pickFirstTime = QDateTime::fromString(components.at(0),
                                                    "yyyy-MM-dd hh:mm:ss");
    pickFirstTime.addMSecs(components.at(1).at(0).digitValue());

    return pickFirstTime;
}

QDateTime DataProcessing::getLastDateTimeFromEvent(QDir eventFiles,QString eventName){

    QString namefile = eventFiles.absoluteFilePath(eventName+".last.xml");
    QDomDocument xml(namefile);
    QFile file(namefile);
    if (!file.open(QIODevice::ReadOnly)){
        std::cerr << "Problem to find the file: "+eventName.toStdString()+".last.xml"<<std::endl;
        std::cerr << "No event information can be reached, simulation aborted"<<std::endl;
        exit(-1);
    }
    if (!xml.setContent(&file)) {
        std::cerr << "Problem to read the content: "+eventName.toStdString()+".last.xml"<<std::endl;
        std::cerr << "No event information can be reached, simulation aborted"<<std::endl;
        file.close();
        exit(-1);
    }
    file.close();

    QDomNodeList eventTagList = xml.elementsByTagName("event");
    QDomNode eventTag = eventTagList.item(0);
    QString modificationTimeString = eventTag.firstChildElement("creationInfo")
                                       .firstChildElement("modificationTime").text();

    modificationTimeString.replace(QChar('T'),QChar(' '));
    modificationTimeString.remove(QChar('Z'));
    QStringList components = modificationTimeString.split(".");
    QDateTime modificationTime = QDateTime::fromString(components.at(0),
                                                    "yyyy-MM-dd hh:mm:ss");
    modificationTime.addMSecs(components.at(1).at(0).digitValue());

    return modificationTime;
}


/*
QString DataProcessing::getBlockPick(const QDateTime& firstdatetime, const QDateTime& lastdatetime){
    int posBegin, posEnd;
    QString blockPick;
    posBegin = getPositionBegin(firstdatetime,":/testFiles/scalertes_picks.log");
    posEnd = getPositionEnd(lastdatetime, ":/testFiles/scalertes_picks.log");
    QFile file(":/testFiles/scalertes_picks.log");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << std::endl;

    }
    if(posBegin < posEnd){
        file.seek(posBegin);
        blockPick = file.read(posEnd - posBegin);
    }
    return blockPick;

}

QString DataProcessing::getBlockOrigin(const QDateTime& firstdatetime, const QDateTime& lastdatetime){
    int posBegin, posEnd;
    QString blockOrigin;
    posBegin = getPositionBegin(firstdatetime,":/testFiles/scalertes_origenes.log");
    posEnd = getPositionEnd(lastdatetime, ":/testFiles/scalertes_origenes.log");
    QFile file(":/testFiles/scalertes_origenes.log");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << std::endl;

    }
    if(posBegin < posEnd){
        file.seek(posBegin);
        blockOrigin = file.read(posEnd - posBegin);
    }
    return blockOrigin;

}



int DataProcessing::getPositionBegin(const QDateTime& firstdatetime, const QString& namefile){
    QRegExp rxDateBlock("\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
    QString fileContent;
    QFile file(namefile);
    bool found = false;
    int pos = 0;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return -1;
    }

    while(!file.atEnd() && !found){
        pos = file.pos();
        fileContent = file.readLine();
        if(rxDateBlock.indexIn(fileContent) != -1 ){
            if(QDateTime::fromString(rxDateBlock.cap(0).remove("\n"),"yyyy-MM-dd hh:mm:ss.z")
            >= firstdatetime)
                found = true;
        }
    }
    return pos;

}



int DataProcessing::getPositionEnd(const QDateTime &lastdatetime, const QString& namefile){
    QRegExp rxDateBlock("\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
    QString fileContent;
    QFile file(namefile);
    bool found = false;
    int pos = 0;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return -1;
    }
    while(!file.atEnd() && !found){
        pos = file.pos();
        fileContent = file.readLine();
        if(rxDateBlock.indexIn(fileContent) != -1 ){
            if(QDateTime::fromString(rxDateBlock.cap(0),"yyyy-MM-dd hh:mm:ss.z")
            > lastdatetime){
                file.seek(pos);
                std::cout << QString(file.readLine()).toStdString() << std::endl;
                found = true;
            }
        }
    }
    return pos;

}*/
