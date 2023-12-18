#include "app/manager.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <gtest/gtest.h>

auto logger = spdlog::basic_logger_mt("test_logger", "logs/test_app_manager.txt", true);

class AppT1 : public crisp::App
{

public:
    ~AppT1() override
    {
        spdlog::info("{} > {}, release resource", getInnerData()->getAppName(), __func__);
    }

private:
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
};

struct AppT1Packer : public crisp::AppData
{
public:
    AppT1Packer() : AppData("AppT1") {}
    void *createApp() override { return new AppT1; }
    void deleteApp(void *app) override
    {
        auto real_app = static_cast<AppT1 *>(app);
        delete real_app;
        real_app = nullptr;
    }
};

struct AppPacker
{
    crisp::App *app{nullptr};
};

/* App state list:

    APP_ON_CREATE             = 0,
    APP_ON_RESUME             = 1,
    APP_ON_RUNNING            = 2,
    APP_ON_RUNNING_BACKGROUND = 3,
    APP_ON_PAUSE              = 4,
    APP_ON_DESTROY            = 5,

  You can check `logs/test_app_manager.txt for more details
*/
TEST(AppManagerTest, AppLifeCycleBasicTest)
{
    spdlog::info("{:=^4} AppLifeCycleBasicTest START {:=^4}", "", "");

    crisp::AppManager app_manager;

    AppT1Packer app_t1_packer;
    crisp::App *app_t1 = app_manager.createApp(app_t1_packer.getAddr());
    EXPECT_EQ(app_t1->getCurrentState(), 0 /* APP_ON_CREATE */);

    // According to FSM transition graph, only when you call startApp()
    // can you invoke FSM to transfer created App to `ON_RESUME`.
    for (int i = 0; i < 5; i++) {
        app_manager.update();
    }
    EXPECT_EQ(app_t1->getCurrentState(), 0 /* APP_ON_CREATE */);

    app_manager.startApp(app_t1);
    EXPECT_EQ(app_t1->getCurrentState(), 1 /* APP_ON_RESUME */);

    for (int i = 0; i < 5; i++) {
        app_manager.update();
    }
    EXPECT_EQ(app_t1->getCurrentState(), 2 /* APP_ON_RUNNING */);
    EXPECT_EQ(app_manager.size(), 1);

    app_manager.destroyApp(&app_t1);
    EXPECT_EQ(app_t1, nullptr); /* app_t1 has been destroyed */
    EXPECT_EQ(app_manager.size(), 0);

    for (int i = 0; i < 5; i++) {
        app_manager.update();
    }
    /* app_t1 has been destroyed */
    EXPECT_EQ(app_t1, nullptr); /* app_t1 has been destroyed */
    EXPECT_EQ(app_manager.size(), 0);
}

TEST(AppManagerTest, BunchAppLifeCycleTest)
{
    spdlog::info("{:=^4} BunchAppLifeCycleBasicTest START {:=^4}", "", "");

    AppT1Packer app_t1_packer;
    crisp::AppManager app_manager;
    std::vector<AppPacker> apps_packer;

    int app_num = 5;
    apps_packer.reserve(app_num);
    for (int i = 0; i < app_num; i++) {
        crisp::App *new_app = app_manager.createApp(app_t1_packer.getAddr());
        EXPECT_EQ(new_app->getCurrentState(), 0 /* APP_ON_CREATE */);
        app_manager.startApp(new_app);
        EXPECT_EQ(new_app->getCurrentState(), 1 /* APP_ON_RESUME */);

        apps_packer.emplace_back(AppPacker{new_app});
    }
    EXPECT_EQ(app_manager.size(), 5);
    app_manager.printInfo();

    /* trigger FSM to transit state */
    app_manager.update();
    for (const auto &packer : apps_packer) {
        EXPECT_EQ(packer.app->getCurrentState(), 2 /* APP_ON_RUNNING */);
    }
    EXPECT_EQ(app_manager.size(), 5);

    /* =========================== STAGE 1 BEGIN =========================== */
    /* let app send message to FSM core */
    apps_packer[0].app->setRunningBackgroundPermission(true);
    apps_packer[0].app->sendGotoCloseMsg();
    apps_packer[1].app->sendGotoDestroyMsg();
    // NOTE: FSM will receive message and reset message box
    //       when you call app_manager's update func.
    app_manager.update();

    // This update will change app to ON_PAUSE first then ON_RUNNING_BACKGROUND
    EXPECT_EQ(apps_packer[0].app->getCurrentState(), 3 /* APP_ON_RUNNING_BACKGROUND */);
    apps_packer[1].app = nullptr; // This app has been destroyed, for usage safety, it better to set
                                  // app pointer to `nullptr`.
    EXPECT_EQ(app_manager.size(), 4);
    app_manager.printInfo();

    /* =========================== STAGE 2 BEGIN =========================== */
    /* restart background app */
    apps_packer[0].app->sendGotoStartMsg();
    app_manager.update();
    EXPECT_EQ(apps_packer[0].app->getCurrentState(), 2 /* APP_ON_RUNNING */);

    /* =========================== STAGE 3 BEGIN =========================== */
    /* rest permission and close app */
    apps_packer[0].app->setRunningBackgroundPermission(false);
    apps_packer[0].app->sendGotoCloseMsg();
    app_manager.update();
    apps_packer[0].app = nullptr; // This app has been destroyed, for usage safety, it better to set
                                  // app pointer to `nullptr`.
    EXPECT_EQ(app_manager.size(), 3);
    app_manager.printInfo();

    /* =========================== STAGE 4 BEGIN =========================== */
    apps_packer[2].app->setRunningBackgroundPermission(true);
    apps_packer[2].app->sendGotoCloseMsg();
    app_manager.update();
    apps_packer[2].app->sendGotoDestroyMsg();
    app_manager.update();
    apps_packer[2].app = nullptr; // This app has been destroyed, for usage safety, it better to set
    EXPECT_EQ(app_manager.size(), 2);
    app_manager.printInfo();

    /* =========================== STAGE 5 BEGIN =========================== */
    /* check remained apps'state */
    for (int i = 3; i < app_num; i += 1) {
        EXPECT_EQ(apps_packer[i].app->getCurrentState(), 2 /* APP_ON_RUNNING */);
        app_manager.destroyApp(&apps_packer[i].app);
    }
    EXPECT_EQ(app_manager.size(), 0);
    app_manager.printInfo();

    /* =========================== STAGE 6 BEGIN =========================== */
    crisp::App *app_t1 = app_manager.createApp(app_t1_packer.getAddr());
    EXPECT_EQ(app_t1->getCurrentState(), 0 /* APP_ON_CREATE */);
    EXPECT_EQ(app_manager.size(), 1);
    app_manager.printInfo();
    app_manager.destroyApp(&app_t1);
    EXPECT_EQ(app_manager.size(), 0);
}

int main(int argc, char **argv)
{
    // TODO(user,test_app_manager): Comment following line if you want string output
    spdlog::set_default_logger(logger); // set default logger to log-file version

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}