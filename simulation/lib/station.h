#ifndef Station_H
#define Station_H

 
class Station{
	
	public:
	
    Station(const std::string& Id, const std::string& Net_Id, long double Lat, long double Lon, int Col=-1);
    friend bool operator < (const Station& op1, const Station& op2);
    friend std::ostream& operator << (std::ostream& os, const Station& ar);

	private:
	
    std::string Station_ID;
    std::string Station_Network_ID;
	long double Latitud;
	long double Longitud;
	int color;
};
	
    // lista de inicializacion.
Station::Station(const std::string& Id, const std::string& Net_Id, long double Lat, long double Lon, int Col){
    Station_ID = Id;
    Station_Network_ID = Net_Id;
	Latitud = Lat;
	Longitud = Lon;
	color = Col;	
}
	
bool operator < (const Station& op1, const Station& op2){
    return op1.Station_ID<op2.Station_ID;
}
	


std::ostream& operator << (std::ostream& os, const Station& ar){
    os << "Station ID: " << ar.Station_ID ;
    os << "\nStation Network ID: " << ar.Station_Network_ID << "\nLatitude: ";
    os << ar.Latitud << "\nLongitude: " << ar.Longitud << "\nCode: " << ar.color << "\n";
    return os;
}

#endif
