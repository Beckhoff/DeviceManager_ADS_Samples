#include "mass_storage_monitoring.h"
#include "Mdp.h"
#include "BasicADS.h"

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

using namespace DeviceManager;

MassStorageMonitoring::MassStorageMonitoring(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	// Search if module type is present on the target
	// If so, assign module Id
	m_moduleId = getFirstModuleId(m_moduleType);
	m_bModuleExists = (m_moduleId > -1) ? true : false;
};

MassStorageMonitoring::MassStorageMonitoring(const MassStorageMonitoring& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleId(other.m_moduleId) {};

MassStorageMonitoring& MassStorageMonitoring::operator=(const MassStorageMonitoring& other) {
	m_adsClient = other.m_adsClient;
	m_moduleId = other.m_moduleId;
	return *this;
}

int32_t MassStorageMonitoring::getDriveTypes(std::vector<uint8_t>& driveTypes)
{
	int32_t error = 0;
	uint16_t n_drives = 0;
	uint32_t n_bytesRead = 0;
	uint32_t u32_idx_drive_type_len = 0;
	u32_idx_drive_type_len = 0x8000 + (m_moduleId << 4) + 5; // +5 for Drive Type table
	u32_idx_drive_type_len = u32_idx_drive_type_len << 16;

	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_idx_drive_type_len, sizeof(n_drives), &n_drives, &n_bytesRead);
	if (error != ADSERR_NOERR) return error;

	if (n_bytesRead == 0 || n_drives == 0) return ADSERR_NOERR;

	for (int sub_idx = 1; sub_idx <= n_drives; sub_idx++)
	{
		uint8_t driveType = 0;
		uint32_t u32_idx_drive_type_string = 0;
		u32_idx_drive_type_string = 0x8000 + (m_moduleId << 4) + 5; // +5 for Drive Type table
		u32_idx_drive_type_string = (u32_idx_drive_type_string << 16) + sub_idx; // +1 for each drive letter (1..Len)

		error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_idx_drive_type_string, sizeof(driveType), &driveType, &n_bytesRead);
		if (error != ADSERR_NOERR) return error;

		driveTypes.push_back(driveType);
	}
	return error;
}