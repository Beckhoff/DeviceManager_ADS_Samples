#include "TC1000_AdsClient.h"
#include "BDM_ConfigurationArea.h"

int main() {

	static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };

	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));

	DeviceManager::ConfigurationArea configArea(adsClient.get());
	configArea.deleteFile(R"(/usr/local/etc/TwinCAT/3.1/Boot/test.txt)");
}