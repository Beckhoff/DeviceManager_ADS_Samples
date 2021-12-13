#ifndef BASIC_ADS_H
#define BASIC_ADS_H

/*
 * BasicADS
 *
 * This interface defines a thin wrapper arround the 
 * diffrent variants of AdsClients
 */


class BasicADS {
public:
    virtual ~BasicADS(){};

    virtual long AdsReadReq(    
                                long indexGroup,
                                long indexOffset,
                                unsigned long length,
                                void* pData,
                                unsigned long* pBytesRead) = 0;

    virtual long AdsWriteReq(
                                long indexGroup,
                                long indexOffset,
                                unsigned long length,
                                void* pData) = 0;
           
};

#endif