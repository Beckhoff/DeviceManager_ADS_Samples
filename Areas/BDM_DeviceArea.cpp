#include "BDM_DeviceArea.h"
#include "Mdp.h"
#include "BasicADS.h"

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

#include <memory>

using namespace DeviceManager;

DeviceArea::DeviceArea(BasicADS& adsClient)
	: m_adsClient(adsClient) {};

DeviceArea::DeviceArea(const DeviceArea& other)
	: m_adsClient(other.m_adsClient) {};

DeviceArea& DeviceArea::operator=(const DeviceArea& other) {
	m_adsClient = other.m_adsClient;
	return *this;
}


int32_t DeviceArea::getSerialNumber(std::string& serialNo) {
	int32_t error = 0;
	uint32_t strLen = 0;

	auto sBuf = std::shared_ptr<char[]>(new char[m_stringBuf]);

	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_SERIAL_NO, m_stringBuf, sBuf.get(), &strLen);

	if (error != ADSERR_NOERR) return error;
	
	sBuf[strLen] = 0; // End String
	serialNo = sBuf.get();
	return error;
}
