#ifndef OrigenH
#define OrigenH

#include <ctime>
#include <set>

class Origen{

    public:
    Origen(const std::string& Id, const std::tm&  Time, long double Lat, long double Lon, double Magn, const std::set<station>& Stas);
    Origen(const origen& o);

    friend bool operator < (const origen& op1, const origen& op2);
    friend std::ostream& operator << (std::ostream& os, const origen& ar);
    private:

    std::string origenID;
    struct std::tm originTime;
    long double latitud;
    long double longitud;
    double magnitud;
    std::set<station> stations;


};

origen::origen(const std::string& Id, const std::tm&  Time, long double Lat, long double Lon, double Magn, const std::set<station>& Stas){
    origenID = Id;
    originTime = Time;
    latitud = Lat;
    longitud = Lon;
    magnitud = Magn;
    stations= Stas;
}


origen::origen(const origen& o){
    origenID = o.origenID;
    originTime = o.originTime;
    latitud = o.latitud;
    longitud = o.longitud;
    magnitud = o.magnitud;
    stations = o.stations;
}

bool operator < (const origen& op1, const origen& op2){
    return op1.origenID<op2.origenID;
}

std::ostream& operator << (std::ostream& os, const origen& ar){
    os << "Timestamp: " << 1900 + ar.originTime.tm_year << "-" << 1 + ar.originTime.tm_mon << "-" << ar.originTime.tm_mday ;
    os << " " << ar.originTime.tm_hour << ":" << ar.originTime.tm_min << ":" << ar.originTime.tm_sec;
    os << "\nMagnitude: " << ar.magnitud << "\nLatitude: ";
    os << ar.latitud << "\nLongitude: " << ar.longitud << "\n";
    for(std::set<station>::iterator it=ar.stations.begin(); it!=ar.stations.end(); ++it)
        os << *it;
    return os;
}


#endif
