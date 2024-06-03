#include <iostream> 
#include <string> 
#include <stdexcept> 

#include <gtest/gtest.h> 
#include <gmock/gmock.h> 

#include "../20240603_DeviceDriverKATA/Application.cpp"
#include "mocks.cpp"
#include "application_test.h"

using namespace testing;

class ApplicationFixture : public testing::Test {
public:
    MockFlash flash;
    DeviceDriver drv{ &flash };
    Application app{ &drv };

    void ReadAndVerify(long startAddress, long endAddress, long writeData)
    {
        oldstream = std::cout.rdbuf(buffer.rdbuf());

        int readCallCount = (endAddress - startAddress + 1);
        EXPECT_CALL(flash, read).Times(readCallCount * 5).WillRepeatedly(Return(writeData));
        app.ReadAndPrint(startAddress, endAddress);

        int expectData = writeData;
        std::vector<std::string> tokens = tokenize(buffer.str());
        for (auto element : tokens)
        {
            EXPECT_THAT(expectData, Eq(std::stoi(element)));
        }

        std::cout.rdbuf(oldstream);
    }
private:
    std::stringstream buffer;
    std::streambuf* oldstream;

    std::vector<std::string> tokenize(const std::string& input) {
        std::vector<std::string> tokens;
        std::stringstream ss(input);
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
};


TEST_F(ApplicationFixture, AppReadAndPrint) {

    long startAddress = 0x10;
    long endAddress = 0x15;
    long writeData = 0xA;

    ReadAndVerify(startAddress, endAddress, writeData);
}

TEST_F(ApplicationFixture, AppWriteAll) {

    int readCallCount = 5;
    int writeCallCount = 5;
    long writeData = 0xA;
    EXPECT_CALL(flash, read).Times(readCallCount).WillRepeatedly(Return(FlashMemoryDevice::IS_CLEAN_PAGE));
    EXPECT_CALL(flash, write).Times(writeCallCount);

    Application app(&drv);
    app.WriteAll(writeData);

    ReadAndVerify(Application::writeAllStartAddress, Application::writeAllEndAddress, writeData);
}

