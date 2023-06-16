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

DiskMgmt& DiskMgmt::operator[](int idx)
{
	m_moduleId = m_moduleIds[idx];
	return *this;
}

uint32_t DiskMgmt::count()
{
	return static_cast<uint32_t>(m_moduleIds.size());
}

int32_t DiskMgmt::getVolumeLabels(std::vector<std::string>& volLabels)
{
	int32_t error = 0;
	uint16_t n_volLables = 0;
	uint32_t n_bytesRead = 0;
	uint32_t u32_idx_vol_lbl_len = 0;
	u32_idx_vol_lbl_len = 0x8000 + (m_moduleId << 4) + 2; // +2 for Volume Label table
	u32_idx_vol_lbl_len = u32_idx_vol_lbl_len << 16;

	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_idx_vol_lbl_len, sizeof(n_volLables), &n_volLables, &n_bytesRead);
	if (error != ADSERR_NOERR) return error;

	if (n_bytesRead == 0 || n_volLables == 0) return ADSERR_NOERR;

	for(int sub_idx = 1; sub_idx <= n_volLables; sub_idx++)
	{
		uint32_t u32_idx_vol_lbl_string = 0;
		u32_idx_vol_lbl_string = 0x8000 + (m_moduleId << 4) + 2; // +2 for Volume Label table
		u32_idx_vol_lbl_string = (u32_idx_vol_lbl_string << 16) + sub_idx; // +1 for each volume label (1..Len)

		auto sBuf = std::shared_ptr<char[]>(new char[m_cbStringBuf]);
		
		error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_idx_vol_lbl_string, m_cbStringBuf, sBuf.get(), &n_bytesRead);
		if (error != ADSERR_NOERR) return error;

		sBuf[n_bytesRead] = 0; // End String
		volLabels.push_back(std::string(sBuf.get()));	
	}
	return error;
}

int32_t DiskMgmt::getDriveLetters(std::vector<std::string>& driveLetters)
{
	int32_t error = 0;
	uint16_t n_driveLetters = 0;
	uint32_t n_bytesRead = 0;
	uint32_t u32_idx_drive_letter_len = 0;
	u32_idx_drive_letter_len = 0x8000 + (m_moduleId << 4) + 1; // +1 for Drive Letter table
	u32_idx_drive_letter_len = u32_idx_drive_letter_len << 16;

	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_idx_drive_letter_len, sizeof(n_driveLetters), &n_driveLetters, &n_bytesRead);
	if (error != ADSERR_NOERR) return error;

	if (n_bytesRead == 0 || n_driveLetters == 0) return ADSERR_NOERR;

	for (int sub_idx = 1; sub_idx <= n_driveLetters; sub_idx++)
	{
		uint32_t u32_idx_drive_letter_string = 0;
		u32_idx_drive_letter_string = 0x8000 + (m_moduleId << 4) + 1; // +1 for Drive Letter table
		u32_idx_drive_letter_string = (u32_idx_drive_letter_string << 16) + sub_idx; // +1 for each drive letter (1..Len)

		auto sBuf = std::shared_ptr<char[]>(new char[m_cbStringBuf]);

		error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_idx_drive_letter_string, m_cbStringBuf, sBuf.get(), &n_bytesRead);
		if (error != ADSERR_NOERR) return error;

		sBuf[n_bytesRead] = 0; // End String
		driveLetters.push_back(std::string(sBuf.get()));
	}
	return error;
}

int32_t DiskMgmt::getFileSystems(std::vector<std::string>& fileSystems)
{
	int32_t error = 0;
	uint16_t n_fileSystems = 0;
	uint32_t n_bytesRead = 0;
	uint32_t u32_idx_file_sys_len = 0;
	u32_idx_file_sys_len = 0x8000 + (m_moduleId << 4) + 3; // +3 for File System table
	u32_idx_file_sys_len = u32_idx_file_sys_len << 16;

	error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_idx_file_sys_len, sizeof(n_fileSystems), &n_fileSystems, &n_bytesRead);
	if (error != ADSERR_NOERR) return error;

	if (n_bytesRead == 0 || n_fileSystems == 0) return ADSERR_NOERR;

	for (int sub_idx = 1; sub_idx <= n_fileSystems; sub_idx++)
	{
		uint32_t u32_idx_file_system_string = 0;
		u32_idx_file_system_string = 0x8000 + (m_moduleId << 4) + 3; // +3 for File System table
		u32_idx_file_system_string = (u32_idx_file_system_string << 16) + sub_idx; // +1 for each drive letter (1..Len)

		auto sBuf = std::shared_ptr<char[]>(new char[m_cbStringBuf]);

		error = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_idx_file_system_string, m_cbStringBuf, sBuf.get(), &n_bytesRead);
		if (error != ADSERR_NOERR) return error;

		sBuf[n_bytesRead] = 0; // End String
		fileSystems.push_back(std::string(sBuf.get()));
	}
	return error;
}
