#ifndef TC_ADSCLIENT_H
#define TC_ADSCLIENT_H

#if __has_include( <Windows.h> )
#   include <Windows.h>
#endif

#include <cstdint>
#include "TcAdsDef.h"
#include "TcAdsAPI.h"
#include "BasicADS.h"


class TC1000AdsClient : public BasicADS {
public:
    TC1000AdsClient(const AmsNetId& remoteNetId)
        : m_AmsAddr()
        , m_port(0) {

        m_AmsAddr.netId = remoteNetId;
        m_AmsAddr.port = 10000;

        m_port = AdsPortOpen();

    };

    ~TC1000AdsClient() {
        AdsPortClose();
    };

    long AdsReadReq(uint32_t indexGroup, uint32_t indexOffset, uint32_t length, void* pData, uint32_t *pBytesRead)
    {
#if __FreeBSD__
 	long ret = AdsSyncReadReqEx(&m_AmsAddr, indexGroup, indexOffset, length, pData, (ads_ui32*)pBytesRead);
#else
 	long ret = AdsSyncReadReqEx(&m_AmsAddr, indexGroup, indexOffset, length, pData, (unsigned long*)pBytesRead);
#endif
        return ret;
    };

    long AdsWriteReq(uint32_t indexGroup, uint32_t indexOffset, uint32_t length, void* pData) {
        return AdsSyncWriteReq(&m_AmsAddr, indexGroup, indexOffset, length, pData);
    };

private:

    AmsAddr m_AmsAddr;
    long    m_port;
};

#endif
