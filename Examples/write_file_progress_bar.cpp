#if defined(USE_TWINCAT_ROUTER)
#include "TC1000_AdsClient.h"
#else
#include "GenericAdsClient.h"
#endif

#include "file_system_object.h"
#include "ads_error.h"
#include "ads_exception.h"
#include <iostream>
#include <optional>

void progressBar(int progress)
{
	int barWidth = 70;
	std::cout << "[";
	int pos = barWidth * progress / 100;
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] " << int(progress) << " %\r";
	std::cout.flush();
}


int main() {

	//static const AmsNetId remoteNetId{ 5, 80, 201, 232, 1, 1 };
	static const AmsNetId remoteNetId{ 5, 69, 55, 236, 1, 1 };

#if defined(USE_TWINCAT_ROUTER)
	auto adsClient = std::shared_ptr<BasicADS>(new TC1000AdsClient(remoteNetId));
#else
	static const char remoteIpV4[] = "192.168.1.98";
	auto adsClient = std::shared_ptr<BasicADS>(new GenericAdsClient(remoteNetId, remoteIpV4));
#endif

	std::optional<DeviceManager::FileSystemObject> fso;

	try {
		fso.emplace(*adsClient);
	}
	catch (const DeviceManager::AdsException& ex) {
		std::cout << ex.what() << std::endl;
		exit(-1);
	}

	if (!fso) {
		std::cerr << "Module not available on target" << std::endl;
		return -1;
	}

	const char* sourceFile = R"(C:\TwinCAT\AdsApi\TcAdsDll\TcAdsDll.dll)";
	const char* targetFile = R"(C:\TwinCAT\AdsApi\TcAdsDll\TcAdsDll.dll_from_XAE)";

	std::cout << "> Write " << sourceFile << " to " << targetFile << " on target" << std::endl;

	size_t bytesWritten = 0;
	std::ifstream tmc_file(sourceFile, std::ios::binary);
	int32_t error = fso->writeDeviceFile(targetFile, tmc_file, bytesWritten, progressBar);
	handleError(error);
}