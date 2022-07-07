#ifndef BASIC_ADS_H
#define BASIC_ADS_H

#include <cstdint>
/*
 * BasicADS
 *
 * This interface defines a thin wrapper arround the 
 * diffrent variants of AdsClients
 */


class BasicADS {
public:
    virtual ~BasicADS(){};

    virtual int32_t AdsReadReq(
                                uint32_t indexGroup,
                                uint32_t indexOffset,
                                uint32_t length,
                                void* pData,
                                uint32_t* pBytesRead) = 0;

    virtual int32_t AdsWriteReq(
                                uint32_t indexGroup,
                                uint32_t indexOffset,
                                uint32_t length,
                                void* pData) = 0;
           
};

#endif