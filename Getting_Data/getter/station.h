#ifndef station_H
#define station_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>


#include "getter.h"
 
class station{
	
	public:
	
	station(std::string str);
	
	private:
	
	std::string Time;
	std::string Target;
	std::pair<double,double> coordenates;
	double PGV;
	double LeadTime;
};
	
		
	station::station(std::string str){
		// obtener cada valor de un texto con esta estructura:
		/*		
		2014-10-07 11:44:51.2	Target: Portimao(37.13;-8.53)
		2014-10-07 11:44:51.2	PGV: 0.00100149
		2014-10-07 11:44:51.2	LeadTime: 24.0501
		*/
		
		getter::get_section_determinated(str,"\r\n","\t",Time);
		getter::get_value_string(str, "Target", Target);
		getter::get_section_determinated(Target,"","(",Target);
		getter::get_value_range(str, "Target", coordenates);
		getter::get_value_double(str,"PGV",PGV);
		getter::get_value_double(str,"LeadTime",LeadTime);
		
		/*std::cout << Time << std::endl;
		std::cout << Target << std::endl;
		std::cout << coordenates.first << std::endl;
		std::cout << coordenates.second << std::endl;
		std::cout << PGV << std::endl;
		std::cout << LeadTime << std::endl;
		std::cout << "--------------------------------------" << std::endl;//*/
		
	}
	
	
#endif