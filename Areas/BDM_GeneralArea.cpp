#include "BDM_GeneralArea.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <iostream>
#include <cstddef>
#include <cstdint>

#if defined _WIN32 || defined __FreeBSD__
	#include "TcAdsDef.h"
#else
	#include "AdsDef.h"
#endif

using namespace DeviceManager;

GeneralArea::GeneralArea(BasicADS& adsClient)
	: m_adsClient(adsClient) {};

GeneralArea::GeneralArea(const GeneralArea& other)
	: m_adsClient(other.m_adsClient) {};

GeneralArea& GeneralArea::operator=(const GeneralArea& other) {
	m_adsClient = other.m_adsClient;
	return *this;
}

void GeneralArea::getDeviceName() {

	int32_t n_err = 0;
	uint32_t strLen = 0;
	char s_deviceName[50];

	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_DEVICE_NAME, sizeof(s_deviceName), s_deviceName, &strLen);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	s_deviceName[strLen] = 0; // End String
	std::cout << "Device Name: " << s_deviceName << std::endl;
}