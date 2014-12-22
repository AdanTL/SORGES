#ifndef Station_H
#define Station_H

 
class Station{
	
	public:
	
    Station(const std::string& Id, const std::string& Net_Id, long double Lat, long double Lon, int Col=-1);
    friend bool operator < (const Station& op1, const Station& op2);
    friend std::ostream& operator << (std::ostream& os, const Station& ar);

	private:
	
    std::string stationID;
    std::string stationNetworkID;
    long double latitud;
    long double longitud;
	int color;
};
	
    // lista de inicializacion.
Station::Station(const std::string& Id, const std::string& NetId, long double Lat, long double Lon, int Col):
    stationID(Id), stationNetworkID(NetId),latitud(Lat),longitud(Lon),color(Col){}
	
bool operator < (const Station& op1, const Station& op2){
    return op1.stationID<op2.stationID;
}
	


std::ostream& operator << (std::ostream& os, const Station& ar){
    os << "Station ID: " << ar.stationID ;
    os << "\nStation Network ID: " << ar.stationNetworkID << "\nLatitude: ";
    os << ar.latitud << "\nLongitude: " << ar.longitud << "\nCode: " << ar.color << "\n";
    return os;
}

#endif
