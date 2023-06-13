#ifndef DISK_MGMT_H
#define DISK_MGMT_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

namespace DeviceManager {

	class DiskMgmt : public ConfigurationArea {
	public:
		DiskMgmt(BasicADS& adsClient);

		virtual ~DiskMgmt() {};

		DiskMgmt(const DiskMgmt& other);

		DiskMgmt& operator=(const DiskMgmt& other);
		DiskMgmt& operator[](int idx);

		uint32_t count();

		static const uint16_t m_moduleType = MODULETYPE_DISKMGMT;
		std::vector<uint16_t> m_moduleIds;
		int16_t m_moduleId;
	};
}

#endif