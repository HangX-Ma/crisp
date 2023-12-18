#include "app/app.h"
#include "core/crisp.h"
#include "core/database.h"
#include <gtest/gtest.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

auto logger = spdlog::basic_logger_mt("test_logger", "logs/test_crisp.txt", true);
class AppT1 : public crisp::App
{
    void onCreate(const crisp::AppData * /*unused*/) override { sendGotoStartMsg(); }
    void onRunning(const crisp::AppData * /*unused*/) override
    {
        auto crisp_data = static_cast<crisp::CrispData *>(getInnerData()->getUserData());
        spdlog::info("{}", (*crisp_data->crisp->getInstalledAppList().begin())->getAppName());
        spdlog::info(
            "Width {}, Height {}",
            crisp_data->database->get(global_uint16_keys[gCrispScreenWidth])->value<uint16_t>(),
            crisp_data->database->get(global_uint16_keys[gCrispScreenHeight])->value<uint16_t>());
    }
};

class AppT1Packer : public crisp::AppData
{
public:
    AppT1Packer() : crisp::AppData("AppT1Packer") {}
    void *createApp() override { return new AppT1; }
    void deleteApp(void *app) override { delete static_cast<AppT1 *>(app); }
};

void callbackInner(simple_kv::SimpleKV &database)
{
    spdlog::warn("check database: {} {}",
                 database.get(global_uint16_keys[gCrispScreenWidth])->value<uint16_t>(),
                 database.get(global_uint16_keys[gCrispScreenHeight])->value<uint16_t>());
}

TEST(CrispTest, BasicTest)
{
    spdlog::info("{:=^4} BasicTest START {:=^4}", "", "");

    crisp::Crisp crisp;

    std::function<void(simple_kv::SimpleKV &)> callback = callbackInner;
    crisp.registerDBCallBack(callback);

    crisp.init();

    for (int i = 0; i < 3; i += 1) {
        crisp.update();
    }
}

TEST(CrispTest, UserDataTest)
{
    spdlog::info("{:=^4} UserDataTest START {:=^4}", "", "");

    crisp::Crisp crisp;

    crisp.init();

    crisp.installApp(new AppT1Packer);
    crisp.createApp(*crisp.getInstalledAppList().begin());

    for (int i = 0; i < 6; i++) {
        crisp.update();
    }
}

int main(int argc, char **argv)
{
    // TODO(user,test_crisp): Comment following line if you want string output
    spdlog::set_default_logger(logger); // set default logger to log-file version

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}