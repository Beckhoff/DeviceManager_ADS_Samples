#include "BDM_ConfigurationArea.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <iostream>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <algorithm>

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

ConfigurationArea::ConfigurationArea(BasicADS* adsClient)
	: m_adsClient(*adsClient) {

	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;

	// Read length of list of module IDs https://infosys.beckhoff.com/content/1031/devicemanager/45035996536742667.html?id=5503267175110745821
	uint16_t u16_len_module_id_list = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_DEVICE_AREA, sizeof(u16_len_module_id_list), &u16_len_module_id_list, &n_bytesRead);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	std::cout << "Number of MDP modules available on target: " << u16_len_module_id_list << std::endl;

	// Download table of module IDs (Configuration Area)
	for (int i = 1; i < u16_len_module_id_list; ++i) {

		uint32_t  u32_module_entry = 0;
		uint32_t indexOffset = MDP_IDX_OFFS_DEVICE_AREA + i;
		n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, indexOffset, sizeof(u32_module_entry), &u32_module_entry, &n_bytesRead);

		if (n_err != ADSERR_NOERR) {
			std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
			break;
		}

		uint16_t u16_highWord = u32_module_entry >> 16; // ModuleType
		uint16_t u16_lowWord = u32_module_entry & 0xFF; // ModuleId

		DeviceManager::Module module = {
			u32_module_entry >> 16, // ModuleType
			u32_module_entry & 0xFF // ModuleId
		};

		m_modules.push_back(module);
	}
};

ConfigurationArea::ConfigurationArea(const ConfigurationArea& other)
	: m_adsClient(other.m_adsClient)
	, m_modules(other.m_modules) {};

ConfigurationArea& ConfigurationArea::operator=(const ConfigurationArea& other){
	m_adsClient	= other.m_adsClient;
	m_modules	= other.m_modules;
	return *this;
}

void ConfigurationArea::changeIPAddress() {
	// MODULETYPE_NIC
	// https://infosys.beckhoff.com/content/1031/devicemanager/263013131.html

	// Get all NIC modules
	std::vector<DeviceManager::Module> nic_modules;
	auto is_nic = [](const DeviceManager::Module& m) { return m.ModuleType == MODULETYPE_NIC; };
	std::copy_if(m_modules.begin(), m_modules.end(), std::back_inserter(nic_modules), is_nic);

	if (nic_modules.empty()) {
		std::cout << "Not NIC modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first NIC module
	uint16_t moduleId = nic_modules.front().ModuleId;
	
	std::cout << ">>> Changing IP-Address..." << std::endl;

	int32_t n_err = 0;
	uint32_t strLen = 0;

	uint32_t u32_NIC_properties = 0;
	u32_NIC_properties = 0x8001 + (moduleId << 4);
	u32_NIC_properties = (u32_NIC_properties << 16) + 2; // subindex for IP-Address

	char s_ipAddr[50] = {};
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_NIC_properties, sizeof(s_ipAddr), s_ipAddr, &strLen);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	s_ipAddr[strLen] = 0;

	std::cout << ">>> Current IP-Address:: " << s_ipAddr << std::endl;


	// Change IP-Address of first NIC


	std::cout << ">>> Performing address change..." << std::endl;

	char new_address[] = "192.168.3.106";

	// Write new address
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_NIC_properties, strlen(new_address), new_address);
	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	// Read new address again
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_NIC_properties, sizeof(s_ipAddr), s_ipAddr, &strLen);
	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	s_ipAddr[strLen] = 0;

	std::cout << ">>> New IP-Address:: " << s_ipAddr << std::endl;
}

void ConfigurationArea::deleteAdsRoute() {
	// MODULETYPE_TWINCAT
	// https://infosys.beckhoff.com/content/1031/devicemanager/263030539.html?id=1967927695808387382 

	// Get all TWINCAT modules
	std::vector<DeviceManager::Module> twincat_modules;
	auto is_twincat = [](const DeviceManager::Module& m) { return m.ModuleType == MODULETYPE_TWINCAT; };
	std::copy_if(m_modules.begin(), m_modules.end(), std::back_inserter(twincat_modules), is_twincat);

	if (twincat_modules.empty()) {
		std::cout << "Not TWINCAT modules found on device" << std::endl;
		return;
	}

	// Get ModuleId from first TWINCAT module
	uint16_t moduleId = twincat_modules.front().ModuleId;

	char route_name[] = "CX-50C9E8";
	std::cout << ">>> Delete ADS Route \"" << route_name << "\"" << std::endl; 

	char service_transfer_object[50] = {};
	size_t route_name_length = strlen(route_name);

	// Copy length of route name to service transfer object
	*reinterpret_cast<uint32_t*>(service_transfer_object) = route_name_length;
	// Copy the route name to the service transfer object
	memcpy(service_transfer_object + sizeof(uint32_t), route_name, route_name_length);

	uint32_t u32_del_ads_route_idx = 0xB001 + (moduleId << 4);
	u32_del_ads_route_idx = (u32_del_ads_route_idx << 16) + 1; // Subindex "Write Data"

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_del_ads_route_idx, sizeof(uint32_t) + route_name_length, service_transfer_object);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	std::cout << ">>> Route deleteded successful" << std::endl;
}

void ConfigurationArea::readCPU() {
	// MODULETYPE_CPU
	// https://infosys.beckhoff.com/content/1033/devicemanager/54043195791430411.html?id=2286125776581746345

	// Get all CPU modules
	std::vector<DeviceManager::Module> cpu_modules;
	auto is_cpu = [](const DeviceManager::Module& m) { return m.ModuleType == MODULETYPE_CPU; };
	std::copy_if(m_modules.begin(), m_modules.end(), std::back_inserter(cpu_modules), is_cpu);
	
	if (cpu_modules.empty()) {
		std::cout << "Not CPU modules found on device" << std::endl;
		return;
	}

	// Get ModuleId from first CPU module
	uint16_t moduleId = cpu_modules.front().ModuleId;

	std::cout << ">>> Read CPU Information:" << std::endl;

	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;


	 // Read CPU frqeuency


	uint32_t u32_cpu_freq_idx = 0;
	u32_cpu_freq_idx = 0x8000 + (moduleId << 4) + 1; // +1 for CPU properties table
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
	u32_cpu_usage_idx = 0x8000 + (moduleId << 4) + 1;	// + 1 for CPU properties table
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
	u32_cpu_temp_idx = 0x8000 + (moduleId << 4) + 1; // + 1 for CPU properties table
	u32_cpu_temp_idx = (u32_cpu_temp_idx << 16) + 3; // 3 = Subindex of CPU temeprature

	uint16_t u16_cpu_temperature = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_cpu_temp_idx, sizeof(u16_cpu_temperature), &u16_cpu_temperature, &n_bytesRead);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	std::cout << ">>> CPU temperature: " << u16_cpu_temperature << " C" << std::endl;
}

void ConfigurationArea::readStateSecurityWizard() {
	// MODULETYPE_MISC
	// https://infosys.beckhoff.com/content/1033/devicemanager/263010571.html?id=2359555515732312853 

	// Get all MISC modules
	std::vector<DeviceManager::Module> misc_modules;
	auto is_misc = [](const DeviceManager::Module& m) { return m.ModuleType == MODULETYPE_MISC; };
	std::copy_if(m_modules.begin(), m_modules.end(), std::back_inserter(misc_modules), is_misc);

	if (misc_modules.empty()) {
		std::cout << "Not MISC modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first MISC module
	uint16_t moduleId = misc_modules.front().ModuleId;

	std::cout << ">>> Read state of the SecurityWizard:" << std::endl;

	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;


	// Read the state of the Security Wizard


	uint32_t u32_secWizard = 0;
	u32_secWizard = 0x8001 + (moduleId << 4);
	u32_secWizard = (u32_secWizard << 16) + 4; // 4 == Subindex for Security Wizard

	unsigned char bSecWizardState = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_secWizard, sizeof(bSecWizardState), &bSecWizardState, &n_bytesRead);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	std::cout << ">>> Security Wizard: " << std::boolalpha << static_cast<bool>(bSecWizardState) << std::endl;
}

void ConfigurationArea::rebootDevice() {
	// MODULETYPE_MISC
	// https://infosys.beckhoff.com/content/1033/devicemanager/263010571.html?id=2359555515732312853 

	// Get all MISC modules
	std::vector<DeviceManager::Module> misc_modules;
	auto is_misc = [](const DeviceManager::Module& m) { return m.ModuleType == MODULETYPE_MISC; };
	std::copy_if(m_modules.begin(), m_modules.end(), std::back_inserter(misc_modules), is_misc);

	if (misc_modules.empty()) {
		std::cout << "Not MISC modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first MISC module
	uint16_t moduleId = misc_modules.front().ModuleId;

	std::cout << ">>> Request reboot" << std::endl;

	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;


	// Reboot the device

	uint32_t u32_reboot = 0;
	u32_reboot = 0xB001 + (moduleId << 4);
	u32_reboot = (u32_reboot << 16) + 1; // SubIndex = 1 as described in https://infosys.beckhoff.com/content/1033/devicemanager/263036171.html
	unsigned char dummy = 0;

	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_reboot, sizeof(dummy), &dummy);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	std::cout << ">>> Reboot Requested" << std::endl;
}