#include <iostream>
#include <memory>

#include "BasicADS.h"
#include "TC1000_AdsClient.h"
#include "BDM_GeneralArea.h"
#include "BDM_ConfigurationArea.h"
#include "BDM_DeviceArea.h"


int main() {

	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
	
	ConfigurationArea configArea(adsClient.get());
	configArea.readCPU();
	//configArea.rebootDevice();
	//configArea.changeIPAddress();
	//configArea.deleteAdsRoute();
	//configArea.deleteFile(R"(C:\test)");
	//configArea.listFiles(R"(C:\TwinCAT\3.1\Boot\*)");
	//configArea.listFiles(R"(/usr/local/etc/TwinCAT/3.1/Boot/*)"); // TwinCAT/BSD

	//std::ofstream current_config("CurrentConfig.tszip", std::ios::binary);
	//configArea.readDeviceFile(R"(C:\TwinCAT\3.1\Boot\CurrentConfig.tszip)", current_config);

	GeneralArea genArea(adsClient.get());
	genArea.readDeviceName();

	DeviceArea devArea(adsClient.get());
	devArea.readSerialNumber();
}