#ifndef BDM_CONFIGURATION_AREA_H
#define BDM_CONFIGURATION_AREA_H

#include "BasicADS.h"

#include <cstdint>
#include <vector>
#include <fstream>
#include <memory>

namespace DeviceManager {

		struct Module {
			uint16_t ModuleType;
			uint16_t ModuleId;
		};

	class ConfigurationArea {

	public:
		bool operator!() const;

	protected:
		ConfigurationArea(BasicADS& adsClient);
		
		virtual ~ConfigurationArea() {};

		ConfigurationArea(const ConfigurationArea& other);

		ConfigurationArea& operator=(const ConfigurationArea& other);

		static const uint32_t	m_large_buf		= 65536;
		static const uint32_t	m_cbWriteMax	= 1024;
		static const uint32_t	m_cbReadMax		= 1024;
		static const uint32_t	m_cbStringBuf	= 128;
		static const uint32_t   m_cbBufMin		= 6;
		

		int32_t		getStoStateInfo(uint32_t index);
		int32_t		getStoStateInfo(uint32_t index,	uint32_t cbBuf, std::shared_ptr<char[]> &buf);
		uint16_t	getFirstModuleId(uint16_t moduleType);

		std::vector<uint16_t> getModuleIds(uint16_t moduleType);
		//template <uint16_t ModuleType> std::vector<uint16_t> getModuleIds();
		//template <uint16_t ModuleType> std::vector<DeviceManager::Module> get_modules();

		std::vector<DeviceManager::Module> m_modules;

		BasicADS& m_adsClient;
		bool	  m_bModuleExists; // == true if one or more modules are available
	};
}
#endif
