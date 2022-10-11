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

	const char* source = R"(C:\TwinCAT\3.1\Boot\CurrentConfig.tszip)";
	const char* dest = R"(C:\TwinCAT\3.1\CurrentConfig_backup.tszip)";

	std::cout << "> Copy file from " << source << " to " << dest << std::endl;
	//const char* source = R"(/usr/local/etc/TwinCAT/3.1/Boot/CurrentConfig.tszip)";
	//const char* dest = R"(/usr/local/etc/TwinCAT/3.1/CurrentConfig_backup.tszip)";
	int32_t error = fso->copyDeviceFile(source, dest, 1);
	handleError(error);
}
