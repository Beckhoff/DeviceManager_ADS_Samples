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
	std::ofstream current_config(R"(CurrentConfig.tszip)", std::ios::binary);
	configArea.readDeviceFile(R"(C:\TwinCAT\3.1\Boot\CurrentConfig.tszip)", current_config);
	//configArea.readDeviceFile(R"(/usr/local/etc/TwinCAT/3.1/Boot/CurrentConfig.tszip)", current_config);
}