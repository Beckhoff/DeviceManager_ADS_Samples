#ifndef TC_ADSCLIENT_H
#define TC_ADSCLIENT_H

#if __has_include( <Windows.h> )
#   include <Windows.h>
#endif

#include <stdint.h>
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

    long AdsReadReq(long indexGroup, long indexOffset, unsigned long length, void* pData, unsigned long* pBytesRead)
    {
#if __FreeBSD__
 	long ret = AdsSyncReadReqEx(&m_AmsAddr, indexGroup, indexOffset, length, pData, (ads_ui32*)pBytesRead);
#else
 	long ret = AdsSyncReadReqEx(&m_AmsAddr, indexGroup, indexOffset, length, pData, pBytesRead);
#endif
        return ret;
    };

    long AdsWriteReq(long indexGroup, long indexOffset, unsigned long length, void* pData) {
        return AdsSyncWriteReq(&m_AmsAddr, indexGroup, indexOffset, length, pData);
    };

private:

    AmsAddr m_AmsAddr;
    long    m_port;
};

#endif