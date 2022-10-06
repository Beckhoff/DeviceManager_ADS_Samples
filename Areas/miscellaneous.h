#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

#include <fstream>

namespace DeviceManager {

	class Miscellaneous : public ConfigurationArea {
	public:
		Miscellaneous(BasicADS* adsClient);

		virtual ~Miscellaneous() {};

		Miscellaneous(const Miscellaneous& other);

		Miscellaneous& operator=(const Miscellaneous& other);

		void rebootDevice();
		void readStateSecurityWizard();

		static const uint16_t m_moduleType = MODULETYPE_MISC;
		int16_t m_moduleId;
	};
}
#endif