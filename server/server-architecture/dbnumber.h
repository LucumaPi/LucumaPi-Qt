#ifndef dbnumber_h
#define dbnumber_h
#include <iostream>
#include <map>
#include "plcnumber.h"

class DBNumber{
private:
	short _dbnumber;
public:
	DBNumber(std::string ipPLC, int dbNumber, PLCNumber* k);
	static std::map<int, DBNumber*> objectDBNumber;
	std::map<std::string, PLCNumber*> objectPLCNumber;
	void setDBNumberPLCNumber(std::string ipPLC, int dbNumber, PLCNumber* k);
};
#endif