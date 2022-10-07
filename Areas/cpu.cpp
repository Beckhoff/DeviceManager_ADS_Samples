#include "cpu.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <assert.h>
#include <iostream> // if ndebug

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

void CPU::readCPU() {
	std::cout << "> Read CPU Information:" << std::endl;

	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;

	// Read CPU frqeuency


	uint32_t u32_cpu_freq_idx = 0;
	u32_cpu_freq_idx = 0x8000 + (m_moduleId << 4) + 1; // +1 for CPU properties table
	u32_cpu_freq_idx = (u32_cpu_freq_idx << 16) + 1;   // 1 = Subindex of CPU frequency

	uint32_t u32_cpu_freq = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_cpu_freq_idx, sizeof(u32_cpu_freq), &u32_cpu_freq, &n_bytesRead);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	std::cout << ">>> CPU frequency: " << u32_cpu_freq << "MHz" << std::endl;


	// Read CPU usage


	uint32_t u32_cpu_usage_idx = 0;
	u32_cpu_usage_idx = 0x8000 + (m_moduleId << 4) + 1;	// + 1 for CPU properties table
	u32_cpu_usage_idx = (u32_cpu_usage_idx << 16) + 2;	// 2 = Subindex of CPU usage

	uint16_t u16_cpu_usage = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_cpu_usage_idx, sizeof(u16_cpu_usage), &u16_cpu_usage, &n_bytesRead);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	std::cout << ">>> CPU usage: " << u16_cpu_usage << "%" << std::endl;


	// Read CPU Temperature


	uint32_t u32_cpu_temp_idx = 0;
	u32_cpu_temp_idx = 0x8000 + (m_moduleId << 4) + 1; // + 1 for CPU properties table
	u32_cpu_temp_idx = (u32_cpu_temp_idx << 16) + 3; // 3 = Subindex of CPU temeprature

	uint16_t u16_cpu_temperature = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_cpu_temp_idx, sizeof(u16_cpu_temperature), &u16_cpu_temperature, &n_bytesRead);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	std::cout << ">>> CPU temperature: " << u16_cpu_temperature << " C" << std::endl;
}