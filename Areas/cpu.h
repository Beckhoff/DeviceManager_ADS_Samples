#ifndef CPU_H
#define CPU_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

namespace DeviceManager {

	class CPU : public ConfigurationArea {
	public:
		CPU(BasicADS& adsClient);

		virtual ~CPU() {};

		CPU(const CPU& other);

		CPU& operator=(const CPU& other);

		int32_t getFrequency(uint32_t& freq);
		int32_t getUsage(uint16_t& usage);
		int32_t getTemp(int16_t& temp);

		static const uint16_t m_moduleType = MODULETYPE_CPU;
		int16_t m_moduleId;
	};
}
#endif