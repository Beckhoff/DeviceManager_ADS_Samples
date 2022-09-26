#ifndef BDM_CONFIGURATION_AREA_H
#define BDM_CONFIGURATION_AREA_H

#include <vector>

#include "BasicADS.h"

namespace DeviceManager {
	struct Module {
		uint16_t ModuleType;
		uint16_t ModuleId;
	};

#pragma pack ( push, 1 )
	typedef struct _tagTDirInfo
	{
		uint32_t nOffsNextDir;
		uint32_t cchName;
		//char[cchName] szName
	} TDirInfo, * PTDirInfo;

	typedef struct _tagTFileInfo
	{
		uint32_t	nOffsNextFile;
		__int64		filesize;
		uint32_t	attribs;
		uint32_t	cchFile;
		//char[File] szName
	} TFileInfo, * PTFileInfo;
#pragma pack ( pop )
}



class ConfigurationArea {
public:
	ConfigurationArea(BasicADS* adsClient);
		
	virtual ~ConfigurationArea() {};

	ConfigurationArea(const ConfigurationArea& other);

	ConfigurationArea& operator=(const ConfigurationArea& other);

	void changeIPAddress();
	void deleteAdsRoute();
	void readCPU();
	void readStateSecurityWizard();
	void rebootDevice();
	void deleteFile(char file_name[], bool bRecursive = false);
	void listFiles(char folder_name[]);

private:

	template <uint16_t ModuleType> std::vector<DeviceManager::Module> get_modules();

	std::vector<DeviceManager::Module> m_modules;
	BasicADS& m_adsClient;
};

#endif