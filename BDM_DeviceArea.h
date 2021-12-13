#ifndef BDM_DEVICE_AREA_H
#define BDM_DEVICE_AREA_H

#include "BasicADS.h"
#include "Mdp.h"

void readSerialNumber(BasicADS& adsClient) {
	unsigned long n_err;
	unsigned long strLen = 0;
	char s_serialNo[50];


	n_err = adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_SERIAL_NO, sizeof(s_serialNo), s_serialNo, &strLen);

	if (n_err != ADSERR_NOERR) {
		std::cout << "Error AdsSyncReadReq: " << n_err << std::endl;
		exit(-1);
	}
	s_serialNo[strLen] = 0; // End String
	std::cout << "Serial Number: " << s_serialNo << std::endl;
}

#endif