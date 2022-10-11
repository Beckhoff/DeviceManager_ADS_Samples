#ifndef ADS_EXCEPTION_H
#define ADS_EXCEPTION_H

#include <stdexcept>
#include <string>
#include <sstream>

namespace DeviceManager {

// Deklaration from https://github.com/Beckhoff/ADS/blob/master/AdsLib/AdsException.h
struct AdsException : std::exception {
    AdsException(const long adsErrorCode) :
        errorCode(adsErrorCode)
    {
        std::stringstream sstream;
        sstream << "Ads operation failed with error code 0x" << std::hex << adsErrorCode;
        m_Message = sstream.str();
    }

    virtual ~AdsException() throw () {}

    virtual const char* what() const throw ()
    {
        return m_Message.c_str();
    }

    const long errorCode;
protected:
    std::string m_Message;
};
}
#endif
