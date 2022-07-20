#ifndef BDM_CONFIGURATION_AREA_H
#define BDM_CONFIGURATION_AREA_H

#include <vector>

#include "BasicADS.h"

namespace DeviceManager {
	struct Module {
		uint16_t ModuleType;
		uint16_t ModuleId;
	};
}



class ConfigurationArea {
public:
	ConfigurationArea(BasicADS* adsClient);
		
	virtual ~ConfigurationArea() {};

	ConfigurationArea(const ConfigurationArea& other);

	ConfigurationArea& operator=(const ConfigurationArea& other);

	void changeIPAddress();
	void deleteAdsRoute();
	void readCPU();
	void readStateSecurityWizard();
	void rebootDevice();

private:

	std::vector<DeviceManager::Module> m_modules;
	BasicADS& m_adsClient;
};

#endif