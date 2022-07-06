#include <iostream>
#include <memory>

#include "BasicADS.h"
#include "TC1000_AdsClient.h"
#include "BDM_GeneralArea.h"
#include "BDM_ConfigurationArea.h"
#include "BDM_DeviceArea.h"


int main() {

	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
	
	ConfigurationArea configArea(adsClient.get());
	configArea.readModules();

	GeneralArea genArea(adsClient.get());
	genArea.readDeviceName();

	DeviceArea devArea(adsClient.get());
	devArea.readSerialNumber();
}