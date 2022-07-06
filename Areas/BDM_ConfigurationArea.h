#ifndef BDM_CONFIGURATION_AREA_H
#define BDM_CONFIGURATION_AREA_H

#include "BasicADS.h"

class ConfigurationArea {
public:
	ConfigurationArea(BasicADS* adsClient);
		
	virtual ~ConfigurationArea() {};

	ConfigurationArea(const ConfigurationArea& other);

	ConfigurationArea& operator=(const ConfigurationArea& other);

	void readModules();

private:
	void readCPU(unsigned short moduleId);
	void rebootDevice(unsigned short moduleId);
	void changeIPAddress(unsigned short moduleId);
	void deleteAdsRoute(unsigned short moduleId);

	BasicADS& m_adsClient;
};

#endif