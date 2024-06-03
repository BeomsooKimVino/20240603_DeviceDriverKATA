#pragma once
#include <iostream>
#include "Application.h"


Application::Application(DeviceDriver* devicedriver) : m_devicedriver(devicedriver)
{}
void Application::ReadAndPrint(long startAddr, long endAddr)
{
    for (int addr = startAddr; addr <= endAddr; addr++)
    {
        std::cout << m_devicedriver->read(addr) << std::endl;
    }
}

void Application::WriteAll(int data)
{
    for (int addr = 0; addr <= 0x4; addr++)
    {
        m_devicedriver->write(addr, data);
    }
}