
#ifndef PAGEPROC_SETTINGS_HPP 
#define PAGEPROC_SETTINGS_HPP

#include <map> 
#include <string> 

namespace PageProc {

private: 
	
	union value 
	{
		bool boolVal; 
		int intVal; 
		std::string strVal; 
	}; 

	std::map<value> vals; 

public: 
	
	Settings(); 

	void setValue(std::string str, bool val); 
	void setValue(std::string str, int val);
	void setValue(std::string str, std::string val);

	bool getBool(std::string str); 
	int getInt(std::string str); 
	std::string getStr(std::string str); 

} // namespace PageProc
