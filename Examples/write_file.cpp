#if defined(USE_TWINCAT_ROUTER)
	#include "TC1000_AdsClient.h"
#else
	#include "GenericAdsClient.h"
#endif

#include "BDM_ConfigurationArea.h"

int main() {

	static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };

#if defined(USE_TWINCAT_ROUTER)
	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
#else
	static const char remoteIpV4[] = "192.168.1.98";
	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));
#endif

	DeviceManager::ConfigurationArea configArea(adsClient.get());
	const char* source_file = R"(C:\TwinCAT\3.1\Target\StaticRoutes.xml)";
	const char* target_file = R"(C:\TwinCAT\3.1\Boot\TestRoutes.xml)";
	std::ifstream tmc_file(source_file, std::ios::binary);
	configArea.writeDeviceFile(target_file, tmc_file);
}