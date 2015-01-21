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
    os << "<Origin>\n";
    os << "\t<OriginID>" << origin.getOriginID().c_str() << "</OriginID>\n";
    os << "\t<OriginDate>"
       << origin.getOriginDate().toString("yyyy-MM-dd").toStdString().c_str()
       << "</OriginDate>\n";
    os << "\t<OriginTime>"
       << origin.getOriginTime().toString("hh:mm:ss.z").toStdString().c_str()
       << "</OriginTime>\n";
    os << "\t<OriginMagnitude>" << origin.getMagnitude()
       << "</OriginMagnitude>\n";
    os << "\t<OriginLatitude>" << (double)origin.getLatitude()
       << "</OriginLatitude>\n";
    os << "\t<OriginLongitude>" << (double)origin.getLongitude()
       << "</OriginLongitude>\n";
    os << "\t<AssociatedStations>\n";
    os << "\t\t<StationsNumber>"<< origin.stations.size()
       << "</StationsNumber>\n";
    for(std::set<Station>::iterator it=origin.stations.begin();
                                    it!=origin.stations.end(); ++it)
        os << *it << "\n" ;
    os << "\t</AssociatedStations>\n";
    os << "</Origin>";
    return os;
}

std::string& operator << (std::string& os, const Origin& origin){
    os += "<Origin>\n";
    os +=  "\t<OriginID>" + origin.getOriginID()+ "</OriginID>\n";
    os += "\t<OriginDate>" + origin.getOriginDate().toString("yyyy-MM-dd").toStdString() + "</OriginDate>\n";
    os += "\t<OriginTime>" + origin.getOriginTime().toString("hh:mm:ss.z").toStdString() + "</OriginTime>\n";
    os += "\t<OriginMagnitude>" +  QString::number((double)origin.getMagnitude()).toStdString() +  "</OriginMagnitude>\n";
    os += "\t<OriginLatitude>" +  QString::number((double)origin.getLatitude()).toStdString() + "</OriginLatitude>\n";
    os += "\t<OriginLongitude>" + QString::number((double)origin.getLongitude()).toStdString() +  "</OriginLongitude>\n";
    os += "\t<StationsAssocied>\n";
    os += "\t\t<NumberStations>"+ QString::number(origin.stations.size()).toStdString() + "</NumberStations>\n";
    for(std::set<Station>::iterator it=origin.stations.begin(); it!=origin.stations.end(); ++it){
            os << *it;
            os += "\n" ;
    }
    os += "\t</StationsAssocied>\n";
    os += "</Origin>";
    return os;
}

/**To String method*/
/**uses the output operator format to create a string**/
std::string Origin::originToString() const{
    std::ostringstream convert;

    convert << "Origin ID: " << this->originID << "\n";
    convert << "Timestamp: " << this->originDate.toString().toStdString()
                             << this->originDate.toString().toStdString()<<"\n";
    convert << "Latitude: "<< this->latitude
            << "\nLongitude: " << this->longitude << "\n";
    if (this->magnitude != 0) convert << "Magnitude: " << this->magnitude << "\n";
    if (!(this->stations.empty())){
        convert << "Related Stations:\n";
        foreach(Station st,this->stations){
            convert << st.getStationID() << "\n";
        }
    }
    return convert.str();
}
