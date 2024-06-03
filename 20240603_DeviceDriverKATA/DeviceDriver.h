#pragma once
#include "FlashMemoryDevice.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice *hardware);
    virtual int read(long address);
    void CheckRTBB(long address, int ret);
    virtual void write(long address, int data);

    void CheckCleanPage(long address);

protected:
    FlashMemoryDevice* m_hardware;
};


