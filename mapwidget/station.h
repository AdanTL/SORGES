#ifndef STATION_H
#define STATION_H
#include <string>
#include <map>
 
class Station{
	
	public:
    Station(const std::string& id ="", const std::string& netId ="", long double latitude = 0,
            long double longitude = 0, int color = -1);

   	//escala de colores: correspondencias numero->color hexadecimal
   	static std::map<int,const char*> colorScale;

    //getters and setters
    std::string getStationID() const;
    void setStationID(const std::string &value);

    std::string getNetworkID() const;
    void setNetworkID(const std::string &value);

    long double getLatitude() const;
    void setLatitude(long double value);

    long double getLongitude() const;
    void setLongitude(long double value);

    int getColor() const;
    void setColor(int value);
    const char* getCurrentColorScale();

    //operators
    friend bool operator < (const Station& station1, const Station& station2);

    bool operator == (const Station& station){
        return stationID == station.stationID;
    }

    friend std::ostream& operator << (std::ostream& os, const Station& station);

private:
    std::string stationID;
    std::string networkID;
    long double latitude;
    long double longitude;
	int color;
};

#endif
