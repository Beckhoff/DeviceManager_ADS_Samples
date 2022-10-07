#include "file_system_object.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <assert.h>
#include <iostream> // if ndebug
#include <cstring>

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

using namespace DeviceManager;

FileSystemObject::FileSystemObject(BasicADS& adsClient)
	:  ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	// Search if module type is present on the target
	// If so, assign module Id
	m_moduleId = getFirstModuleId(m_moduleType);
	m_bModuleExists = (m_moduleId > 1) ? true : false;
};

FileSystemObject::FileSystemObject(const FileSystemObject& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleId(other.m_moduleId) {};

FileSystemObject& FileSystemObject::operator=(const FileSystemObject& other) {
	m_adsClient = other.m_adsClient;
	m_moduleId = other.m_moduleId;
	return *this;
}



void FileSystemObject::deleteFile(const char file_name[], bool bRecursive)
{
	assert(file_name != NULL);
	assert(strlen(file_name) > 0);

	std::cout << "> Delete file/folder \"" << file_name << "\"" << std::endl;

	char service_transfer_object[50] = {}; // cbFilename (4 byte), bRecursive (4byte), filename
	char* p_sdo = service_transfer_object;

	uint32_t file_name_length = (uint32_t)strlen(file_name);

	// Copy cbFilename to service transfer object
	*reinterpret_cast<uint32_t*>(p_sdo) = file_name_length;
	p_sdo += sizeof(file_name_length);
	// Copy bRecursive to service transfer object
	*reinterpret_cast<uint32_t*>(p_sdo) = bRecursive;
	p_sdo += 4;
	// Copy filename to service transfer object
	memcpy(p_sdo, file_name, file_name_length);

	uint32_t u32_del_file_idx = 0xB004 + (m_moduleId << 4);
	u32_del_file_idx = (u32_del_file_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_del_file_idx + 1 /* trigger */, 8 + (uint32_t)file_name_length, service_transfer_object);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	// Read state of operation

	n_err = getStoStateInfo(u32_del_file_idx);
	if (n_err != ADSERR_NOERR) {
		exit(-1);
	}
}

void FileSystemObject::dir(const char folder_name[])
{
	assert(folder_name != NULL);
	assert(strlen(folder_name) > 0);

	std::cout << "> List files/folder in \"" << folder_name << "\"" << std::endl;

	char service_transfer_object[50] = {}; // cbFilename (4 byte), bRecursive (4byte), filename
	uint32_t folder_name_length = (uint32_t)strlen(folder_name);

	// Copy cbFilename to service transfer object
	*reinterpret_cast<uint32_t*>(service_transfer_object) = folder_name_length;
	// Copy folder name to service transfer object
	memcpy(service_transfer_object + sizeof(folder_name_length), folder_name, folder_name_length);

	uint32_t u32_dir_idx = 0xB000 + (m_moduleId << 4);
	u32_dir_idx = (u32_dir_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_dir_idx + 1 /* trigger */, sizeof(folder_name_length) + folder_name_length, service_transfer_object);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
	}

	// Read state and data of operation
	std::shared_ptr<char[]> buffer;
	n_err = getStoStateInfo(u32_dir_idx, m_large_buf, buffer);

	if (n_err != ADSERR_NOERR) {
		exit(-1);
	}

	char* dir_sdo_data = buffer.get();

	DeviceManager::TDir dirInfo = *reinterpret_cast<DeviceManager::PTDir>(dir_sdo_data);

	// Iterate over directories
	const char* announce_folders = (dirInfo.cDirs > 0) ? ">> Directories :" : " >> No directories found";
	std::cout << announce_folders << std::endl;
	char* dir_offset = dir_sdo_data + dirInfo.nOffsFirstDir;

	for (uint32_t i_dir = 0; i_dir < dirInfo.cDirs; ++i_dir) {
		DeviceManager::TDirInfo dirInfo = *reinterpret_cast<DeviceManager::PTDirInfo>(dir_offset);

		char* pDirName = dir_offset + sizeof(dirInfo); // Move forward to char[]
		std::string sDirName(pDirName, dirInfo.cchName);
		std::cout << ">>> " << sDirName << "/" << std::endl;

		dir_offset = dir_sdo_data + dirInfo.nOffsNextDir; // Move forward to next TDirInfo
	}

	// Iterate over files
	const char* announce_files = (dirInfo.cFiles > 0) ? ">> Files :" : " >> No files found";
	std::cout << announce_files << std::endl;
	char* file_offset = dir_sdo_data + dirInfo.nOffsFirstFile;

	for (uint32_t i_files = 0; i_files < dirInfo.cFiles; ++i_files) {
		DeviceManager::TFileInfo fileInfo = *reinterpret_cast<DeviceManager::PTFileInfo>(file_offset);

		char* pFileName = file_offset + sizeof(fileInfo); // Move forward to char[]
		std::string sFileName(pFileName, fileInfo.cchFile);
		std::cout << ">>> " << sFileName << " [size: " << fileInfo.filesize << " byte]" << std::endl;

		file_offset = dir_sdo_data + fileInfo.nOffsNextFile; // Move forward to next TFileInfo
	}
}

void FileSystemObject::readDeviceFile(const char file_name[], std::ostream& local_file)
{
	assert(file_name != NULL);
	assert(strlen(file_name) > 0);

	std::cout << "> Read file " << file_name << " from target" << std::endl;

	// STO: cbFilename (4 byte), Continuation handle (4byte), cbMaxRead (4byte), Filename (char[])
	uint32_t file_name_length = (uint32_t)strlen(file_name);
	auto sdo_wBuf = std::shared_ptr<char[]>(new char[12 + file_name_length]);
	char* pWBuf = sdo_wBuf.get();

	DeviceManager::TReadFileIn write_info = {
		file_name_length, // cbFileName
		0, // Continuation handle
		m_cbReadMax
	};
	
	// Copy read info SDO
	*reinterpret_cast<DeviceManager::PTReadFileIn>(pWBuf) = write_info;
	// Copy file name to service transfer object
	pWBuf += sizeof(write_info);
	memcpy(pWBuf, file_name, file_name_length);

	uint32_t u32_rd_idx = 0xB001 + (m_moduleId << 4);
	u32_rd_idx = (u32_rd_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_rd_idx + 1 /* trigger */, 12 + file_name_length, sdo_wBuf.get());

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	bool bComplete = false;
	bool bErr = false;

	while (!bComplete && !bErr) {

		std::shared_ptr<char[]> sdo_rBuf;
		n_err = getStoStateInfo(u32_rd_idx, m_cbReadMax, sdo_rBuf);

		if (n_err != ADSERR_NOERR) {
			bErr = true;
		}
		char* rd_sdo_data = sdo_rBuf.get();

		DeviceManager::TReadFileOut read_info = *reinterpret_cast<DeviceManager::PTReadFileOut>(rd_sdo_data);
		rd_sdo_data += sizeof(read_info);
		local_file.write(rd_sdo_data, (std::streamsize)read_info.cbData);

		bComplete = read_info.bMoreData;

		if (!bComplete) {
			// Request more data
			DeviceManager::TReadFileIn sdo_rd_in = {
				0, // cbFileName
				read_info.handle,
				m_cbReadMax
			};

			n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_rd_idx + 1 /* trigger */, sizeof(sdo_rd_in), &sdo_rd_in);

			if (n_err != ADSERR_NOERR) {
				bErr = true;
				std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
				exit(-1);
			}
		}
	}
}

void FileSystemObject::writeDeviceFile(const char file_name[], std::istream& data)
{
	assert(file_name != NULL);
	assert(strlen(file_name) > 0);

	std::cout << "> Write file " << file_name << " to target" << std::endl;

	// Calcualte size of data:
	data.seekg(0, data.end);
	uint32_t data_length = (uint32_t)data.tellg();
	data.seekg(0, data.beg);

	uint32_t file_name_length = (uint32_t)strlen(file_name);

	// Request write handle first
	DeviceManager::TWriteFileIn write_info = {
		file_name_length, // cbFilename
		0, // Continuation handle
		0, // cbData
		0 // bWriteCompleted
	};

	// Create buffer for the SDO object to write
	uint32_t cbWrite = sizeof(write_info) + file_name_length;// +cbDataWrite;
	auto sdo_wBuf = std::shared_ptr<char[]>(new char[cbWrite]);
	char* pWBuf = sdo_wBuf.get();

	// Copy write info to buffer
	*reinterpret_cast<DeviceManager::PTWriteFileIn>(pWBuf) = write_info;
	// Copy file name to service transfer object
	pWBuf += sizeof(write_info);
	memcpy(pWBuf, file_name, file_name_length);


	uint32_t u32_wrt_idx = 0xB002 + (m_moduleId << 4);
	u32_wrt_idx = (u32_wrt_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_wrt_idx + 1 /* trigger */, cbWrite, sdo_wBuf.get());

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}

	// Read write-handle and state of operation (MDP status (1byte), padding (1byte), Continuation handle (4 byte))
	std::shared_ptr<char[]> buf_hdl;
	n_err = getStoStateInfo(u32_wrt_idx, 6, buf_hdl);
	
	uint32_t wrt_hdl = 0;
	wrt_hdl = *reinterpret_cast<uint32_t*>(buf_hdl.get());

	// Calculates the remaining bytes to write
	auto f_remaining = [&]() -> uint32_t { return data_length - (uint32_t)data.tellg(); };

	while (f_remaining() > 0) {

		uint32_t cbDataWrite = (f_remaining() >= m_cbWriteMax) ? m_cbWriteMax : f_remaining();

		DeviceManager::TWriteFileIn write_info = {
			0, // cbFilename
			wrt_hdl, // Continuation handle
			cbDataWrite, // cbData
			(uint32_t)((cbDataWrite < m_cbWriteMax) ? 1 : 0) // Set to 1 to indicate the last write access
		};

		// Create buffer for the SDO object to write
		uint32_t cbWrite = sizeof(write_info) + cbDataWrite;// +cbDataWrite;
		auto sdo_wBuf = std::shared_ptr<char[]>(new char[cbWrite]);
		char* pWBuf = sdo_wBuf.get();

		// Copy write info to buffer
		*reinterpret_cast<DeviceManager::PTWriteFileIn>(pWBuf) = write_info;
		// Copy file name to service transfer object
		pWBuf += sizeof(write_info);

		// Copy data to SDO
		data.read(pWBuf, cbDataWrite);

		n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_wrt_idx + 1 /* trigger */, cbWrite, sdo_wBuf.get());

		if (n_err != ADSERR_NOERR) {
			std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
			exit(-1);
		}

		// Read state of operation
		n_err = getStoStateInfo(u32_wrt_idx);
		
		if (n_err != ADSERR_NOERR) {
			exit(-1); // Some error occurred
		}
	}
}

void FileSystemObject::copyDeviceFile(const char source[], const char dest[], uint32_t flags)
{
	assert(source != NULL && dest !=NULL);
	assert((strlen(source) > 0) && (strlen(dest) > 0));

	std::cout << "> Copy file " << source << " to " << dest << std::endl;

	uint32_t cb_source	= (uint32_t)strlen(source);
	uint32_t cb_dest	= (uint32_t)strlen(dest);

	DeviceManager::TCopyFileIn copyInfo = {
		cb_source, //cbSrc
		cb_dest, //cbDest
		flags
	};

	uint32_t cbWrite = (uint32_t)sizeof(copyInfo) + cb_source + cb_dest;
	auto sdo_wBuf = std::shared_ptr<char[]>(new char[cbWrite]);
	char* pWBuf = sdo_wBuf.get();

	// Copy write info to buffer
	*reinterpret_cast<DeviceManager::PTCopyFileIn>(pWBuf) = copyInfo;
	pWBuf += sizeof(copyInfo);
	memcpy(pWBuf, source, cb_source);
	pWBuf += cb_source;
	memcpy(pWBuf, dest, cb_dest);
	
	uint32_t u32_cpy_idx = 0xB003 + (m_moduleId << 4);
	u32_cpy_idx = (u32_cpy_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_cpy_idx + 1 /* trigger */, cbWrite, sdo_wBuf.get());

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	// Read state of operation
	n_err = getStoStateInfo(u32_cpy_idx);
}

void FileSystemObject::mkdir(const char path[], bool bRecursive)
{
	assert(path != NULL);
	assert((strlen(path) > 0));

	uint32_t cb_path = (uint32_t)strlen(path);
	std::cout << "> Create new folder: " << path << std::endl;

	DeviceManager::TMkdirIn mkdirInfo = {
		cb_path,
		(uint32_t)bRecursive
	};

	uint32_t cbWrite = sizeof(mkdirInfo) + cb_path;
	auto sdo_wBuf = std::shared_ptr<char[]>(new char[cbWrite]);

	*reinterpret_cast<DeviceManager::PTMkdirIn>(sdo_wBuf.get()) = mkdirInfo;

	memcpy(sdo_wBuf.get() + sizeof(mkdirInfo), path, cb_path);

	uint32_t u32_mkdir_idx = 0xB005 + (m_moduleId << 4);
	u32_mkdir_idx = (u32_mkdir_idx << 16);

	int32_t n_err = 0;
	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_mkdir_idx + 1 /* trigger */, cbWrite, sdo_wBuf.get());

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncWriteReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	// Read state of operation
	n_err = getStoStateInfo(u32_mkdir_idx);
}