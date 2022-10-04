#include "TC1000_AdsClient.h"
#include "BDM_ConfigurationArea.h"

int main() {

	static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };
	//static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));

	DeviceManager::ConfigurationArea configArea(adsClient.get());

	//const char* source = R"(C:\TwinCAT\3.1\Boot\CurrentConfig.tszip)";
	//const char* dest = R"(C:\TwinCAT\3.1\CurrentConfig_backup.tszip)";
	const char* source = R"(/usr/local/etc/TwinCAT/3.1/Boot/CurrentConfig.tszip)";
	const char* dest = R"(/usr/local/etc/TwinCAT/3.1/CurrentConfig_backup.tszip)";
	configArea.copyDeviceFile(source, dest, 1);
}