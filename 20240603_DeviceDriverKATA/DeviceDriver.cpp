#pragma once
#include "DeviceDriver.h"

class ReadFailException : public std::exception {
    std::string what_message;
public:
    ReadFailException(char const* const message) {
        what_message = message;
    }
    const char* what() const override
    {
        return what_message.c_str();
    }
};

class WriteFailException : public std::exception {
    std::string what_message;
public:
    WriteFailException(char const* const message) {
        what_message = message;
    }
    const char* what() const override
    {
        return what_message.c_str();
    }
};

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int ret = (int)(m_hardware->read(address));
    CheckRTBB(address, ret);
    return ret;
}

void DeviceDriver::CheckRTBB(long address, int ret)
{
    for (int turn = 0; turn < 4; turn++)
    {
        int ret_inner = (int)(m_hardware->read(address));
        if (ret != ret_inner)
        {
            throw ReadFailException("RTBB");
        }
    }
}

void DeviceDriver::write(long address, int data)
{
    CheckCleanPage(address);
    m_hardware->write(address, (unsigned char)data);
}

void DeviceDriver::CheckCleanPage(long address)
{
    int ret = (int)(m_hardware->read(address));
    if (ret != 0xFF)
    {
        throw WriteFailException("Program Fail");
    }
}
