
#include "setting.hpp"

using namespace std; 

Settings::Settings() 
{
	
}

void Settings::setValue(string str, bool val) 
{
	value newVal; 
	newVal.boolVal = val; 
	this.vals[str] = newValue; 
}

void Settings::setValue(string str, int val) 
{
	value newVal; 
	newVal.intVal = val; 
	this.vals[str] = newValue; 
}
void Settings::setValue(string str, std::string val) 
{
	value newVal; 
	newVal.strVal = val; 
	this.vals[str] = newValue; 
}

bool Settings::getBool(string str) 
{
	return this.vals[str].boolVal; 
}
int Settings::getInt(string str) 
{
	return this.vals[str].intVal; 
} 
string Settings::getStr(string str) 
{
	return this.vals[str].strVal; 
}
