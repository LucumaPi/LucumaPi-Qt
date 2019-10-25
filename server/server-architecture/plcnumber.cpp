#include "plcnumber.h"

PLCNumber::PLCNumber(std::string ipClient, std::string ipPLC, int dbNumber, short rack, short slot){
	int f = 0;
	this->setPLCNumber( ipPLC, rack, slot);
	this->setDataPLCNumber( ipClient, ipPLC, dbNumber, rack, slot);
}
void PLCNumber::setPLCNumber(std::string ipPLC, short rack, short slot){
	this->_ipPLC = ipPLC;
	this->_rack = rack;
	this->_slot = slot;
}

std::string PLCNumber::getIpPLC(){
	return this->_ipPLC;
}

short PLCNumber::getRack(){
	return this->_rack;
}

short PLCNumber::getSlot(){
	return this->_slot;
}

void PLCNumber::setDataPLCNumber(std::string ipClient, std::string ipPLC, int dbNumber, short rack, short slot){
	this->dataPLCNumber.ipClient =  ipClient;
	this->dataPLCNumber.ipPLC =  ipPLC;
	this->dataPLCNumber.dbNumber =  dbNumber;
	this->dataPLCNumber.rack =  rack;
	this->dataPLCNumber.slot =  slot;

	std::cout << "data Struct are : " << std::endl;
	std::cout << "ip Client "<< this->dataPLCNumber.ipClient << std::endl;
	std::cout << "ip PLC "<< this->dataPLCNumber.ipPLC << std::endl;
	std::cout << "db Number "<< this->dataPLCNumber.dbNumber << std::endl;
	std::cout << "rack "<< this->dataPLCNumber.rack << std::endl;
	std::cout << "slot "<< this->dataPLCNumber.slot << std::endl;
}