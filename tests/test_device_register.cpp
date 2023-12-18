#include "device/register.h"
#include <gtest/gtest.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

using std::string;

auto logger = spdlog::basic_logger_mt("test_logger", "logs/test_device_register.txt", true);

class DeviceBtn1 : public crisp::Device
{

public:
    DeviceBtn1() { spdlog::info("{} > constructor", getDeviceName()); }
    ~DeviceBtn1() override { spdlog::info("{} > destructor", getDeviceName()); }
    void init() override
    {
        setDeviceType(crisp::DeviceType::DEV_BUTTON);
        spdlog::info("{} > {}", getDeviceName(), __func__);
    }
    void update() override { spdlog::info("{} > {}", getDeviceName(), __func__); }

    std::string getDeviceName() override { return {"DeviceBtn1"}; }
};

class DeviceBtn2 : public crisp::Device
{

public:
    DeviceBtn2() { spdlog::info("{} > constructor", getDeviceName()); }
    ~DeviceBtn2() override { spdlog::info("{} > destructor", getDeviceName()); }
    void init() override
    {
        setDeviceType(crisp::DeviceType::DEV_BUTTON);
        spdlog::info("{} > {}", getDeviceName(), __func__);
    }
    void update() override { spdlog::info("{} > {}", getDeviceName(), __func__); }

    std::string getDeviceName() override { return {"DeviceBtn2"}; }
};

TEST(DeviceRegisterTest, BasicTest)
{
    spdlog::info("{:=^4} BasicTest START {:=^4}", "", "");

    crisp::DeviceRegister device_register;
    EXPECT_EQ(device_register.size(), 0);

    auto dev1 = new DeviceBtn1;
    auto dev2 = new DeviceBtn2;

    device_register.install(dev1);
    device_register.install(dev2);
    EXPECT_TRUE(device_register.isInstalled(dev1));
    EXPECT_TRUE(device_register.isInstalled(dev2));
    EXPECT_EQ(device_register.size(), 2);

    device_register.update();

    dev1->setUpdatePermission(false);
    device_register.update();

    device_register.clear();
    EXPECT_EQ(device_register.size(), 0);
}

int main(int argc, char **argv)
{
    // TODO(user,test_device_register): Comment following line if you want string output
    spdlog::set_default_logger(logger); // set default logger to log-file version

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}