#ifndef BDM_GENERAL_AREA_H
#define BDM_GENERAL_AREA_H

#include "BasicADS.h"

class GeneralArea {
public:
	GeneralArea(BasicADS* adsClient);

	virtual ~GeneralArea() {};

	GeneralArea(const GeneralArea& other);

	GeneralArea& operator=(const GeneralArea& other);

	void readDeviceName();

private:
	BasicADS& m_adsClient;
};

#endif