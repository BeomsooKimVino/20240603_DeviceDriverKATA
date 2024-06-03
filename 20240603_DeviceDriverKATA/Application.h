#pragma once
#include "DeviceDriver.h"

class Application
{
public:
    Application(DeviceDriver *devicedriver);
    void ReadAndPrint(long startAddr, long endAddr);
    void WriteAll(int data);

protected:
    DeviceDriver *m_devicedriver;
};
