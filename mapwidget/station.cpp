#include <iostream>
#include "station.h"

Station::Station(const std::string& id, const std::string& netId, long double latitude, 
    	         long double longitude, int color):
    			 stationID(id), 
    			 networkID(netId),
    			 latitude(latitude),
    			 longitude(longitude),
    			 color(color)
                 {}

/**funcion inicializadora de atributo estático
 * escala de colores: correspondencias codigo -> color hexadecimal
 *-1 negro, 0 verde, 1 amarillo, 2 naranja, 3 rojo
 */
std::map<int, const char*> initOnSiteAlert(){
    std::map<int, const char*> scale;
    scale[-1] = "#000000";
    scale[0] = "#33FF00";
    scale[1] = "#CCFF00";
    scale[2] = "#FF9900";
    scale[3] = "#FF0000";
    return scale;
}
/**atributo estático (de clase) con la escala de colores*/
std::map<int,const char*> Station::onSiteAlert = initOnSiteAlert();


/**Getters and setters*/
std::string Station::getStationID() const
{
    return stationID;
}
void Station::setStationID(const std::string &value)
{
    stationID = value;
}

std::string Station::getNetworkID() const
{
    return networkID;
}
void Station::setNetworkID(const std::string &value)
{
    networkID = value;
}

long double Station::getLatitude() const
{
    return latitude;
}
void Station::setLatitude(long double value)
{
    latitude = value;
}

long double Station::getLongitude() const
{
    return longitude;
}
void Station::setLongitude(long double value)
{
    longitude = value;
}

int Station::getColor() const
{
    return color;
}

void Station::setColor(int value)
{
    color = value;
}

const char* Station::getCurrentOnSiteAlert(){
    return onSiteAlert[this->color];
}


/**Operators*/
	
bool operator < (const Station& station1, const Station& station2){
    return station1.stationID < station2.stationID;
}

std::ostream& operator << (std::ostream& os, const Station& station){
    os << "Station ID: " << station.stationID ;
    os << "\nStation Network ID: " << station.networkID << "\nLatitude: ";
    os << station.latitude << "\nLongitude: " << station.longitude << "\nCode: " << station.color << "\n";
    return os;
}
