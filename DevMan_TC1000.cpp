#include <iostream>
#include <memory>
#include <filesystem>

#include "BasicADS.h"
#include "TC1000_AdsClient.h"
#include "BDM_GeneralArea.h"
#include "BDM_ConfigurationArea.h"
#include "BDM_DeviceArea.h"

int main() {

	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };
	//static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };

	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
	
	ConfigurationArea configArea(adsClient.get());
	configArea.readCPU();
	//configArea.rebootDevice();
	//configArea.changeIPAddress();
	//configArea.deleteAdsRoute("CX-50C9E8");
	//configArea.deleteFile(R"(C:\test.txt)");
	//configArea.listFiles(R"(C:\TwinCAT\3.1\Boot\*)");

	
	//configArea.listFiles(R"(/usr/local/etc/TwinCAT/3.1/Boot/*)"); // TwinCAT/BSD
	//configArea.mkdir(R"(C:\TwinCAT\3.1\Boot\NewFolder)", false);
	//configArea.mkdir(R"(/usr/local/etc/TwinCAT/3.1/Boot/NewFolder)", false); // TwinCAT/BSD

	// Read file from target
	//std::ofstream current_config(R"(CurrentConfig.tszip)", std::ios::binary);
	//configArea.readDeviceFile(R"(C:\TwinCAT\3.1\Boot\CurrentConfig.tszip)", current_config);
	
	// Write file to target
	//const char* source_file = R"(C:\TwinCAT\3.1\Target\StaticRoutes.xml)";
	//const char* target_file = R"(C:\TwinCAT\3.1\Boot\TestRoutes.xml)";
	//std::ifstream tmc_file(source_file, std::ios::binary);
	//configArea.writeDeviceFile(target_file, tmc_file);

	// Copy file on target
	//const char* source = R"(C:\TwinCAT\3.1\Boot\CurrentConfig.tszip)";
	//const char* dest = R"(C:\TwinCAT\3.1\CurrentConfig_backup.tszip)";
	//const char* source = R"(/usr/local/etc/TwinCAT/3.1/Boot/CurrentConfig.tszip)";
	//const char* dest = R"(/usr/local/etc/TwinCAT/3.1/CurrentConfig_backup.tszip)";
	//configArea.copyDeviceFile(source, dest, 1);


	GeneralArea genArea(adsClient.get());
	genArea.readDeviceName();

	DeviceArea devArea(adsClient.get());
	devArea.readSerialNumber();
}