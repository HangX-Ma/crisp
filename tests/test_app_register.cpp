#include "app/register.h"
#include <gtest/gtest.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

auto logger = spdlog::basic_logger_mt("test_logger", "logs/test_app_register.txt", true);

struct AppT1Packer : public crisp::AppData
{
    AppT1Packer() : AppData("AppT1") { spdlog::info("{} > {}", getAppName(), __func__); }
    ~AppT1Packer() override { spdlog::info("{} > {}", getAppName(), __func__); }
};

struct AppT2Packer : public crisp::AppData
{
public:
    AppT2Packer() : AppData("AppT2") { spdlog::info("{} > {}", getAppName(), __func__); }
    ~AppT2Packer() override { spdlog::info("{} > {}", getAppName(), __func__); }
};

TEST(AppRegisterTest, BasicTest)
{
    spdlog::info("{:=^4} BasicTest START {:=^4}", "", "");

    crisp::AppRegister app_register;

    EXPECT_EQ(app_register.size(), 0);

    auto app_t1 = new AppT1Packer;
    auto app_t2 = new AppT1Packer;
    app_register.install(app_t1);
    app_register.install(app_t2);

    EXPECT_TRUE(app_register.isInstalled(app_t1));
    EXPECT_TRUE(app_register.isInstalled(app_t2));
    EXPECT_EQ(app_register.size(), 2);

    app_register.uninstall(app_t1);
    EXPECT_EQ(app_register.size(), 1);

    app_register.clear();
    EXPECT_EQ(app_register.size(), 0);
}

int main(int argc, char **argv)
{
    // TODO(user,test_app_register): Comment following line if you want string output
    spdlog::set_default_logger(logger); // set default logger to log-file version

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}