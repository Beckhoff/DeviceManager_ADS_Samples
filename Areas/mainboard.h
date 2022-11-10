#ifndef MAINBOARD_H
#define MAINBOARD_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

namespace DeviceManager {

	class Mainboard : public ConfigurationArea {
	public:
		Mainboard(BasicADS& adsClient);

		virtual ~Mainboard() {};

		Mainboard(const Mainboard& other);

		Mainboard& operator=(const Mainboard& other);

		int32_t getSerialNumber(std::string& serialNumber);
		int32_t getMinTemp(int32_t& temp);
		int32_t getMaxTemp(int32_t& temp);
		int32_t getTemp(int16_t& temp);

		static const uint16_t m_moduleType = MODULETYPE_MAINBOARD;
		int16_t m_moduleId;
	};
}
#endif