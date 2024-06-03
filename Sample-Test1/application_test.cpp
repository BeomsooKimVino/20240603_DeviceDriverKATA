#include <iostream> 
#include <string> 
#include <stdexcept> 

#include <gtest/gtest.h> 
#include <gmock/gmock.h> 

#include "../20240603_DeviceDriverKATA/Application.cpp"
#include "mocks.cpp"

using namespace testing;

class ApplicationFixture : public testing::Test {
public:
    MockFlash flash;
    DeviceDriver drv{ &flash };
    Application app{ &drv };

};

std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

TEST_F(ApplicationFixture, AppReadAndPrint) {

    long startAddress = 0x10;
    long endAddress = 0x15;
    int readCallCount = (endAddress - startAddress + 1);
    long expectData = 0xA;

    EXPECT_CALL(flash, read).Times(readCallCount*5).WillRepeatedly(Return(0xA));

    std::stringstream buffer;
    // 함수의 출력을 버퍼로 리디렉션
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    //////////////////////////////////////////////////////////////////
    
    app.ReadAndPrint(startAddress, endAddress);

    std::vector<std::string> tokens = tokenize(buffer.str());
    for (auto element : tokens)
    {
        EXPECT_THAT(expectData, Eq(std::stoi(element)));
    }
    //////////////////////////////////////////////////////////////////

    // 스트림 리디렉션을 원래대로 복구
    std::cout.rdbuf(old);
}

TEST_F(ApplicationFixture, AppWriteAll) {

    int readCallCount = 5;
    int writeCallCount = 5;
    long writeData = 0xA;
    EXPECT_CALL(flash, read).Times(readCallCount).WillRepeatedly(Return(FlashMemoryDevice::IS_CLEAN_PAGE));
    EXPECT_CALL(flash, write).Times(writeCallCount);

    Application app(&drv);
    app.WriteAll(writeData);

    std::stringstream buffer;
    // 함수의 출력을 버퍼로 리디렉션
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    //////////////////////////////////////////////////////////////////
    long startAddress = 0x0;
    long endAddress = 0x4;
    readCallCount = (endAddress - startAddress + 1);
    EXPECT_CALL(flash, read).Times(readCallCount * 5).WillRepeatedly(Return(writeData));
    app.ReadAndPrint(startAddress, endAddress);

    std::vector<std::string> tokens = tokenize(buffer.str());
    int expectData = writeData;
    for (auto element : tokens)
    {
        EXPECT_THAT(expectData, Eq(std::stoi(element)));
    }
    //////////////////////////////////////////////////////////////////

    // 스트림 리디렉션을 원래대로 복구
    std::cout.rdbuf(old);
}
