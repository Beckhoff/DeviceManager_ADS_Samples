#if defined(USE_TWINCAT_ROUTER)
	#include "TC1000_AdsClient.h"
#else
	#include "GenericAdsClient.h"
#endif

#include "file_system_object.h"
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

	std::optional<DeviceManager::FileSystemObject> fso;

	try {
		fso.emplace(*adsClient);
	}
	catch (const DeviceManager::AdsException& ex) {
		std::cout << ex.what() << std::endl;
		exit(-1);
	}

	if (!fso) {
		std::cerr << "Module not available on target" << std::endl;
		return -1;
	}

	const char* folderName = R"(C:\TwinCAT\3.1\Boot\NewFolder)";
	//const char* folderName = R"(/usr/local/etc/TwinCAT/3.1/Boot/NewFolder)";
	std::cout << "> Create new folder: " << folderName << std::endl;

	int32_t error = fso->mkdir(folderName, false);
	handleError(error);
}
