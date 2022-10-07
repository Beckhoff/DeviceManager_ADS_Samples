#include "miscellaneous.h"
#include "Mdp.h"
#include "BasicADS.h"

#include <assert.h>
#include <iostream> // if ndebug

#if defined _WIN32 || defined __FreeBSD__
#include "TcAdsDef.h"
#else
#include "AdsDef.h"
#endif

using namespace DeviceManager;

Miscellaneous::Miscellaneous(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	// Search if module type is present on the target
	// If so, assign module Id
	m_moduleId = getFirstModuleId(m_moduleType);
	m_bModuleExists = (m_moduleId > 1) ? true : false;
};

Miscellaneous::Miscellaneous(const Miscellaneous& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleId(other.m_moduleId) {};

Miscellaneous& Miscellaneous::operator=(const Miscellaneous& other) {
	m_adsClient = other.m_adsClient;
	m_moduleId = other.m_moduleId;
	return *this;
}

void Miscellaneous::rebootDevice() {
	std::cout << "> Request reboot" << std::endl;

	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;


	// Reboot the device

	uint32_t u32_reboot = 0;
	u32_reboot = 0xB001 + (m_moduleId << 4);
	u32_reboot = (u32_reboot << 16) + 1; // SubIndex = 1 as described in https://infosys.beckhoff.com/content/1033/devicemanager/263036171.html
	unsigned char dummy = 0;

	n_err = m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_reboot, sizeof(dummy), &dummy);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	std::cout << ">>> Reboot Requested" << std::endl;
}

void Miscellaneous::readStateSecurityWizard() {
	std::cout << "> Read state of the SecurityWizard:" << std::endl;

	int32_t n_err = 0;
	uint32_t n_bytesRead = 0;

	// Read the state of the Security Wizard

	uint32_t u32_secWizard = 0;
	u32_secWizard = 0x8001 + (m_moduleId << 4);
	u32_secWizard = (u32_secWizard << 16) + 4; // 4 == Subindex for Security Wizard

	unsigned char bSecWizardState = 0;
	n_err = m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_secWizard, sizeof(bSecWizardState), &bSecWizardState, &n_bytesRead);

	if (n_err != ADSERR_NOERR) {
		std::cerr << "Error AdsSyncReadReq: 0x" << std::hex << n_err << std::endl;
		exit(-1);
	}
	std::cout << ">>> Security Wizard: " << std::boolalpha << static_cast<bool>(bSecWizardState) << std::endl;
}