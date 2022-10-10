#include "BDM_ConfigurationArea.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <iostream>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <assert.h>

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

using namespace DeviceManager;

ConfigurationArea::ConfigurationArea(BasicADS& adsClient)
	: m_adsClient(adsClient)
	, m_bModuleExists(false)
	{

	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;

	// Read length of list of module IDs https://infosys.beckhoff.com/content/1031/devicemanager/45035996536742667.html?id=5503267175110745821
	uint16_t u16_len_module_id_list = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, MDP_IDX_OFFS_DEVICE_AREA, sizeof(u16_len_module_id_list), &u16_len_module_id_list, &n_bytesRead);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl; // TODO Throw exception
		exit(-1);
	}

	// Download table of module IDs (Configuration Area)
	for (int i = 1; i < u16_len_module_id_list; ++i) {

		uint32_t  u32_module_entry = 0;
		uint32_t indexOffset = MDP_IDX_OFFS_DEVICE_AREA + i;
		n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, indexOffset, sizeof(u32_module_entry), &u32_module_entry, &n_bytesRead);

		if (n_err != ADSERR_NOERR) {
			std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
			break;
		}

		DeviceManager::Module module = {
			static_cast<uint16_t>(u32_module_entry >> 16), // ModuleType
			static_cast<uint16_t>(u32_module_entry & 0xFF) // ModuleId
		};

		m_modules.push_back(module);
	}
};

ConfigurationArea::ConfigurationArea(const ConfigurationArea& other)
	: m_adsClient(other.m_adsClient)
	, m_bModuleExists(other.m_bModuleExists)
	, m_modules(other.m_modules) {};

ConfigurationArea& ConfigurationArea::operator=(const ConfigurationArea& other){
	m_adsClient	= other.m_adsClient;
	m_modules	= other.m_modules;
	return *this;
}

bool ConfigurationArea::operator!() const {
	return !m_bModuleExists;
}

template<uint16_t ModuleType>
inline std::vector<DeviceManager::Module> ConfigurationArea::get_modules()
{
	std::vector<DeviceManager::Module> modules;
	auto test_module = [](const DeviceManager::Module& m) { return m.ModuleType == ModuleType; };
	std::copy_if(m_modules.begin(), m_modules.end(), std::back_inserter(modules), test_module);
	return modules;
}

uint16_t DeviceManager::ConfigurationArea::getFirstModuleId(uint16_t moduleType)
{

	auto testId = [moduleType](const DeviceManager::Module& m) { return m.ModuleType == moduleType; };
	auto first = std::find_if(m_modules.begin(), m_modules.end(), testId);

	if (first != m_modules.end()) {
		return first->ModuleId;
	}

	return -1;
}

int32_t ConfigurationArea::getStoStateInfo(uint32_t index)
{
	std::shared_ptr<char[]> buf;
	return getStoStateInfo(index, 4, buf);
}

int32_t ConfigurationArea::getStoStateInfo(uint32_t index, uint32_t cbBuf, std::shared_ptr<char[]> &buf)
{
	uint32_t result = 0;
	uint32_t n_bytes_read = 0;
	// Increase cbBuf +2 to for MDP status and padding
	cbBuf += 2;
	// Read at lest state of operation (MDP status (1byte), padding (1byte), MDP error code (4byte))
	cbBuf = (cbBuf < m_cbBufMin) ? m_cbBufMin : cbBuf; // set to minimum 6 bytes

	buf = std::shared_ptr<char[]>(new char[cbBuf]);

	result = m_adsClient.AdsReadReq(MDP_IDX_GRP, index + 3 /* state & data */, cbBuf, buf.get(), &n_bytes_read);

	if (result != ADSERR_NOERR) {
		#ifndef NDEBUG
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << result << std::endl;
		#endif
		return result;
	}

	uint8_t mdp_status = *reinterpret_cast<uint8_t*>(buf.get());

	switch (mdp_status) {

	case 0:
		break;
	case 1:
		// Advance pointer to actual data area
		buf = std::shared_ptr<char[]>(buf, (char*)(buf.get() + 2));
		break;
	case 2:
		result = 0xECA60001; // Unspecified error
#ifndef NDEBUG
		std::cerr << ">>> MDP error: 0x" << std::hex << result << std::endl;
#endif
		break;
	case 3:
		result = *reinterpret_cast<uint32_t*>(buf.get() + 2);
#ifndef NDEBUG
		std::cerr << ">>> MDP error: 0x" << std::hex << result << std::endl;
#endif
		break;
	}

	return result;
}