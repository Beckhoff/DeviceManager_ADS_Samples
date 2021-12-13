#include <iostream>
#include <memory>

#include "BasicADS.h"
#include "GenericAdsClient.h"
#include "BDM_GeneralArea.h"
#include "BDM_ConfigurationArea.h"
#include "BDM_DeviceArea.h"


int main(){
	
	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };
	static const char remoteIpV4[] = "192.168.1.102";

	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));

	// General Area
	readDeviceName(*adsClient);

	// Device Area
	readSerialNumber(*adsClient);

	// Configuration Area
	readModules(*adsClient);

	return 0;
}
