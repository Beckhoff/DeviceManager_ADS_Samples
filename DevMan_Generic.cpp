#include <iostream>
#include <memory>

#include "BasicADS.h"
#include "GenericAdsClient.h"
#include "BDM_GeneralArea.h"
#include "BDM_ConfigurationArea.h"
#include "BDM_DeviceArea.h"


int main(){
	
	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };
	static const char remoteIpV4[] = "192.168.1.102";

	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));

	ConfigurationArea configArea(adsClient.get());
	configArea.readCPU();
	//configArea.rebootDevice();
	//configArea.changeIPAddress();
	//configArea.deleteAdsRoute();
	//configArea.deleteFile(R"(C:\test)");
	configArea.listFiles(R"(C:\TwinCAT\3.1\Boot\*)");
	//configArea.listFiles(R"(/usr/local/etc/TwinCAT/3.1/Boot/*)"); // TwinCAT/BSD

	GeneralArea genArea(adsClient.get());
	genArea.readDeviceName();

	DeviceArea devArea(adsClient.get());
	devArea.readSerialNumber();

	return 0;
}
