#if defined(USE_TWINCAT_ROUTER)
#include "TC1000_AdsClient.h"
#else
#include "GenericAdsClient.h"
#endif

#include "twincat.h"
#include "ads_error.h"
#include "ads_exception.h"
#include <iostream>
#include <optional>

int main() {

	static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };
	//static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

#if defined(USE_TWINCAT_ROUTER)
	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
#else
	static const char remoteIpV4[] = "192.168.1.98";
	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));
#endif

	std::optional<DeviceManager::TwinCAT> twincat;

	try {
		twincat.emplace(*adsClient);
	}
	catch (const DeviceManager::AdsException& ex) {
		std::cout << ex.what() << std::endl;
		exit(-1);
	}

	if (!twincat) {
		std::cerr << "Module not available on target" << std::endl;
		return -1;
	}

	int32_t error = 0;

	uint16_t major = 0;
	error = twincat->getTcMajor(major);
	handleError(error);

	uint16_t minor = 0;
	error = twincat->getTcMinor(minor);
	handleError(error);

	uint16_t build = 0;
	error = twincat->getTcBuild(build);
	handleError(error);

	std::cout << "TwinCAT version on target: " << major << "." << minor << "." << build << std::endl;
}