#include "core/crisp.h"
#include "core/database.h"
#include <gtest/gtest.h>
#include "manager.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

auto logger = spdlog::basic_logger_mt("test_logger", "logs/test_app_manager.txt", true);

class AppT1 : public crisp::App
{
    void onCreate(const crisp::AppData * /*unused*/) override
    {
        spdlog::info("{} > {}", getInnerData()->getAppName(), __func__);
    }
    void onResume(const crisp::AppData * /*unused*/) override
    {
        spdlog::info("{} > {}", getInnerData()->getAppName(), __func__);
    }
    void onRunning(const crisp::AppData * /*unused*/) override
    {
        spdlog::info("{} > {}", getInnerData()->getAppName(), __func__);
    }
    void onPause(const crisp::AppData * /*unused*/) override
    {
        spdlog::info("{} > {}", getInnerData()->getAppName(), __func__);
    }
    void onDestroy(const crisp::AppData * /*unused*/) override
    {
        spdlog::info("{} > {}", getInnerData()->getAppName(), __func__);
    }

public:
    ~AppT1() override
    {
        spdlog::info("{} > {}, release resource", getInnerData()->getAppName(), __func__);
    }
};

class AppT1Packer : public crisp::AppData
{
    std::string getAppName() override { return "AppT1"; }
    void *createApp() override { return new AppT1; }
    void deleteApp(void *app) override { delete static_cast<AppT1 *>(app); }
};

TEST(AppManagerTest, AppLifeCycleTest)
{
    crisp::AppManager app_manager;

    AppT1Packer app_t1_packer;
    crisp::App *app_t1 = app_manager.createApp(app_t1_packer.getAddr());

    for (int i = 0; i < 5; i++) {
        app_manager.update();
    }

    app_manager.startApp(app_t1);

    for (int i = 0; i < 5; i++) {
        app_manager.update();
    }
    app_manager.destroyApp(app_t1);
}

int main(int argc, char **argv)
{
    // TODO(user,test_app_manager): Comment following line if you want string output
    spdlog::set_default_logger(logger); // set default logger to log-file version

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}