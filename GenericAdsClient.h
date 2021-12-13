#ifndef GENERIC_ADSCLIENT_H
#define GENERIC_ADSCLIENT_H

#include "BasicADS.h"
#include "AdsDevice.h"
#include "AdsDef.h"
#include "AdsLib.h"


class GenericAdsClient : public BasicADS {
public:
    GenericAdsClient(const AmsNetId& remoteNetId, const char* remoteIpV4)
        : m_adsClient(nullptr){

        m_adsClient = new AdsDevice(remoteIpV4, remoteNetId, 10000);

    };

    ~GenericAdsClient(){
        delete m_adsClient;
    };


    long AdsReadReq(long indexGroup, long indexOffset, unsigned long length, void *pData, unsigned long* pBytesRead)
    {
        return m_adsClient->ReadReqEx2(indexGroup, indexOffset, length, pData, (unsigned int*)pBytesRead);
    };

    long AdsWriteReq(long indexGroup, long indexOffset, unsigned long length, void *pData){
        return m_adsClient->WriteReqEx(indexGroup, indexOffset, length, pData);
    };

private:

    AdsDevice *m_adsClient;
};

#endif