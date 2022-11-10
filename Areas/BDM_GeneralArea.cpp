#include "BDM_GeneralArea.h"
#include "Mdp.h"
#include "BasicADS.h"

#if defined _WIN32 || defined __FreeBSD__
	#include "TcAdsDef.h"
#else
	#include "AdsDef.h"
#endif

#include <memory>


using namespace DeviceManager;

GeneralArea::GeneralArea(BasicADS& adsClient)
	: m_adsClient(adsClient) {};

GeneralArea::GeneralArea(const GeneralArea& other)
	: m_adsClient(other.m_adsClient) {};

GeneralArea& GeneralArea::operator=(const GeneralArea& other) {
	m_adsClient = other.m_adsClient;
	return *this;
}

int32_t GeneralArea::getDeviceName(std::string& deviceName) {

	int32_t error = 0;
	uint32_t strLen = 0;

	auto sBuf = std::shared_ptr<char[]>(new char[m_cbStringBuf]);
	
	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_DEVICE_NAME, m_cbStringBuf, sBuf.get(), &strLen);

	if (error != ADSERR_NOERR) return error;

	sBuf[strLen] = 0; // End String
	deviceName = sBuf.get();
	return error;
}
