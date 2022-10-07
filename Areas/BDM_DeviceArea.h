#ifndef BDM_DEVICE_AREA_H
#define BDM_DEVICE_AREA_H

#include "BasicADS.h"

namespace DeviceManager {

	class DeviceArea {
	public:
		DeviceArea(BasicADS& adsClient);

		virtual ~DeviceArea() {};

		DeviceArea(const DeviceArea& other);

		DeviceArea& operator=(const DeviceArea& other);

		void getSerialNumber();

	private:
		BasicADS& m_adsClient;
	};
}
#endif