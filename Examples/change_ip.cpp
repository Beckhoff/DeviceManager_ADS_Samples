#if defined(USE_TWINCAT_ROUTER)
	#include "TC1000_AdsClient.h"
#else
	#include "GenericAdsClient.h"
#endif

#include "nic.h"
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

	std::optional<DeviceManager::NIC> nic;

	try {
		nic.emplace(*adsClient);
	}
    catch (const DeviceManager::AdsException &ex) {
		std::cout << ex.what() << std::endl;
		exit(-1);
	}

	if (!nic) {
		std::cerr << "Module not available on target" << std::endl;
		return -1;
	}

	uint32_t nNICs = nic->count();
	std::cout << nNICs << " NICs available on target" << std::endl;

	int32_t error = 0;

	for (uint32_t i = 0; i < nic->count(); i++) {
		std::string sIpv4Addr;
		error = (*nic)[i].getIPv4Address(sIpv4Addr);
		handleError(error);

		std::cout << "Interface_" << i << " - " << sIpv4Addr << std::endl;

		// Change address
		if (sIpv4Addr == "192.168.3.102") {
			const char* newAddr = "192.168.3.103";
			std::cout << ">>> Change " << sIpv4Addr << " to " << newAddr << std::endl;
			error = (*nic)[i].setIPv4Address(newAddr);
			handleError(error);
		}
	}
}
