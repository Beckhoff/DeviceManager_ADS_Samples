#include "nic.h"
#include "Mdp.h"
#include "BasicADS.h"

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

#include <memory>
#include <cstring>

using namespace DeviceManager;

NIC::NIC(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	m_moduleId = getFirstModuleId(m_moduleType);
	m_moduleIds = getModuleIds(m_moduleType);
	m_bModuleExists = (m_moduleIds.empty()) ? false : true;
};

NIC::NIC(const NIC& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleIds(other.m_moduleIds)
	, m_moduleId(other.m_moduleId) {};

NIC& NIC::operator=(const NIC& other) {
	m_adsClient = other.m_adsClient;
	m_moduleIds = other.m_moduleIds;
	m_moduleId = other.m_moduleId;
	return *this;
}

NIC& DeviceManager::NIC::operator[](int idx)
{
	m_moduleId = m_moduleIds[idx];
	return *this;
}

int32_t DeviceManager::NIC::getIPv4Address(std::string& IPv4)
{
	int32_t error = 0;
	uint32_t strLen = 0;

	auto sBuf = std::shared_ptr<char[]>(new char[m_cbStringBuf]);

	uint32_t u32_NIC_properties = 0;
	u32_NIC_properties = 0x8001 + (m_moduleId << 4);
	u32_NIC_properties = (u32_NIC_properties << 16) + 2; // subindex for IP-Address

	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_NIC_properties, m_cbStringBuf, sBuf.get(), &strLen);

	if (error != ADSERR_NOERR) return error;

	sBuf[strLen] = 0; // End String
	IPv4 = sBuf.get();
	return error;
}

int32_t DeviceManager::NIC::setIPv4Address(const char IPv4[])
{
	int32_t error = 0;
	uint32_t strLen = 0;

	uint32_t u32_NIC_properties = 0;
	u32_NIC_properties = 0x8001 + (m_moduleId << 4);
	u32_NIC_properties = (u32_NIC_properties << 16) + 2; // subindex for IP-Address

	return m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_NIC_properties, (uint32_t)strlen(IPv4), const_cast<char*>(IPv4));
}

uint32_t DeviceManager::NIC::count()
{
	return m_moduleIds.size();
}
