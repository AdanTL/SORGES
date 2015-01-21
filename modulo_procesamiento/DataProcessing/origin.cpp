#include<iostream>
#include "origin.h"

Origin::Origin(const std::string& id, const QDate& date, const QTime& time, long double latitude,
    	   	   long double longitude, double magnitude, const std::set<Station>& stations):
               originID(id),
               originDate(date),
               originTime(time),
   			   latitude(latitude),
   			   longitude(longitude),
   			   magnitude(magnitude),
   			   stations(stations)
               {}

/**getters and setters*/
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


/**operators*/

bool operator < (const Origin& origin1, const Origin& origin2){
    QDateTime origin1DateTime, origin2DateTime;
    origin1DateTime.setDate(origin1.getOriginDate());
    origin2DateTime.setDate(origin2.getOriginDate());
    origin1DateTime.setTime(origin1.getOriginTime());
    origin2DateTime.setTime(origin2.getOriginTime());
    return origin1DateTime < origin2DateTime;
}

std::ostream& operator << (std::ostream& os, const Origin& origin){
    os << "<Origin>\n";
    os << "\t<OriginID>" << origin.getOriginID().c_str() << "</OriginID>\n";
    os << "\t<OriginDate>" << origin.getOriginDate().toString("yyyy-MM-dd").toStdString().c_str() << "</OriginDate>\n";
    os << "\t<OriginTime>" << origin.getOriginTime().toString("hh:mm:ss.z").toStdString().c_str() << "</OriginTime>\n";
    os << "\t<OriginMagnitude>" << origin.getMagnitude() <<  "</OriginMagnitude>\n";
    os << "\t<OriginLatitude>" << (double)origin.getLatitude() << "</OriginLatitude>\n";
    os << "\t<OriginLongitude>" << (double)origin.getLongitude() <<  "</OriginLongitude>\n";
    os << "\t<StationsAssocied>\n";
    os << "\t\t<NumberStations>"<< origin.stations.size() << "</NumberStations>\n";
    for(std::set<Station>::iterator it=origin.stations.begin(); it!=origin.stations.end(); ++it)
        os << *it << "\n" ;
    os << "\t</StationsAssocied>\n";
    os << "</Origin>\n";
    return os;
}


void Origin::fromQDomNode(const QDomNode& originNode){
    originID = originNode.firstChildElement("OriginID").text().toStdString();
    originDate = QDate::fromString(originNode.firstChildElement("OriginDate").text(),"yyyy-MM-dd");
    originTime = QTime::fromString(originNode.firstChildElement("OriginTime").text(),"hh:mm:ss.zzz");
    latitude = originNode.firstChildElement("OriginLatitude").text().toDouble();
    longitude = originNode.firstChildElement("OriginLongitude").text().toDouble();
    magnitude = originNode.firstChildElement("OriginMagnitude").text().toDouble();
    stations = Station::stationsFromQDomElement(originNode.firstChildElement("StationsAssocied"));
}

std::set<Origin> Origin::originsFromQDomDocument(const QDomDocument& xml){
    std::set<Origin> myOrigins;
    QDomNodeList origins = xml.elementsByTagName("Origin");
    for (int i = 0; i < origins.size(); i++) {
        Origin myOrigin;
        myOrigin.fromQDomNode(origins.item(i));
        std::cout << myOrigin << std::endl;
        myOrigins.insert(myOrigin);
    }
    return myOrigins;
}

std::string Origin::toStdString ()const{
    std::string os;

    os += "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \n";
    os += "<Origin>\n";
    os +=  "\t<OriginID>" + getOriginID()+ "</OriginID>\n";
    os += "\t<OriginDate>" + getOriginDate().toString("yyyy-MM-dd").toStdString() + "</OriginDate>\n";
    os += "\t<OriginTime>" + getOriginTime().toString("hh:mm:ss.z").toStdString() + "</OriginTime>\n";
    os += "\t<OriginMagnitude>" +  QString::number((double)getMagnitude()).toStdString() +  "</OriginMagnitude>\n";
    os += "\t<OriginLatitude>" +  QString::number((double)getLatitude()).toStdString() + "</OriginLatitude>\n";
    os += "\t<OriginLongitude>" + QString::number((double)getLongitude()).toStdString() +  "</OriginLongitude>\n";
    os += "\t<StationsAssocied>\n";
    os += "\t\t<NumberStations>"+ QString::number(stations.size()).toStdString() + "</NumberStations>\n";
    for(std::set<Station>::iterator it=stations.begin(); it!=stations.end(); ++it){
            os += (*it).toStdString();
            os += "\n" ;
    }
    os += "\t</StationsAssocied>\n";
    os += "</Origin>\n";
    return os;
}
