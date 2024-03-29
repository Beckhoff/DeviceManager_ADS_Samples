#ifndef FILE_SYSTEM_OBJECT_H
#define FILE_SYSTEM_OBJECT_H

#include "BasicADS.h"
#include "BDM_ConfigurationArea.h"
#include "Mdp.h"

#include <fstream>
#include <vector>
#include <string>
#include <functional>

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

	typedef struct _tagFileInfoEx
	{
		int64_t			filesize;
		uint32_t		attribs;
		std::string		fName;
	} TFileInfoEx, * PTFileInfoEx;

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

	typedef void (*fProgress)(int progress);

	class FileSystemObject : public ConfigurationArea {

	public:
		FileSystemObject(BasicADS& adsClient);

		virtual ~FileSystemObject() {};

		FileSystemObject(const FileSystemObject& other);

		FileSystemObject& operator=(const FileSystemObject& other);

		int32_t deleteFile(const char file_name[], bool bRecursive = false);
		int32_t dir(const char folder_name[], std::vector<std::string>& folders, std::vector<std::string>& files);
		int32_t dir(const char folder_name[], std::vector<std::string>& folders, std::vector<TFileInfoEx>& files);
		int32_t readDeviceFile(	const char file_name[],
								std::ostream& local_file,
								size_t& n_bytes_count = m_bytesCount,
								std::function<void(int)> bar = std::function<void(int)>(),
								bool& cancel = m_bDefaultCancel);
		int32_t writeDeviceFile(const char file_name[],
								std::istream& data,
								size_t &n_bytes_count = m_bytesCount,
								std::function<void(int)> bar = std::function<void(int)>(),
								bool& cancel = m_bDefaultCancel);
		int32_t copyDeviceFile(const char source[], const char dest[], uint32_t flags);
		int32_t mkdir(const char path[], bool bRecursive);
		int32_t getFileSize(const char file_path[], size_t& fileSize);

		static const uint16_t	m_moduleType = MODULETYPE_FSO;
		int16_t m_moduleId;
	private:

		static bool m_bDefaultCancel;
		static size_t m_bytesCount;
	};
}
#endif