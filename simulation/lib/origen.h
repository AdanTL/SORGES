#ifndef Origen_H
#define Origen_H

#include <ctime>
#include <set>
#include "lib/station.h"

class Origen{

    public:
    Origen(const std::string& Id, const std::tm&  Time, long double Lat, long double Lon, double Magn, const std::set<Station>& Stas);

    friend bool operator < (const Origen& op1, const Origen& op2);
    friend std::ostream& operator << (std::ostream& os, const Origen& ar);
    private:

    std::string origenID;
    struct std::tm origenTime;
    long double latitud;
    long double longitud;
    double magnitud;
    std::set<Station> stations;


};

Origen::Origen(const std::string& Id, const std::tm&  Time, long double Lat, long double Lon, double Magn, const std::set<Station>& Stas):
    origenID(Id),origenTime(Time),latitud(Lat),longitud(Lon),magnitud(Magn),stations(Stas){}

bool operator < (const Origen& op1, const Origen& op2){
    return op1.origenID<op2.origenID;
}

std::ostream& operator << (std::ostream& os, const Origen& ar){
    os << "Timestamp: " << 1900 + ar.origenTime.tm_year << "-" << 1 + ar.origenTime.tm_mon << "-" << ar.origenTime.tm_mday ;
    os << " " << ar.origenTime.tm_hour << ":" << ar.origenTime.tm_min << ":" << ar.origenTime.tm_sec;
    os << "\nMagnitude: " << ar.magnitud << "\nLatitude: ";
    os << ar.latitud << "\nLongitude: " << ar.longitud << "\n";
    for(std::set<Station>::iterator it=ar.stations.begin(); it!=ar.stations.end(); ++it)
        os << *it;
    return os;
}


#endif
