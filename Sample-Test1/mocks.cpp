#pragma once
#include <gtest/gtest.h> 
#include <gmock/gmock.h> 

#include "../20240603_DeviceDriverKATA/DeviceDriver.h"

class MockFlash : public FlashMemoryDevice {
public:
    MOCK_METHOD(unsigned char, read, (long addr), (override));
    MOCK_METHOD(void, write, (long address, unsigned char data), (override));

};
