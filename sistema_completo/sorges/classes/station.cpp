#include <iostream>
#include "station.h"

Station::Station(const std::string& id, const std::string& netId, 
				 long double latitude,long double longitude, int color):
    			 stationID(id), 
    			 networkID(netId),
    			 latitude(latitude),
    			 longitude(longitude),
    			 color(color)
                 {}

/**ON-SITE ALERT
* Color Scale.
* Static attribute for all the station objects
* Map with alert code and its color in hexadecimal form.
*/
/*Initializing function*/
std::map<int, const char*> initOnSiteAlert(){
    std::map<int, const char*> scale;
    //-1 = black
    scale[-1] = "#000000";
    //0 = green
    scale[0] = "#33FF00";
    //1 = yellow
    scale[1] = "#CCFF00";
    //2 = orange
    scale[2] = "#FF9900";
    //3 = red
    scale[3] = "#FF0000";
    return scale;
}
/*Attribute definition*/
std::map<int,const char*> Station::onSiteAlert = initOnSiteAlert();

/*Getter for the color corresponding the code attribute of a single object*/		
const char* Station::getCurrentOnSiteAlert() const{		
    return onSiteAlert[this->color];		
}


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


/**Operators*/
	
bool operator < (const Station& station1, const Station& station2){
    return station1.stationID < station2.stationID;
}

bool operator == (const Station& station1, const Station& station2){		
    return station1.stationID == station2.stationID;		
}

std::ostream& operator << (std::ostream& os, const Station& station){
    os << "\t\t<Station>\n";
    os << "\t\t\t<StationID>" << station.getStationID().c_str()
       << "</StationID>\n";
    os << "\t\t\t<StationNetworkID>" << station.getNetworkID().c_str()
       << "</StationNetworkID>\n";
    os << "\t\t\t<StationLatitude>" << (double)station.getLatitude()
       << "</StationLatitude>\n";
    os << "\t\t\t<StationLongitude>" << (double)station.getLongitude()
       << "</StationLongitude>\n";
    os << "\t\t\t<StationOnSiteAlert>" << station.getColor()
       << "</StationOnSiteAlert>\n";
    os << "\t\t</Station>";
    return os;
}

/**To String method*/
/**uses the output operator format to create a string**/
std::string Station::stationToString() const{
    std::ostringstream convert;

    convert << "Station ID: " << this->stationID ;
    convert << "\nStation Network ID: " << this->networkID << "\nLatitude: ";
    convert << this->latitude << "\nLongitude: " << this->longitude << "\n";
    if (this->color != -1) convert << "On-Site Alert: " << this->color << "\n";

    return convert.str();
}
