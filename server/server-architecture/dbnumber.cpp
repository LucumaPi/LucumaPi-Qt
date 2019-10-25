#include "dbnumber.h"

DBNumber::DBNumber(std::string ipPLC, int dbNumber, PLCNumber* k){
	this->setDBNumberPLCNumber(ipPLC, dbNumber, k);
}

std::map<int, DBNumber*> DBNumber::objectDBNumber;

void DBNumber::setDBNumberPLCNumber(std::string ipPLC, int dbNumber, PLCNumber* k){
	this->objectPLCNumber.insert(std::pair<std::string, PLCNumber*>(ipPLC, k));
	this->objectDBNumber.insert(std::pair<int, DBNumber*>(dbNumber, this));
	PLCNumber *plcnumberPointer = this->objectPLCNumber.find(ipPLC)->second;
	std::cout << "IP PLC : " << plcnumberPointer->getIpPLC() <<", rack : " << plcnumberPointer->getRack()<< ", slot : " << plcnumberPointer->getSlot()<< std::endl;
}

