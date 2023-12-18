#include "core/crisp.h"
#include "core/database.h"
#include <gtest/gtest.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

auto logger = spdlog::basic_logger_mt("test_logger", "logs/test_crisp.txt", true);

void callbackInner(simple_kv::SimpleKV &database)
{
    spdlog::warn("check database: {} {}",
                 database.get(global_uint16_keys[gCrispScreenWidth])->value<uint16_t>(),
                 database.get(global_uint16_keys[gCrispScreenHeight])->value<uint16_t>());
}

TEST(CrispTest, BasicTest)
{
    crisp::Crisp crisp;

    std::function<void(simple_kv::SimpleKV &)> callback = callbackInner;
    crisp.registerDBCallBack(callback);

    crisp.init();

    for (int i = 0; i < 3; i += 1) {
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