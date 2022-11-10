#include "mainboard.h"
#include "Mdp.h"
#include "BasicADS.h"

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

#include <memory>

using namespace DeviceManager;

Mainboard::Mainboard(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	// Search if module type is present on the target
	// If so, assign module Id
	m_moduleId = getFirstModuleId(m_moduleType);
	m_bModuleExists = (m_moduleId > -1) ? true : false;
};

Mainboard::Mainboard(const Mainboard& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleId(other.m_moduleId) {};

Mainboard& Mainboard::operator=(const Mainboard& other) {
	m_adsClient = other.m_adsClient;
	m_moduleId = other.m_moduleId;
	return *this;
}

int32_t Mainboard::getSerialNumber(std::string& serialNumber)
{
	int32_t error = 0;
	uint32_t strLen = 0;

	uint32_t u32_mb_serial_no_idx = 0;
	u32_mb_serial_no_idx = 0x8000 + (m_moduleId << 4) + 1; // +1 for Mainboard Information table
	u32_mb_serial_no_idx = (u32_mb_serial_no_idx << 16) + 2;   // 2 = Subindex for Serial Number

	auto sBuf = std::shared_ptr<char[]>(new char[m_cbStringBuf]);
	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_mb_serial_no_idx, m_cbStringBuf, sBuf.get(), &strLen);


	if (error != ADSERR_NOERR) return error;

	sBuf[strLen] = 0; // End String
	serialNumber = sBuf.get();
	return error;
}

int32_t Mainboard::getMinTemp(int32_t& temp)
{
	uint32_t n_bytesRead = 0;

	uint32_t u32_mb_min_temp_idx = 0;
	u32_mb_min_temp_idx = 0x8000 + (m_moduleId << 4) + 1; // +1 for Mainboard Information table
	u32_mb_min_temp_idx = (u32_mb_min_temp_idx << 16) + 6;   // 6 = Subindex for min. board temperature

	return m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_mb_min_temp_idx, sizeof(temp), &temp, &n_bytesRead);
}

int32_t Mainboard::getMaxTemp(int32_t& temp)
{
	uint32_t n_bytesRead = 0;

	uint32_t u32_mb_max_temp_idx = 0;
	u32_mb_max_temp_idx = 0x8000 + (m_moduleId << 4) + 1; // +1 for Mainboard Information table
	u32_mb_max_temp_idx = (u32_mb_max_temp_idx << 16) + 7;   // 7 = Subindex for min. board temperature

	return m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_mb_max_temp_idx, sizeof(temp), &temp, &n_bytesRead);
}

int32_t Mainboard::getTemp(int16_t& temp)
{
	uint32_t n_bytesRead = 0;

	uint32_t u32_mb_temp_idx = 0;
	u32_mb_temp_idx = 0x8000 + (m_moduleId << 4) + 1; // +1 for Mainboard Information table
	u32_mb_temp_idx = (u32_mb_temp_idx << 16) + 10;   // 10 = Subindex for temperature

	return m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_mb_temp_idx, sizeof(temp), &temp, &n_bytesRead);
}