#ifndef BDM_GENERAL_AREA_H
#define BDM_GENERAL_AREA_H

#include "BasicADS.h"
#include <string>

namespace DeviceManager {

	class GeneralArea {
	public:
		GeneralArea(BasicADS& adsClient);

		virtual ~GeneralArea() {};

		GeneralArea(const GeneralArea& other);

		GeneralArea& operator=(const GeneralArea& other);

		int32_t getDeviceName(std::string &deviceName);

		static const uint32_t	m_cbStringBuf = 128;

	private:
		BasicADS& m_adsClient;
	};
}
#endif