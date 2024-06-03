#pragma once
#include "DeviceDriver.h"

class Application
{
public:

    static const long writeAllStartAddress = 0x0;
    static const long writeAllEndAddress = 0x4;

    Application(DeviceDriver *devicedriver);
    void ReadAndPrint(long startAddr, long endAddr);
    void WriteAll(int data);

protected:
    DeviceDriver *m_devicedriver;
};
