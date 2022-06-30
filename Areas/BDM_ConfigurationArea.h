#ifndef BDM_CONFIGURATION_AREA_H
#define BDM_CONFIGURATION_AREA_H

#include "BasicADS.h"

void readModules(BasicADS& adsClient);
void readCPU(BasicADS& adsClient, unsigned short moduleId);
void rebootDevice(BasicADS& adsClient, unsigned short moduleId);
void changeIPAddress(BasicADS& adsClient, unsigned short moduleId);
void deleteAdsRoute(BasicADS& adsClient, unsigned short moduleId);

#endif