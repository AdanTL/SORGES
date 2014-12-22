#ifndef origen_H
#define origen_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>


 #include "getter.h"
 #include "station.h"

class origen{
	
	public:
	
	origen(const std::string& str);
	
	private:
	
	std::string CreationTime;
	int count_oringin;
	std::string id;
	std::string autor;
	int phase_count;
	int station_count;
	bool is_event;
	std::string event_id;
	
	std::vector<station> stations;
	
	
	
};
	
origen::origen(const std::string& str){
		// obtener cada valor del texto:
		/*
		2014-10-07 11:44:50.9	NUEVO ORIGEN - count_origin= 3
		2014-10-07 11:44:50.9	Origen asociado: Origin#20141007114450.532423.1037471
		2014-10-07 11:44:50.9	Autor: scautoloc@geo1
		2014-10-07 11:44:50.9	    CreationTime= 2014-10-07 11:44:50.5
		2014-10-07 11:44:50.9	    Associated Phase Count= 6
		2014-10-07 11:44:50.9	    Associated Station Count= 6
		2014-10-07 11:44:51.2	count_origin= 3
		
		2014-10-08 04:52:13.5	ATENCIÓN!!! - EVENTO -gfz2014tryl
		*/
		//std::string CreationTime;
		getter::get_value_int(str,"count_origin",count_oringin);
		getter::get_value_int(str,"Associated Phase Count",phase_count);
		getter::get_value_int(str,"Associated Station Count",station_count);
		getter::get_value_string(str,"CreationTime",CreationTime);
		getter::get_value_string(str,"Autor",autor);
		getter::get_value_string(str,"Origen asociado",id);
		is_event = getter::get_value_string(str,"ATENCIÓN!!! - EVENTO",event_id);
		
		/*std::cout << count_oringin << std::endl;
		std::cout << phase_count << std::endl;
		std::cout << station_count << std::endl;
		std::cout << CreationTime << std::endl;
		std::cout << autor << std::endl;
		std::cout << id << std::endl;
		std::cout << event_id << std::endl;
		std::cout << "_----------------------------------------_" << std::endl;//*/
		
		//Obtener estas cadenas y pasarlas al constructor de station:
		/*
		2014-10-07 11:44:51.2	Target: Cadiz(36.32;-6.18)
		2014-10-07 11:44:51.2	PGV: 0.00151009
		2014-10-07 11:44:51.2	LeadTime: 2.19425
		*/
		
		std::vector<std::string> contents_stations = getter::get_sections_station(str);
		for(size_t i=0; i<contents_stations.size(); i++)
			stations.push_back(station(contents_stations[i]));
	}
	
	
#endif