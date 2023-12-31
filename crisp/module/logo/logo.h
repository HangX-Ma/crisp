#ifndef CRISP_MODULE_LOGO_LOGO_H
#define CRISP_MODULE_LOGO_LOGO_H

#include "app/app.h"
#include "core/version.h"
#include "spdlog/spdlog.h"
#include <iostream>

namespace crisp::apps
{
class Logo : public App
{
public:
    static const std::string &getLogo() { return LOGO; }
    static void showInfo()
    {
        /* Print logo */
        spdlog::info("\n{}"
                     "\n- @author HangX-Ma\n"
                     "- @version v{}.{}.{}\n"
                     "- @build at {} {}\n",
                     getLogo(), CRISP_VER_MAJOR, CRISP_VER_MINOR, CRISP_VER_PATCH, __TIME__,
                     __DATE__);
    }

    void onCreate(const AppData * /*unused*/) override
    {
        /* Print Info */
        showInfo();
        sendGotoDestroyMsg();
    }

private:
    // generated by TAAG:
    // <https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20>
    static const std::string LOGO;
};

struct LogoData : public AppData
{
    LogoData() : AppData("BootLogo") {}
    void *createApp() override { return new Logo; }
    void deleteApp(void *app) override
    {
        auto real_app = static_cast<Logo *>(app);
        delete real_app;
        real_app = nullptr;
    }
};

} // namespace crisp::apps

#endif // CRISP_MODULE_LOGO_LOGO_H