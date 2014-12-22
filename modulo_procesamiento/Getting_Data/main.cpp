 #include <stdlib.h>
 #include <iostream>
 #include <vector>	
 #include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <utility>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 #include "getter/getter.h"
 #include "getter/origen.h"
 
 
 std::string seach_file(const char* file);
 
 int main(int argc, char *argv[])
 {
	std::string file_content = seach_file("log.log");
	std::vector<std::string> content_origens = getter::get_sections_origen(file_content);
	std::vector<origen> origens;
	for(size_t i=0; i<content_origens.size(); i++){
		origens.push_back(origen(content_origens[i]));
	}
	return 0;
 }

 
 
 		
std::string seach_file(const char* file){
    std::ifstream streamfile;
    streamfile.open(file,std::ifstream::in);
	char * characters= new char[500];
	std::string st="";
	std::stringstream ss;
	
	if(streamfile){
		streamfile.getline(characters,500);
		while( !streamfile.eof()){
			ss << characters << '\n';
			streamfile.getline(characters,500);
		}
		ss << characters << '\n';
		st = ss.str();
		
	}else{
		std::cerr << "FILE " << file << " NOT FOUND. Aborting..."<< std::endl;
		assert(false && "FILE NOT FOUND");
	}
	return st;
}
