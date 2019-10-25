#ifndef plcnumber_h
#define plcnumber_h
#include <iostream>

class PLCNumber{
	private:
		std::string _ipPLC;
		short _rack;
		short _slot;
	public:
		struct DataPLCNumber
		{
			std::string ipClient;
			std::string ipPLC;
			int dbNumber;
			short rack;
			short slot;
		} dataPLCNumber;

		PLCNumber(std::string ipClient, std::string ipPLC,int dbNumber, short rack, short slot);
		void setPLCNumber(std::string ipPLC, short rack, short slot);
		std::string getIpPLC();
		short getRack();
		short getSlot();
		void setDataPLCNumber(std::string ipClient, std::string ipPLC, int dbNumber, short rack, short slot);
};
#endif // snap7_h