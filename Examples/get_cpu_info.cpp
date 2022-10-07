#if defined(USE_TWINCAT_ROUTER)
	#include "TC1000_AdsClient.h"
#else
	#include "GenericAdsClient.h"
#endif

#include "cpu.h"
#include <iostream>

int main() {

	static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };

#if defined(USE_TWINCAT_ROUTER)
	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
#else
	static const char remoteIpV4[] = "192.168.1.98";
	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));
#endif
	
	DeviceManager::CPU cpu(*adsClient);

	if (!cpu) {
		std::cerr << "Module not found on target" << std::endl;
		return -1;
	}

	uint32_t frequency = 0;
	cpu.getFrequency(frequency);
	std::cout << ">>> CPU frequency: " << frequency << "MHz" << std::endl;

	uint16_t usage = 0;
	cpu.getUsage(usage);
	std::cout << ">>> CPU usage: " << usage << "%" << std::endl;

	int16_t temperature = 0;
	cpu.getTemp(temperature);
	std::cout << ">>> CPU temperature: " << temperature << " C" << std::endl;
}