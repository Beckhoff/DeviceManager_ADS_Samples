#ifndef NIC_H
#define NIC_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

namespace DeviceManager {

	class NIC : public ConfigurationArea {
	public:
		NIC(BasicADS& adsClient);

		virtual ~NIC() {};

		NIC(const NIC& other);

		NIC& operator=(const NIC& other);
		NIC& operator[](int idx);

		int32_t getIPv4Address(std::string& IPv4);
		int32_t setIPv4Address(const char IPv4[]);
		uint32_t count();

		static const uint16_t m_moduleType = MODULETYPE_NIC;
		std::vector<uint16_t> m_moduleIds;
		int16_t m_moduleId;
	};
}
#endif
