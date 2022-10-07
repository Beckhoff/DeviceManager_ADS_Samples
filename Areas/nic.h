#ifndef NIC_H
#define NIC_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

#include <fstream>

namespace DeviceManager {

	class NIC : public ConfigurationArea {
	public:
		NIC(BasicADS& adsClient);

		virtual ~NIC() {};

		NIC(const NIC& other);

		NIC& operator=(const NIC& other);

		void changeIPAddress();

		static const uint16_t m_moduleType = MODULETYPE_NIC;
		int16_t m_moduleId;
	};
}
#endif