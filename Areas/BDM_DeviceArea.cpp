#include "BDM_DeviceArea.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <iostream>
#include <cstddef>


#ifdef _WIN32
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif


void readSerialNumber(BasicADS& adsClient) {
	long n_err = 0;
	uint32_t strLen = 0;
	char s_serialNo[50];

	n_err = adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_SERIAL_NO, sizeof(s_serialNo), s_serialNo, &strLen);

	if (n_err != ADSERR_NOERR) {
		std::cout << "Error AdsSyncReadReq: " << n_err << std::endl;
		exit(-1);
	}
	s_serialNo[strLen] = 0; // End String
	std::cout << "Serial Number: " << s_serialNo << std::endl;
}