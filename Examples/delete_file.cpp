#if defined(USE_TWINCAT_ROUTER)
	#include "TC1000_AdsClient.h"
#else
	#include "GenericAdsClient.h"
#endif

#include "file_system_object.h"
#include "ads_error.h"
#include <iostream>

int main() {

	static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };

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
	const char* fileName = R"(/usr/local/etc/TwinCAT/3.1/Boot/test.txt)";
	std::cout << "> Delete file/folder \"" << fileName << "\"" << std::endl;
	int32_t error = fso.deleteFile(fileName);
	handleError(error);
}