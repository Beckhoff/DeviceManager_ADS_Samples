#ifndef CPU_H
#define CPU_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

#include <fstream>

namespace DeviceManager {

	class CPU : public ConfigurationArea {
	public:
		CPU(BasicADS& adsClient);

		virtual ~CPU() {};

		CPU(const CPU& other);

		CPU& operator=(const CPU& other);

		void getFrequency(uint32_t& freq);
		void getUsage(uint16_t& usage);
		void getTemp(int16_t& temp);

		static const uint16_t m_moduleType = MODULETYPE_CPU;
		int16_t m_moduleId;
	};
}
#endif