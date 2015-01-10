#ifndef ORIGIN_H
#define ORIGIN_H

#include <ctime>
#include <set>
#include "station.h"

class Origin{

    public:
    Origin(const std::string& id = "", const std::tm& time = std::tm(), long double latitude = 0,
           long double longitude = 0, double magnitude = 0,
           const std::set<Station>& stations = std::set<Station>());
    
    //getters and setters
    std::string getOriginID() const;
    void setOriginID(const std::string &value);

    std::tm getOriginTime() const;
    void setOriginTime(const std::tm &value);

    long double getLatitude() const;
    void setLatitude(long double value);

    long double getLongitude() const;
    void setLongitude(long double value);

    double getMagnitude() const;
    void setMagnitude(double value);

    std::set<Station> getStations() const;
    void setStations(const std::set<Station> &value);

    //operators
    friend bool operator < (const Origin& origin1, const Origin& origin2);
    friend std::ostream& operator << (std::ostream& os, const Origin& origin);

private:
    std::string originID;
    struct std::tm originTime;
    long double latitude;
    long double longitude;
    double magnitude;
    std::set<Station> stations;

};

#endif
