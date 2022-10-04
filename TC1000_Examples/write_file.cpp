#include "TC1000_AdsClient.h"
#include "BDM_ConfigurationArea.h"

int main() {

	//static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };
	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));

	DeviceManager::ConfigurationArea configArea(adsClient.get());
	const char* source_file = R"(C:\TwinCAT\3.1\Target\StaticRoutes.xml)";
	const char* target_file = R"(C:\TwinCAT\3.1\Boot\TestRoutes.xml)";
	std::ifstream tmc_file(source_file, std::ios::binary);
	configArea.writeDeviceFile(target_file, tmc_file);
}