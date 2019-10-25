#include "server.h"

Server::Server(std::string ipClient, int dbNumber, DBNumber* k){
	this->_numberClient++;
	this->setServerClient( ipClient, dbNumber, k);
}

void Server::setServerClient(std::string ipClient, int dbNumber, DBNumber* k){
	char t[ipClient.size() + 1];
	strcpy(t, ipClient.c_str());
	char* chars_array = strtok(t, ".");
	while(chars_array){
		ipdata.push_back(chars_array);
		chars_array = strtok(NULL, ".");
	}
	this->objectDBNumber.insert(std::pair<int, DBNumber*>(dbNumber, k));
	this->objectServer.insert(std::pair<std::string, Server*>(ipClient, this));
	std::cout << "Response "<<std::endl;
	std::cout << "Client number : " << this->_numberClient << ", dbNumber : "<< dbNumber << ", IP client :"<< this->ipdata[0]<< "." << this->ipdata[1] << "." << this->ipdata[2]<<"." << this->ipdata[3] << std::endl;

}
short Server::_numberClient;
std::map<std::string, Server*> Server::objectServer;




















