#ifndef BDM_GENERAL_AREA_H
#define BDM_GENERAL_AREA_H

#include "BasicADS.h"

namespace DeviceManager {

	class GeneralArea {
	public:
		GeneralArea(BasicADS& adsClient);

		virtual ~GeneralArea() {};

		GeneralArea(const GeneralArea& other);

		GeneralArea& operator=(const GeneralArea& other);

		void getDeviceName();

	private:
		BasicADS& m_adsClient;
	};
}
#endif