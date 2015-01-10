#include<iostream>
#include "origin.h"

Origin::Origin(const std::string& id, const QDate& date, const QTime& time, 
			   long double latitude,long double longitude, 
			   double magnitude, const std::set<Station>& stations):
               originID(id),
               originDate(date),
               originTime(time),
   			   latitude(latitude),
   			   longitude(longitude),
   			   magnitude(magnitude),
   			   stations(stations)
               {}


/**GETTERS AND SETTERS**/

std::string Origin::getOriginID() const
{
    return originID;
}
void Origin::setOriginID(const std::string &value)
{
    originID = value;
}

std::set<Station> Origin::getStations() const
{
    return stations;
}
void Origin::setStations(const std::set<Station> &value)
{
    stations = value;
}

double Origin::getMagnitude() const
{
    return magnitude;
}
void Origin::setMagnitude(double value)
{
    magnitude = value;
}

long double Origin::getLongitude() const
{
    return longitude;
}
void Origin::setLongitude(long double value)
{
    longitude = value;
}

long double Origin::getLatitude() const
{
    return latitude;
}
void Origin::setLatitude(long double value)
{
    latitude = value;
}

QTime Origin::getOriginTime() const
{
    return originTime;
}
void Origin::setOriginTime(const QTime &value)
{
    originTime = value;
}

QDate Origin::getOriginDate() const
{
    return originDate;
}
void Origin::setOriginDate(const QDate &value)
{
    originDate = value;
}


/**OPERATORS**/

bool operator < (const Origin& origin1, const Origin& origin2){
    //puede ser interesante ordenar origenes por fecha o por ID
    return origin1.originID < origin2.originID;
    //return origin1.originTime < origin2.originTime;
}

bool operator == (const Origin& origin1, const Origin& origin2){		
	return origin1.originID == origin2.originID;		
}

std::ostream& operator << (std::ostream& os, const Origin& origin){
    os << "Timestamp: " << origin.originDate.year() << "-"
       << origin.originDate.month() << "-" << origin.originDate.day();
    os << " " << origin.originTime.hour() << ":" << origin.originTime.minute()
       << ":" << origin.originTime.second();
    os << "\nMagnitude: " << origin.magnitude << "\nLatitude: ";
    os << origin.latitude << "\nLongitude: " << origin.longitude << "\n";
    for(std::set<Station>::iterator it=origin.stations.begin(); 
    								it!=origin.stations.end(); ++it)
        os << *it;
    return os;
}
