#include "TC1000_AdsClient.h"
#include "BDM_DeviceArea.h"
#include <memory>

int main() {

	static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };

	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));

	DeviceManager::DeviceArea devArea(adsClient.get());
	devArea.getSerialNumber();
}