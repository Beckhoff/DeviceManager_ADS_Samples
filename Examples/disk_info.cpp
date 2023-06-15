#if defined(USE_TWINCAT_ROUTER)
#include "TC1000_AdsClient.h"
#else
#include "GenericAdsClient.h"
#endif

#include "disk_mgmt.h"
#include "ads_error.h"
#include "ads_exception.h"
#include <iostream>
#include <optional>
#include <vector>

int main() {

	//static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };
	static const AmsNetId remoteNetId{ 192, 168, 1, 122, 1, 1 };
	//static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

#if defined(USE_TWINCAT_ROUTER)
	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
#else
	static const char remoteIpV4[] = "192.168.1.98";
	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));
#endif

	std::optional<DeviceManager::DiskMgmt> disk_mgmt;

	try {
		disk_mgmt.emplace(*adsClient);
	}
	catch (const DeviceManager::AdsException& ex) {
		std::cout << ex.what() << std::endl;
		exit(-1);
	}

	if (!disk_mgmt) {
		std::cerr << "Module not available on target" << std::endl;
		return -1;
	}

	int32_t error = 0;

	std::vector<std::string> volumeLabels;
	error = disk_mgmt->getVolumeLabels(volumeLabels);
	handleError(error);

	for (int i = 0; i < volumeLabels.size(); i++)
	{
		std::cout << "Volume [" << i << "] - " << volumeLabels[i] << std::endl;
	}
}