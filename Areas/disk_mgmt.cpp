#include "disk_mgmt.h"
#include "Mdp.h"
#include "BasicADS.h"

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

using namespace DeviceManager;

DiskMgmt::DiskMgmt(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	m_moduleId = getFirstModuleId(m_moduleType);
	m_moduleIds = getModuleIds(m_moduleType);
	m_bModuleExists = (m_moduleIds.empty()) ? false : true;
};

DiskMgmt::DiskMgmt(const DiskMgmt& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleIds(other.m_moduleIds)
	, m_moduleId(other.m_moduleId) {};

DiskMgmt& DiskMgmt::operator=(const DiskMgmt& other) {
	m_adsClient = other.m_adsClient;
	m_moduleIds = other.m_moduleIds;
	m_moduleId = other.m_moduleId;
	return *this;
}

DiskMgmt& DeviceManager::DiskMgmt::operator[](int idx)
{
	m_moduleId = m_moduleIds[idx];
	return *this;
}

uint32_t DeviceManager::DiskMgmt::count()
{
	return static_cast<uint32_t>(m_moduleIds.size());
}