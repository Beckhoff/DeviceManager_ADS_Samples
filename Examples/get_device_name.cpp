#if defined(USE_TWINCAT_ROUTER)
	#include "TC1000_AdsClient.h"
#else
	#include "GenericAdsClient.h"
#endif

#include "BDM_GeneralArea.h"
#include "ads_error.h"
#include <memory>
#include <string>
#include <iostream>

int main() {

	static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };

#if defined(USE_TWINCAT_ROUTER)
	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
#else
	static const char remoteIpV4[] = "192.168.1.98";
	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));
#endif

	DeviceManager::GeneralArea genArea(*adsClient);

	std::string deviceName;
	int32_t error = genArea.getDeviceName(deviceName);
	handleError(error);
	std::cout << "Device Name: " << deviceName << std::endl;
}