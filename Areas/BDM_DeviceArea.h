#ifndef BDM_DEVICE_AREA_H
#define BDM_DEVICE_AREA_H

#include "BasicADS.h"
#include <string>

namespace DeviceManager {

	class DeviceArea {
	public:
		DeviceArea(BasicADS& adsClient);

		virtual ~DeviceArea() {};

		DeviceArea(const DeviceArea& other);

		DeviceArea& operator=(const DeviceArea& other);

		int32_t getSerialNumber(std::string &serialNo);

		static const uint32_t	m_stringBuf = 128;

	private:
		BasicADS& m_adsClient;
	};
}
#endif