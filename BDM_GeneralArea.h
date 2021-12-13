#ifndef BDM_GENERAL_AREA_H
#define BDM_GENERAL_AREA_H

#include "BasicADS.h"
#include "Mdp.h"


void readDeviceName(BasicADS &adsClient){

	unsigned long n_err;
	unsigned long strLen = 0;
	char s_deviceName[50];

	n_err = adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_DEVICE_NAME, sizeof(s_deviceName), s_deviceName, &strLen);

	if (n_err != ADSERR_NOERR) {
		std::cout << "Error AdsSyncReadReq: " << n_err << std::endl;
		exit(-1);
	}
	s_deviceName[strLen] = 0; // End String
	std::cout << "Device Name: " << s_deviceName << std::endl;
}




#endif