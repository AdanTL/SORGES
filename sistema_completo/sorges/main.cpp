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
#include "widgets/mapwidget.h"
#include "widgets/origindatawidget.h"
#include "widgets/stationsdatawidget.h"

//funciones temporales para simular la llegada de datos
//cuando se definan sockets y extracción desde fichero entonces se separarán a su modulo
void recibirEstaciones(std::set<Station>& estaciones){
        //datos que se sacarán del fichero que llegue, esto es a fuego temporalmente
        estaciones.insert(Station("0x0000", "0x0010", 35.222, -13.332, -1));
        estaciones.insert(Station("0x0001", "0x0011", 35.675, -10.322, -1));
        estaciones.insert(Station("0x0002", "0x0012", 36.232, -7.543, -1));
        estaciones.insert(Station("0x0003", "0x0013", 37.323, -5.887,-1));
}

void recibirOrigen(Origin& origen,const std::set<Station>& estacionesActuales){

    //las estaciones llegarán por código y color
    //se pueden sacar los objetos de la lista de estaciones actuales (buscando por codigo)
    //o se podria guardar en el objeto origen simplemente una estructura que guarde
    //el codigo de la estacion y el color que llega

    //simular llegada de estaciones asociadas al origen y su color:
    //map codigo->color
    //esto podría ser una estructura muy válida como atributo del objeto Origen
    std::map<std::string, int> estacionesCodigoColor;
    estacionesCodigoColor["0x0001"]=1;
    estacionesCodigoColor["0x0002"]=3;

    //si lo dejamos como lo tenemos ahora y para el objeto origen
    //hay que pasar una lista de objetos estacion, pues hay que
    //rellenar este set buscando las estaciones en las actuales
    //por codigo y añadirlas al nuevo set, y cambiarles el color
    std::set<Station> estacionesOrigen;

    for (std::map<std::string, int>::iterator it=estacionesCodigoColor.begin();
                                              it!=estacionesCodigoColor.end();
                                              ++it){
        //descripción de la siguiente lines:
        //insertar en la lista de estaciones del origen
        //el objeto al que apunta el iterador devuelto por find
        //al buscar en las estaciones actuales los codigos que nos
        //han llegado para encontrar el objeto
        //(it->first es el primer elemento del map)
        //y antes se les cambia el color
        //(it->second es el segundo elemento del map)
        Station station = *(estacionesActuales.find(it->first));
        station.setColor(it->second);
        estacionesOrigen.insert(station);
    }

    origen.setOriginID ("Origin#11111111");
    origen.setOriginDate (QDate().currentDate ());
    //+5segundos para que el radio de expansion salga
    origen.setOriginTime (QTime().currentTime ().addSecs(-5));
    origen.setLatitude (36.111);
    origen.setLongitude (-7.213);
    origen.setMagnitude (1.1);
    origen.setStations (estacionesOrigen);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MapWidget mapW;
    OriginDataWidget originDataW;
    StationsDataWidget stationsDataW;
    mapW.show();
    originDataW.show();
    stationsDataW.show();

    //simular llegada de estaciones
    std::set<Station> stations;
    recibirEstaciones(stations);

    //simular llegada de origen
    Origin origin;
    recibirOrigen(origin,stations);

    mapW.paintStations(stations);
    mapW.paintOrigin(origin);
    originDataW.showOriginData (origin);
    stationsDataW.showStationsData (stations);

    return a.exec();
}
