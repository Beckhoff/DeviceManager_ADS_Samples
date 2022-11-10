#if defined(USE_TWINCAT_ROUTER)
#include "TC1000_AdsClient.h"
#else
#include "GenericAdsClient.h"
#endif

#include "mainboard.h"
#include "ads_error.h"
#include "ads_exception.h"
#include <iostream>
#include <optional>

int main() {

	//static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };
	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

#if defined(USE_TWINCAT_ROUTER)
	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
#else
	static const char remoteIpV4[] = "192.168.1.98";
	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));
#endif

	std::optional<DeviceManager::Mainboard> mainboard;

	try {
		mainboard.emplace(*adsClient);
	}
	catch (const DeviceManager::AdsException& ex) {
		std::cout << ex.what() << std::endl;
		exit(-1);
	}

	if (!mainboard) {
		std::cerr << "Module not available on target" << std::endl;
		return -1;
	}

	int32_t error = 0;

	std::string serialNumber;
	error = mainboard->getSerialNumber(serialNumber);
	handleError(error);
	std::cout << "Mainboard serial number: " << serialNumber << std::endl;


	int32_t minTemp = 0;
	error = mainboard->getMinTemp(minTemp);
	handleError(error);
	std::cout << "Minimal mainboard temperature: " << minTemp << " C" << std::endl;

	int32_t maxTemp = 0;
	error = mainboard->getMaxTemp(maxTemp);
	handleError(error);
	std::cout << "Maximal mainboard temperature: " << maxTemp << " C" << std::endl;

	int16_t temp = 0;
	error = mainboard->getTemp(temp);
	handleError(error);
	std::cout << "Current mainboard temperature: " << temp << " C" << std::endl;
}