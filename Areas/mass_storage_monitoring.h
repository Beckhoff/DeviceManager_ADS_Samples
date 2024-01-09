#ifndef MASS_STORAGE_MONITORING_H
#define MASS_STORAGE_MONITORING_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"
#include <map>

namespace DeviceManager {

	constexpr std::string_view DriveTypes[] =
	{
		"Unknown",
		"Harddisk",
		"2,5\" SSD",
		"CFast",
		"CF",
		"M.2 SATA SSD",
		"M.2 NVMe SSD"
	};

	class MassStorageMonitoring : public ConfigurationArea {
	public:
		MassStorageMonitoring(BasicADS& adsClient);

		virtual ~MassStorageMonitoring() {};

		MassStorageMonitoring(const MassStorageMonitoring& other);

		MassStorageMonitoring& operator=(const MassStorageMonitoring& other);

		int32_t getDriveTypes(std::vector<uint8_t>& driveTypes);

		static const uint16_t m_moduleType = MODULETYPE_MASSSTORAGEMONITORING;
		int16_t m_moduleId;
	};
}

#endif