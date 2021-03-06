#include "BDM_DeviceArea.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <iostream>
#include <cstddef>


#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

DeviceArea::DeviceArea(BasicADS* adsClient)
	: m_adsClient(*adsClient) {};

DeviceArea::DeviceArea(const DeviceArea& other)
	: m_adsClient(other.m_adsClient) {};

DeviceArea& DeviceArea::operator=(const DeviceArea& other) {
	m_adsClient = other.m_adsClient;
	return *this;
}


void DeviceArea::readSerialNumber() {
	int32_t n_err = 0;
	uint32_t strLen = 0;
	char s_serialNo[50];

	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_SERIAL_NO, sizeof(s_serialNo), s_serialNo, &strLen);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	s_serialNo[strLen] = 0; // End String
	std::cout << "Serial Number: " << s_serialNo << std::endl;
}