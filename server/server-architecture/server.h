#ifndef server_h
#define server_h
#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include "dbnumber.h"


class Server{
private:
	std::vector<char*> ipdata;
	static short _numberClient;
public:
	std::string ipstring;
	std::map<int, DBNumber*> objectDBNumber;
	static std::map<std::string, Server*> objectServer;
	Server(std::string ipClient, int dbNumber ,DBNumber* k);
	void setServerClient(std::string ipClient, int dbNumber, DBNumber* k);
};
#endif

