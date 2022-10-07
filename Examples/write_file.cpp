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

	DeviceManager::FileSystemObject fso(*adsClient);

	if (!fso) {
		std::cerr << "Module not found on target" << std::endl;
		return -1;
	}

	const char* source_file = R"(C:\TwinCAT\3.1\Target\StaticRoutes.xml)";
	const char* target_file = R"(C:\TwinCAT\3.1\Boot\TestRoutes.xml)";
	std::ifstream tmc_file(source_file, std::ios::binary);
	fso.writeDeviceFile(target_file, tmc_file);
}