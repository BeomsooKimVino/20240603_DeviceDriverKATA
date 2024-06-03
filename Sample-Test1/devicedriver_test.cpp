#include "../20240603_DeviceDriverKATA/DeviceDriver.cpp"

#include <gtest/gtest.h> 
#include <gmock/gmock.h> 


#include <string> 
#include <stdexcept> 

using namespace testing;

class MockFlash : public FlashMemoryDevice {
public:
    MOCK_METHOD(unsigned char, read, (long addr), (override));
    MOCK_METHOD(void, write, (long address, unsigned char data), (override));

};

class DeviceDriverFixture : public testing::Test {
public:
    MockFlash flash;
};


TEST_F(DeviceDriverFixture, ReadSuccess) {
    
    EXPECT_CALL(flash, read).Times(5).WillRepeatedly(Return(0xA));

    // SUT
    DeviceDriver drv(&flash);
    int address = 0x10;
    drv.read(address);
}

TEST_F(DeviceDriverFixture, ReadSecondReadFail) {
    
    EXPECT_CALL(flash, read).Times(2).WillOnce(Return(0xA)).WillOnce(Return(0xB));

    // SUT
    DeviceDriver drv(&flash);
    int address = 0x10;

    EXPECT_THROW({
            drv.read(address);
        }, ReadFailException);
}

TEST_F(DeviceDriverFixture, Read5thReadFail) {
    
    EXPECT_CALL(flash, read).Times(5).WillOnce(Return(0xA)).WillOnce(Return(0xA)).WillOnce(Return(0xA)).WillOnce(Return(0xA)).WillOnce(Return(0xB));

    // SUT
    DeviceDriver drv(&flash);
    int address = 0x10;
    EXPECT_THROW({
            drv.read(address);
        }, ReadFailException);
}


TEST_F(DeviceDriverFixture, WriteSuccess) {

    int address = 0x10;
    unsigned char willWriteData = 0xA;
    unsigned char willReadData = willWriteData;
    unsigned char expectedData = willWriteData;

    EXPECT_CALL(flash, read).Times(1).WillOnce(Return(FlashMemoryDevice::IS_CLEAN_PAGE));
    EXPECT_CALL(flash, write).Times(1);

    // SUT
    DeviceDriver drv(&flash);
    drv.write(address, willWriteData);
}

TEST_F(DeviceDriverFixture, WriteOverwriteFail) {

    int address = 0x10;
    unsigned char willWriteData = 0xA;
    unsigned char dirtyPageData = 0xAA;

    EXPECT_CALL(flash, read).Times(1).WillOnce(Return(dirtyPageData));

    // SUT
    DeviceDriver drv(&flash);
    EXPECT_THROW({
         drv.write(address, willWriteData);
        }, WriteFailException);
}
