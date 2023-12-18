#include "core/crisp.h"
#include "core/database.h"
#include "spdlog/spdlog.h"
#include "logo/logo.h"

namespace crisp
{
Crisp::~Crisp() { spdlog::warn("release crisp resource"); }

void Crisp::init()
{
    spdlog::info("crisp initialization ...");

    if (crisp_inner_.user_data == nullptr) {
        spdlog::info("create userdata automatically");
        crisp_inner_.user_data = new CrispData;
        crisp_inner_.setUserDataFreeFlag(true);
    }

    crisp_inner_.user_data->crisp = this;
    crisp_inner_.user_data->database = &database_;

    internalSetUpDatabase();

    /* Init boot anim */
    /* If boot anim not set */
    if (crisp_inner_.boot_logo == nullptr) {
        spdlog::info("create boot logo automatically");
        crisp_inner_.boot_logo = new apps::LogoData;
        crisp_inner_.setUserBootLogoFreeFlag(true);
    }

    spdlog::info("booting ...");
    app_manager_.clear();
    auto *new_app = app_manager_.createApp(crisp_inner_.boot_logo);
    app_manager_.startApp(new_app);

    do {
        app_manager_.update();
    }
    while (app_manager_.size() != 0U);
    spdlog::info("crisp init done\n");
}

void Crisp::internalSetUpDatabase()
{
    spdlog::info("setup database internally");
    database_.add<uint16_t>(global_uint16_keys[gCrispScreenWidth],
                            global_uint16_vals[gCrispScreenWidth]);

    database_.add<uint16_t>(global_uint16_keys[gCrispScreenHeight],
                            global_uint16_vals[gCrispScreenHeight]);
    if (database_callback_ != nullptr) {
        spdlog::info("call database callback");
        database_callback_(database_);
    }
}

void Crisp::update()
{
    device_register_.update();
    app_manager_.update();
}

} // namespace crisp