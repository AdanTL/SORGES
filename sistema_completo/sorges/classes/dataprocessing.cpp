#include <QSettings>
#include <QDir>
#include "dataprocessing.h"

DataProcessing::DataProcessing(QObject* parent):
    watcher(this),
    config(new QSettings(":/sorges.conf",QSettings::NativeFormat))
    {

    //SETTINGS CONFIGURATION
    //cargar de resources mientras compilamos (ver lista de inicializacion)
    //una vez se entregue el producto compilado, probar esto añadiendolo a lista inicialización
    //QSettings config(QDir::currentPath()+"directorio de config/sorges.conf",QSettings::NativeFormat);

    if (!watcher.addPath(config->value("filepaths/stations").toString()))
        std::cerr << "Problem to find the file: "
                  << config->value("filepaths/stations").toString().toStdString()
                  << std::endl;
    /*
    if (!watcher.addPath(config->value("filepaths/picks").toString()))
        std::cerr << "Problem to find the file: "
                  << config->value("filepaths/picks").toString().toStdString()
                  << std::endl;

    if (!watcher.addPath(config->value("filepaths/origins").toString()))
        std::cerr << "Problem to find the file: "
                  << config->value("filepaths/origins").toString().toStdString()
                  << std::endl;

    if (!watcher.addPath(config.value("filepaths/events").toString()))
        std::cerr << "Problem to find the file: "
                  << config->value("filepaths/events").toString().toStdString()
                  << std::endl;
    */

    connect(&watcher,SIGNAL(fileChanged(QString)),this,SLOT(fileChangedSlot(QString)));

}

void DataProcessing::init(){
    //List of stations loaded at the start of the system with dataProcessor.init()
    stations = ProcessStationsFromFile(config->value("filepaths/stations").toString());
    if (!stations.empty())
        emit stationsLoaded(stations);
}

void DataProcessing::fileChangedSlot(QString path)
  {
    //keep the watch on the file
    watcher.addPath(path);

    stations = ProcessStationsFromFile(path);
    if (!stations.empty())
        emit stationsLoaded(stations);
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
