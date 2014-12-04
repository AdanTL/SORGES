#ifndef GETTER_H
#ifndef GETTER_H
#define GETTER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

class getter{
	
	public:
	
	getter(){}
	static std::vector<std::string> get_sections_origen(const std::string& str) ;
	static std::vector<std::string> get_sections_station(const std::string& str) ;
	static bool get_section_determinated(const std::string& str, const std::string& begin, const std::string& end, std::string& ret) ;
	
	static bool get_value(const std::string& str, const std::string& name_value, std::string& ret) ;
	static bool get_value_string(const std::string& str, const std::string& name_value, std::string& ret) ;
	static bool get_value_double(const std::string& str, const std::string& name_value, double& ret) ;
	static bool get_value_int(const std::string& str, const std::string& name_value, int& ret) ;
	static bool get_value_range(const std::string& str, const std::string& name_value, std::pair<double,double>& ret);
	
	
};
	
	
std::vector<std::string> getter::get_sections_origen(const std::string& str){
		unsigned int pos= 0;
		unsigned int pos2= str.find("\r\n\r\n\r\n",pos);
		std::vector<std::string> ret;
		std::stringstream ss;
		
		while(pos != std::string::npos){
			ss << str.substr(pos,pos2-pos); 
			ret.push_back( ss.str());
			ss.str("");
			ss.clear();
			if(pos2 != std::string::npos)
				pos = pos2+6;
			else
				pos = pos2;
			pos2 =  str.find("\r\n\r\n\r\n",pos);
		}
		return ret;
	}
	
	
std::vector<std::string> getter::get_sections_station(const std::string& str){
		unsigned int pos= str.find("\tTarget:");
		unsigned int pos2= str.find("\r\n\r\n",pos);
		std::vector<std::string> ret;
		std::stringstream ss;
		
		while(pos != std::string::npos){
			ss << str.substr(pos,pos2-pos); 
			ret.push_back( ss.str());
			ss.str("");
			ss.clear();
			
			pos= str.find("\tTarget:",pos2);
			pos2 =  str.find("\r\n\r\n",pos);
		}
		return ret;
	}
	
	
	
bool getter::get_section_determinated(const std::string& str, const std::string& begin, const std::string& end, std::string& ret){
		unsigned int pos= str.find(begin);
		unsigned int pos2= str.find(end,pos+1);
		if(pos != std::string::npos){
			pos += begin.size();
			ret = str.substr(pos,(pos2-pos)); 
			return true;
		}else 
			return false;
	}
	
// Codigo posiblemente útil para reciclarlo :

bool getter::get_value(const std::string& str, const std::string& name_value, std::string& ret){
		unsigned int pos= str.find(name_value);
		unsigned int pos2;
		
		if(pos != std::string::npos){	
			pos += name_value.size()+2;
			pos2 = str.find("\r",pos);
			ret = str.substr(pos,(pos2-pos)); 
			return true;
		}
		else
			return false;
}

bool getter::get_value_string(const std::string& str, const std::string& name_value, std::string& ret){return get_value(str,name_value,ret);}

bool getter::get_value_double( const std::string& str, const std::string& name_value, double& ret){
	std::string st;
	bool found = get_value(str,name_value, st);
	if(found){
		ret = atof(st.c_str());
		return true;
	}else
		return false;
}

 bool getter::get_value_int(const std::string& str, const std::string& name_value, int& ret){ 
	std::string st;
	bool found = get_value(str,name_value, st);
	if(found){
		ret = atof(st.c_str());
		return true;
	}else
		return false;
}

bool getter::get_value_range(const std::string& str, const std::string& name_value, std::pair<double,double>& ret){
	unsigned int pos= str.find(name_value);
	unsigned int pos2;
	std::string m;

	if(pos != std::string::npos){	
		pos = str.find("(",pos);
		pos2 = str.find(")",pos);
		m = str.substr(pos+1,(pos2-pos)-1); 
	}
	else
		return false;
	
	std::string n;
	pos = m.find(",");
	n = m.substr(0,pos);
	m = m.substr(pos+2);
	
	ret = std::make_pair(atof(n.c_str()),atof(m.c_str()));
	return true;
	
	}

	
#endif