#ifndef GENERIC_ADSCLIENT_H
#define GENERIC_ADSCLIENT_H

#include <cstdint>

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


    long AdsReadReq(uint32_t indexGroup, uint32_t indexOffset, uint32_t length, void *pData, uint32_t *pBytesRead)
    {
        return m_adsClient->ReadReqEx2(indexGroup, indexOffset, length, pData, (unsigned int*)pBytesRead);
    };

    long AdsWriteReq(uint32_t indexGroup, uint32_t indexOffset, uint32_t length, void *pData){
        return m_adsClient->WriteReqEx(indexGroup, indexOffset, length, pData);
    };

private:

    AdsDevice *m_adsClient;
};

#endif