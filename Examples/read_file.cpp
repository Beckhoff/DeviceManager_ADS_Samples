#if defined(USE_TWINCAT_ROUTER)
	#include "TC1000_AdsClient.h"
#else
	#include "GenericAdsClient.h"
#endif

#include "file_system_object.h"
#include <iostream>

int main() {

	//static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };
	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

#if defined(USE_TWINCAT_ROUTER)
	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
#else
	static const char remoteIpV4[] = "192.168.1.98";
	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));
#endif

	DeviceManager::FileSystemObject fso(adsClient.get());

	if (!fso) {
		std::cerr << "Module not found on target" << std::endl;
		return -1;
	}
	std::ofstream current_config(R"(CurrentConfig.tszip)", std::ios::binary);
	fso.readDeviceFile(R"(C:\TwinCAT\3.1\Boot\CurrentConfig.tszip)", current_config);
	//fso.readDeviceFile(R"(/usr/local/etc/TwinCAT/3.1/Boot/CurrentConfig.tszip)", current_config);
}