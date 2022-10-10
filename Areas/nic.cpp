#include "nic.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <assert.h>
#include <iostream> // if ndebug
#include <cstring>

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

using namespace DeviceManager;

NIC::NIC(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	// Search if module type is present on the target
	// If so, assign module Id
	m_moduleId = getFirstModuleId(m_moduleType);
	m_bModuleExists = (m_moduleId > -1) ? true : false;
};

NIC::NIC(const NIC& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleId(other.m_moduleId) {};

NIC& NIC::operator=(const NIC& other) {
	m_adsClient = other.m_adsClient;
	m_moduleId = other.m_moduleId;
	return *this;
}

int32_t NIC::changeIPAddress() {

	int32_t error = 0;
	uint32_t strLen = 0;

	uint32_t u32_NIC_properties = 0;
	u32_NIC_properties = 0x8001 + (m_moduleId << 4);
	u32_NIC_properties = (u32_NIC_properties << 16) + 2; // subindex for IP-Address

	char s_ipAddr[50] = {};
	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_NIC_properties, sizeof(s_ipAddr), s_ipAddr, &strLen);

	if (error != ADSERR_NOERR) return error;

	s_ipAddr[strLen] = 0;

	std::cout << ">>> Current IP-Address:: " << s_ipAddr << std::endl;

	// Change IP-Address of first NIC

	std::cout << ">>> Performing address change..." << std::endl;

	char new_address[] = "192.168.3.106";

	// Write new address
	error = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_NIC_properties, (uint32_t)strlen(new_address), new_address);
	if (error != ADSERR_NOERR) return error;

	// Read new address again
	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_NIC_properties, sizeof(s_ipAddr), s_ipAddr, &strLen);
	if (error != ADSERR_NOERR) return error;

	s_ipAddr[strLen] = 0;

	std::cout << ">>> New IP-Address:: " << s_ipAddr << std::endl;
	return error;
}