#ifndef TC_ADSCLIENT_H
#define TC_ADSCLIENT_H

//#ifdef ADSTC1000

#if __has_include( <Windows.h> )
#   include <Windows.h>
#endif

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <stdexcept>
#include "TcAdsDef.h"
#include "TcAdsAPI.h"
#include "BasicADS.h"


class TC1000AdsClient : public BasicADS {
public:
    TC1000AdsClient(const AmsNetId& remoteNetId = { 0, 0, 0, 0, 0, 0 }, uint32_t timeout = 1000)
        : m_AmsAddr()
        , m_port(0) {

        long nErr = 0;

        m_port = AdsPortOpen();

        if (!m_port)
        {
            throw std::runtime_error(std::string("Opening port on local ADS Router failed."));
        }

        // No AmsNetId provided == local target system
        bool isLocal = std::all_of(remoteNetId.b,
            remoteNetId.b + sizeof(remoteNetId.b),
            [remoteNetId](int x) { return x == remoteNetId.b[0]; }
        );

        if (isLocal)
        {
            nErr = AdsGetLocalAddress(&m_AmsAddr);
            if (nErr) throw std::runtime_error(std::string("Query of local AmsNetId failed."));
        }
        else {
            m_AmsAddr.netId = remoteNetId;
        }

        
        m_AmsAddr.port = 10000;
        AdsSyncSetTimeout(timeout); // ms
    };

    ~TC1000AdsClient() {
        AdsPortClose();
    };

    int32_t AdsReadReq(uint32_t indexGroup, uint32_t indexOffset, uint32_t length, void* pData, uint32_t *pBytesRead)
    {
#if __FreeBSD__
        int32_t ret = AdsSyncReadReqEx(&m_AmsAddr, indexGroup, indexOffset, length, pData, (ads_ui32*)pBytesRead);
#else
        int32_t ret = AdsSyncReadReqEx(&m_AmsAddr, indexGroup, indexOffset, length, pData, (unsigned long*)pBytesRead);
#endif
        return ret;
    };

    int32_t AdsWriteReq(uint32_t indexGroup, uint32_t indexOffset, uint32_t length, void* pData) {
        return AdsSyncWriteReq(&m_AmsAddr, indexGroup, indexOffset, length, pData);
    };

private:

    AmsAddr     m_AmsAddr;
    uint32_t    m_port;
};
//#endif // ADSTC1000
#endif // Include guard
