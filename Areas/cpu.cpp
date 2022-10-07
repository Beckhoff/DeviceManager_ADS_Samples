#include "cpu.h"
#include "Mdp.h"
#include "BasicADS.h"

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

using namespace DeviceManager;

CPU::CPU(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	// Search if module type is present on the target
	// If so, assign module Id
	m_moduleId = getFirstModuleId(m_moduleType);
	m_bModuleExists = (m_moduleId > 1) ? true : false;
};

CPU::CPU(const CPU& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleId(other.m_moduleId) {};

CPU& CPU::operator=(const CPU& other) {
	m_adsClient = other.m_adsClient;
	m_moduleId = other.m_moduleId;
	return *this;
}

void DeviceManager::CPU::getFrequency(uint32_t& freq)
{
	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;

	uint32_t u32_cpu_freq_idx = 0;
	u32_cpu_freq_idx = 0x8000 + (m_moduleId << 4) + 1; // +1 for CPU properties table
	u32_cpu_freq_idx = (u32_cpu_freq_idx << 16) + 1;   // 1 = Subindex of CPU frequency

	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_cpu_freq_idx, sizeof(freq), &freq, &n_bytesRead);
}

void DeviceManager::CPU::getUsage(uint16_t& usage)
{
	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;

	uint32_t u32_cpu_usage_idx = 0;
	u32_cpu_usage_idx = 0x8000 + (m_moduleId << 4) + 1;	// + 1 for CPU properties table
	u32_cpu_usage_idx = (u32_cpu_usage_idx << 16) + 2;	// 2 = Subindex of CPU usage

	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_cpu_usage_idx, sizeof(usage), &usage, &n_bytesRead);
}

void DeviceManager::CPU::getTemp(int16_t& temp)
{
	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;

	uint32_t u32_cpu_temp_idx = 0;
	u32_cpu_temp_idx = 0x8000 + (m_moduleId << 4) + 1; // + 1 for CPU properties table
	u32_cpu_temp_idx = (u32_cpu_temp_idx << 16) + 3; // 3 = Subindex of CPU temeprature

	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_cpu_temp_idx, sizeof(temp), &temp, &n_bytesRead);
}