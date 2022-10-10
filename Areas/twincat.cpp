#include "twincat.h"
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

TwinCAT::TwinCAT(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	// Search if module type is present on the target
	// If so, assign module Id
	m_moduleId = getFirstModuleId(m_moduleType);
	m_bModuleExists = (m_moduleId > -1) ? true : false;
};

TwinCAT::TwinCAT(const TwinCAT& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleId(other.m_moduleId) {};

TwinCAT& TwinCAT::operator=(const TwinCAT& other) {
	m_adsClient = other.m_adsClient;
	m_moduleId = other.m_moduleId;
	return *this;
}

int32_t TwinCAT::deleteAdsRoute(const char route_name[]) {
	assert(route_name != NULL);
	assert(strlen(route_name) > 0);

	// Deleting ADS Route

	char service_transfer_object[50] = {};
	uint32_t route_name_length = (uint32_t)strlen(route_name);

	// Copy length of route name to service transfer object
	*reinterpret_cast<uint32_t*>(service_transfer_object) = route_name_length;
	// Copy the route name to the service transfer object
	memcpy(service_transfer_object + sizeof(uint32_t), route_name, route_name_length);

	uint32_t u32_del_ads_route_idx = 0xB001 + (m_moduleId << 4);
	u32_del_ads_route_idx = (u32_del_ads_route_idx << 16) + 1; // Subindex "Write Data"

	return m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_del_ads_route_idx, sizeof(uint32_t) + route_name_length, service_transfer_object);
}