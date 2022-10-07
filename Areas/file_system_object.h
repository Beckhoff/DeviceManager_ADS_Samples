#ifndef FILE_SYSTEM_OBJECT_H
#define FILE_SYSTEM_OBJECT_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

#include <fstream>

namespace DeviceManager {

#pragma pack ( push, 1 )

	typedef struct _tagTDir
	{
		uint32_t nVersion;
		uint32_t cb;
		uint32_t cDirs;
		uint32_t cFiles;
		uint32_t nOffsFirstDir;
		uint32_t nOffsFirstFile;
	} TDir, * PTDir;

	typedef struct _tagTDirInfo
	{
		uint32_t nOffsNextDir;
		uint32_t cchName;
		//char[cchName] szName
	} TDirInfo, * PTDirInfo;

	typedef struct _tagTFileInfo
	{
		uint32_t	nOffsNextFile;
		int64_t		filesize;
		uint32_t	attribs;
		uint32_t	cchFile;
		//char[File] szName
	} TFileInfo, * PTFileInfo;
#pragma pack ( pop )

	typedef struct _tagReadFileOut
	{
		uint32_t	cbData;
		uint32_t	handle;
		bool		bMoreData;
		// BYTE[cbData]
	} TReadFileOut, * PTReadFileOut;

	typedef struct _tagReadFileIn
	{
		uint32_t	cbFileName;
		uint32_t	handle;
		uint32_t	cbMaxRead;
		// BYTE[cbData]
	} TReadFileIn, * PTReadFileIn;

	typedef struct _tagWriteFileIn
	{
		uint32_t	cbFileName;
		uint32_t	handle;
		uint32_t	cbData;
		uint32_t	bWriteComplete;
		// char[cbFilename]
		// BYTE[cbData]
	} TWriteFileIn, * PTWriteFileIn;

	typedef struct _tagCopyFileIn
	{
		uint32_t	cbSrc;
		uint32_t	cbDst;
		uint32_t	flags;
		// char[cbSrc]
		// char[cbDst]
	} TCopyFileIn, * PTCopyFileIn;

	typedef struct _tagMkdirIn
	{
		uint32_t	cbFilename;
		uint32_t	bRecursive;
		// char[cbFilename]
	} TMkdirIn, * PTMkdirIn;

	class FileSystemObject : public ConfigurationArea {
	public:
		FileSystemObject(BasicADS& adsClient);

		virtual ~FileSystemObject() {};

		FileSystemObject(const FileSystemObject& other);

		FileSystemObject& operator=(const FileSystemObject& other);

		

		void deleteFile(const char file_name[], bool bRecursive = false);
		void dir(const char folder_name[]);
		void readDeviceFile(const char file_name[], std::ostream& local_file);
		void writeDeviceFile(const char file_name[], std::istream& data);
		void copyDeviceFile(const char source[], const char dest[], uint32_t flags);
		void mkdir(const char path[], bool bRecursive);

		static const uint32_t	m_large_buf = 65536;
		static const uint32_t	m_cbWriteMax = 1024;
		static const uint32_t	m_cbReadMax = 1024;
		static const uint16_t	m_moduleType = MODULETYPE_FSO;

		int16_t m_moduleId;
	};
}
#endif