#ifndef TWINCAT_H
#define TWINCAT_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

#include <fstream>

namespace DeviceManager {

	class TwinCAT : public ConfigurationArea {
	public:
		TwinCAT(BasicADS& adsClient);

		virtual ~TwinCAT() {};

		TwinCAT(const TwinCAT& other);

		TwinCAT& operator=(const TwinCAT& other);

		int32_t deleteAdsRoute(const char route_name[]);

		static const uint16_t m_moduleType = MODULETYPE_TWINCAT;
		int16_t m_moduleId;
	};
}
#endif