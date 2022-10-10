#include "miscellaneous.h"
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

Miscellaneous::Miscellaneous(BasicADS& adsClient)
	: ConfigurationArea(adsClient)
	, m_moduleId(-1)
{
	// Search if module type is present on the target
	// If so, assign module Id
	m_moduleId = getFirstModuleId(m_moduleType);
	m_bModuleExists = (m_moduleId > -1) ? true : false;
};

Miscellaneous::Miscellaneous(const Miscellaneous& other)
	: ConfigurationArea(other.m_adsClient)
	, m_moduleId(other.m_moduleId) {};

Miscellaneous& Miscellaneous::operator=(const Miscellaneous& other) {
	m_adsClient = other.m_adsClient;
	m_moduleId = other.m_moduleId;
	return *this;
}

int32_t Miscellaneous::rebootDevice() {
	uint32_t n_bytesRead = 0;

	uint32_t u32_reboot = 0;
	u32_reboot = 0xB001 + (m_moduleId << 4);
	u32_reboot = (u32_reboot << 16) + 1; // SubIndex = 1 as described in https://infosys.beckhoff.com/content/1033/devicemanager/263036171.html
	unsigned char dummy = 0;

	return m_adsClient.AdsWriteReq(MDP_IDX_GRP, u32_reboot, sizeof(dummy), &dummy);	
}

int32_t Miscellaneous::readStateSecurityWizard() {
	std::cout << "> Read state of the SecurityWizard:" << std::endl;

	uint32_t n_bytesRead = 0;

	// Read the state of the Security Wizard

	uint32_t u32_secWizard = 0;
	u32_secWizard = 0x8001 + (m_moduleId << 4);
	u32_secWizard = (u32_secWizard << 16) + 4; // 4 == Subindex for Security Wizard

	unsigned char bSecWizardState = 0;
	return m_adsClient.AdsReadReq(MDP_IDX_GRP, u32_secWizard, sizeof(bSecWizardState), &bSecWizardState, &n_bytesRead); // TODO: Beispiel machen
	//std::cout << ">>> Security Wizard: " << std::boolalpha << static_cast<bool>(bSecWizardState) << std::endl;
}