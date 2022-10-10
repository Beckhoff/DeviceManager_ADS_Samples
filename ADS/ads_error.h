#ifndef ADS_ERROR_H
#define ADS_ERROR_H

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

#ifndef NDEBUG
#include <iostream>
#endif

void handleResult(int32_t result) {
	if (result != ADSERR_NOERR) {
#ifndef NDEBUG
		std::cerr << "Error occurred: 0x" << std::hex << result << std::endl;
#endif
		exit(-1);
	}
}
#endif