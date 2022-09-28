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


template<uint16_t ModuleType>
inline std::vector<DeviceManager::Module> ConfigurationArea::get_modules()
{
	std::vector<DeviceManager::Module> modules;
	auto test_module = [](const DeviceManager::Module& m) { return m.ModuleType == ModuleType; };
	std::copy_if(m_modules.begin(), m_modules.end(), std::back_inserter(modules), test_module);
	return modules;
}

void ConfigurationArea::changeIPAddress() {
	// MODULETYPE_NIC
	// https://infosys.beckhoff.com/content/1031/devicemanager/263013131.html

	// Get all NIC modules
	auto nic_modules = get_modules<MODULETYPE_NIC>();
	if (nic_modules.empty()) {
		std::cout << "No NIC modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first NIC module
	uint16_t moduleId = nic_modules.front().ModuleId;
	
	std::cout << "> Changing IP-Address..." << std::endl;

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
	auto twincat_modules = get_modules<MODULETYPE_TWINCAT>();
	if (twincat_modules.empty()) {
		std::cout << "No TWINCAT modules found on device" << std::endl;
		return;
	}

	// Get ModuleId from first TWINCAT module
	uint16_t moduleId = twincat_modules.front().ModuleId;

	char route_name[] = "CX-50C9E8";
	std::cout << "> Delete ADS Route \"" << route_name << "\"" << std::endl; 


	// Deleting ADS Route


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
	auto cpu_modules = get_modules<MODULETYPE_CPU>();
	if (cpu_modules.empty()) {
		std::cout << "No CPU modules found on device" << std::endl;
		return;
	}

	// Get ModuleId from first CPU module
	uint16_t moduleId = cpu_modules.front().ModuleId;

	std::cout << "> Read CPU Information:" << std::endl;

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
	auto misc_modules = get_modules<MODULETYPE_MISC>();
	if (misc_modules.empty()) {
		std::cout << "No MISC modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first MISC module
	uint16_t moduleId = misc_modules.front().ModuleId;

	std::cout << "> Read state of the SecurityWizard:" << std::endl;

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
	auto misc_modules = get_modules<MODULETYPE_MISC>();
	if (misc_modules.empty()) {
		std::cout << "No MISC modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first MISC module
	uint16_t moduleId = misc_modules.front().ModuleId;

	std::cout << "> Request reboot" << std::endl;

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

void ConfigurationArea::deleteFile(const char file_name[], bool bRecursive)
{
	// MODULETYPE_FSO
	// https://infosys.beckhoff.com/content/1031/devicemanager/263000843.html?id=5965980679203448020 

	// Get all FSO modules
	auto misc_modules = get_modules<MODULETYPE_FSO>();
	if (misc_modules.empty()) {
		std::cout << "No File System Object modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first MISC module
	uint16_t moduleId = misc_modules.front().ModuleId;

	std::cout << "> Delete file/folder \"" << file_name << "\"" << std::endl;

	char service_transfer_object[50] = {}; // cbFilename (4 byte), bRecursive (4byte), filename
	size_t file_name_length = strlen(file_name);

	// Copy cbFilename to service transfer object
	*reinterpret_cast<uint32_t*>(service_transfer_object) = file_name_length;
	// Copy bRecursive to service transfer object
	*reinterpret_cast<uint32_t*>(service_transfer_object + 4) = bRecursive;
	// Copy filename to service transfer object
	memcpy(service_transfer_object + 8, file_name, file_name_length);

	uint32_t u32_del_file_idx = 0xB004 + (moduleId << 4);
	u32_del_file_idx = (u32_del_file_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_del_file_idx + 1 /* trigger */, 8 + file_name_length, service_transfer_object);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	// Read state of operation
	char sto_state[6] = {}; // MDP state (1 byte), padding (1 byte), MDP error code (4 byte)

	uint32_t n_bytes_read = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_del_file_idx + 3 /* state & data */, sizeof(sto_state), sto_state, &n_bytes_read);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	if (n_bytes_read > 0) {
		uint8_t mdp_status = *reinterpret_cast<uint8_t*>(sto_state);

		switch (mdp_status) {

		case 0:
		case 1:
			std::cout << ">>> File/folder deleteded successful" << std::endl;
			break;
		case 2:
			std::cerr << ">>> Unspecified error occured" << std::endl;
			break;
		case 3:
			uint32_t mdp_err = *reinterpret_cast<uint32_t*>(sto_state + 2);
			std::cerr << ">>> MDP error: 0x" << std::hex << mdp_err << std::endl;
		}
	}
}

void ConfigurationArea::listFiles(const char folder_name[])
{
	// MODULETYPE_FSO
	// https://infosys.beckhoff.com/content/1031/devicemanager/263000843.html?id=5965980679203448020 

	// Get all FSO modules
	auto misc_modules = get_modules<MODULETYPE_FSO>();
	if (misc_modules.empty()) {
		std::cout << "No File System Object modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first MISC module
	uint16_t moduleId = misc_modules.front().ModuleId;

	std::cout << "> List files/folder in \"" << folder_name << "\"" << std::endl;

	char service_transfer_object[50] = {}; // cbFilename (4 byte), bRecursive (4byte), filename
	size_t folder_name_length = strlen(folder_name);

	// Copy cbFilename to service transfer object
	*reinterpret_cast<uint32_t*>(service_transfer_object) = folder_name_length;
	// Copy folder name to service transfer object
	memcpy(service_transfer_object + 4, folder_name, folder_name_length);

	uint32_t u32_dir_idx = 0xB000 + (moduleId << 4);
	u32_dir_idx = (u32_dir_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_dir_idx + 1 /* trigger */, 8 + folder_name_length, service_transfer_object);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	// Read state and data of operation

	// Create a large buffer because it is not possible to predict the size of data returned by reading the service transfer object
	auto buffer = std::shared_ptr<char[]>(new char[m_large_buf]);

	uint32_t n_bytes_read = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_dir_idx + 3 /* state & data */, m_large_buf, buffer.get(), &n_bytes_read);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	uint8_t mdp_status = *reinterpret_cast<uint8_t*>(buffer.get());

	if (mdp_status == 1) { // No error; data available
		// Ignore the first two MDP state bytes
		char* dir_sdo_data = buffer.get() + 2;

		DeviceManager::TDir dirInfo = *reinterpret_cast<DeviceManager::PTDir>(dir_sdo_data);

		// Iterate over directories
		const char* announce_folders = (dirInfo.cDirs > 0) ? ">> Directories :" : " >> No directories found";
		std::cout << announce_folders << std::endl;
		char* dir_offset = dir_sdo_data + dirInfo.nOffsFirstDir;

		for (int i_dir = 0; i_dir < dirInfo.cDirs; ++i_dir) {
			DeviceManager::TDirInfo dirInfo = *reinterpret_cast<DeviceManager::PTDirInfo>(dir_offset);

			char* pDirName = dir_offset + sizeof(dirInfo); // Move forward to char[]
			std::string sDirName(pDirName, dirInfo.cchName);
			std::cout << ">>> " << sDirName << "/" << std::endl;

			dir_offset = dir_sdo_data + dirInfo.nOffsNextDir; // Move forward to next TDirInfo
		}

		// Iterate over files
		const char* announce_files = (dirInfo.cFiles > 0) ? ">> Files :" : " >> No files found";
		std::cout << announce_files << std::endl;
		char* file_offset = dir_sdo_data + dirInfo.nOffsFirstFile;

		for (int i_files = 0; i_files < dirInfo.cFiles; ++i_files) {
			DeviceManager::TFileInfo fileInfo = *reinterpret_cast<DeviceManager::PTFileInfo>(file_offset);

			char* pFileName = file_offset + sizeof(fileInfo); // Move forward to char[]
			std::string sFileName(pFileName, fileInfo.cchFile);
			std::cout << ">>> " << sFileName << " [size: " << fileInfo.filesize << " byte]" << std::endl;

			file_offset = dir_sdo_data + fileInfo.nOffsNextFile; // Move forward to next TFileInfo
		}

	} else if (mdp_status == 3) { // Error
		uint32_t mdp_err = *reinterpret_cast<uint32_t*>(buffer.get() + 2);
		std::cerr << ">>> MDP error: 0x" << std::hex << mdp_err << std::endl;
	}
}

void ConfigurationArea::readDeviceFile(const char file_name[], std::ostream& local_file)
{
	// MODULETYPE_FSO
	// https://infosys.beckhoff.com/content/1031/devicemanager/263000843.html?id=5965980679203448020 

	// Get all FSO modules
	auto misc_modules = get_modules<MODULETYPE_FSO>();
	if (misc_modules.empty()) {
		std::cout << "No File System Object modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first MISC module
	uint16_t moduleId = misc_modules.front().ModuleId;

	std::cout << "> Read file " << file_name << " from target" << std::endl;

	// STO: cbFilename (4 byte), Continuation handle (4byte), cbMaxRead (4byte), Filename (char[])
	size_t file_name_length = strlen(file_name);
	auto sdo_wBuf = std::shared_ptr<char[]>(new char[12 + file_name_length]);
	char* pWBuf = sdo_wBuf.get();

	DeviceManager::TReadFileIn write_info = {
		file_name_length, // cbFileName
		0, // Continuation handle
		m_cbReadMax
	};
	
	// Copy read info SDO
	*reinterpret_cast<DeviceManager::PTReadFileIn>(pWBuf) = write_info;
	// Copy file name to service transfer object
	pWBuf += sizeof(write_info);
	memcpy(pWBuf, file_name, file_name_length);

	uint32_t u32_rd_idx = 0xB001 + (moduleId << 4);
	u32_rd_idx = (u32_rd_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_rd_idx + 1 /* trigger */, 12 + file_name_length, sdo_wBuf.get());

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	// Read state and data of operation
	// Create a buffer for MDP state (2byte) + CB_MAX_READ
	uint32_t cb_sdo_rBuf = 2 + m_cbReadMax;

	bool bComplete = false;
	bool bErr = false;

	while (!bComplete && !bErr) {
		auto sdo_rBuf = std::shared_ptr<char[]>(new char[cb_sdo_rBuf]);

		uint32_t n_bytes_read = 0;
		n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_rd_idx + 3 /* state & data */, cb_sdo_rBuf, sdo_rBuf.get(), &n_bytes_read);

		if (n_err != ADSERR_NOERR) {
			std::cerr << "Error AdsReadReq: 0x" << std::hex << n_err << std::endl;
			exit(-1);
		}

		uint8_t mdp_status = *reinterpret_cast<uint8_t*>(sdo_rBuf.get());

		if (mdp_status == 1) { // No error; data available
			char* rd_sdo_data = sdo_rBuf.get() + 2;

			DeviceManager::TReadFileOut read_info = *reinterpret_cast<DeviceManager::PTReadFileOut>(rd_sdo_data);
			rd_sdo_data += sizeof(read_info);
			local_file.write(rd_sdo_data, (std::streamsize)read_info.cbData);

			bComplete = read_info.bMoreData;			

			if (!bComplete) {
				// Request more data
				DeviceManager::TReadFileIn sdo_rd_in = {
					0, // cbFileName
					read_info.handle,
					m_cbReadMax
				};

				n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_rd_idx + 1 /* trigger */, sizeof(sdo_rd_in), &sdo_rd_in);

				if (n_err != ADSERR_NOERR) {
					std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
					exit(-1);
				}
			}
		}
		else if (mdp_status == 3) {
			uint32_t mdp_err = *reinterpret_cast<uint32_t*>(sdo_rBuf.get() + 2);
			std::cerr << ">>> MDP error: 0x" << std::hex << mdp_err << std::endl;
			bErr = true;
		}
	}
}

void ConfigurationArea::writeDeviceFile(const char file_name[], std::istream& data)
{
	// MODULETYPE_FSO
	// https://infosys.beckhoff.com/content/1031/devicemanager/263000843.html?id=5965980679203448020 

	// Get all FSO modules
	auto misc_modules = get_modules<MODULETYPE_FSO>();
	if (misc_modules.empty()) {
		std::cout << "No File System Object modules found on device" << std::endl;
		return;
	}
	// Get ModuleId from first MISC module
	uint16_t moduleId = misc_modules.front().ModuleId;

	std::cout << "> Write file " << file_name << " to target" << std::endl;

	// Calcualte size of data:
	data.seekg(0, data.end);
	uint32_t data_length = (uint32_t)data.tellg();
	data.seekg(0, data.beg);

	uint32_t file_name_length = (uint32_t)strlen(file_name);

	// Request write handle first
	DeviceManager::TWriteFileIn write_info = {
		file_name_length, // cbFilename
		0, // Continuation handle
		0, // cbData
		0 // bWriteCompleted
	};

	// Create buffer for the SDO object to write
	uint32_t cbWrite = sizeof(write_info) + file_name_length;// +cbDataWrite;
	auto sdo_wBuf = std::shared_ptr<char[]>(new char[cbWrite]);
	char* pWBuf = sdo_wBuf.get();

	// Copy write info to buffer
	*reinterpret_cast<DeviceManager::PTWriteFileIn>(pWBuf) = write_info;
	// Copy file name to service transfer object
	pWBuf += sizeof(write_info);
	memcpy(pWBuf, file_name, file_name_length);


	uint32_t u32_wrt_idx = 0xB002 + (moduleId << 4);
	u32_wrt_idx = (u32_wrt_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_wrt_idx + 1 /* trigger */, cbWrite, sdo_wBuf.get());

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	// Read write handle and state of operation (MDP status (1byte), padding (1byte), Continuation handle (4 byte))
	uint32_t cb_sdo_rBuf = 6;
	auto sdo_rBuf = std::shared_ptr<char[]>(new char[cb_sdo_rBuf]);

	uint32_t n_bytes_read = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_wrt_idx + 3 /* state & data */, cb_sdo_rBuf, sdo_rBuf.get(), &n_bytes_read);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	uint32_t wrt_hdl = 0;
	uint8_t mdp_status = *reinterpret_cast<uint8_t*>(sdo_rBuf.get());

	if (mdp_status == 1) { // No error; data available
		wrt_hdl = *reinterpret_cast<uint32_t*>(sdo_rBuf.get() + 2);
		int x = 5;
	}
	else if (mdp_status == 3) {
		uint32_t mdp_err = *reinterpret_cast<uint32_t*>(sdo_rBuf.get() + 2);
		std::cerr << ">>> MDP error: 0x" << std::hex << mdp_err << std::endl;
		exit(-1);
	}

	// Calculates the remaining bytes to write
	auto f_remaining = [&]() -> uint32_t { return data_length - (uint32_t)data.tellg(); };

	while (f_remaining() > 0) {

		uint32_t cbDataWrite = (f_remaining() >= m_cbWriteMax) ? m_cbWriteMax : f_remaining();

		DeviceManager::TWriteFileIn write_info = {
			0, // cbFilename
			wrt_hdl, // Continuation handle
			cbDataWrite, // cbData
			(cbDataWrite < m_cbWriteMax) ? 1 : 0 // Set to 1 to indicate the last write access
		};

		// Create buffer for the SDO object to write
		uint32_t cbWrite = sizeof(write_info) + cbDataWrite;// +cbDataWrite;
		auto sdo_wBuf = std::shared_ptr<char[]>(new char[cbWrite]);
		char* pWBuf = sdo_wBuf.get();

		// Copy write info to buffer
		*reinterpret_cast<DeviceManager::PTWriteFileIn>(pWBuf) = write_info;
		// Copy file name to service transfer object
		pWBuf += sizeof(write_info);

		// Copy data to SDO
		data.read(pWBuf, cbDataWrite);

		n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_wrt_idx + 1 /* trigger */, cbWrite, sdo_wBuf.get());

		if (n_err != ADSERR_NOERR) {
			std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
			exit(-1);
		}

		// Read state of write operation
		char rd_state_buf[6] = {};
		n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_wrt_idx + 3 /* state & data */, sizeof(rd_state_buf), rd_state_buf, &n_bytes_read);

		if (n_err != ADSERR_NOERR) {
			std::cerr << "Error AdsReadReq: 0x" << std::hex << n_err << std::endl;
			exit(-1);
		}

		uint8_t mdp_status = *reinterpret_cast<uint8_t*>(rd_state_buf);

		if (mdp_status == 3) {
			uint32_t mdp_err = *reinterpret_cast<uint32_t*>(sdo_rBuf.get() + 2);
			std::cerr << ">>> MDP error: 0x" << std::hex << mdp_err << std::endl;
			exit(-1);
		}
	}
}
