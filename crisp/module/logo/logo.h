#ifndef CRISP_MODULE_LOGO_LOGO_H
#define CRISP_MODULE_LOGO_LOGO_H

#include "app/app.h"
#include "core/version.h"
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
        std::cout << getLogo();
        std::cout << "\n- @author HangX-Ma\n";
        std::cout << "- @version "
                  << "v" << CRISP_VER_MAJOR << "." << CRISP_VER_MINOR << "." << CRISP_VER_PATCH
                  << "\n";
        std::cout << "- @build at " << __TIME__ << " " << __DATE__ << "\n\n";
    }

    void onCreate(const AppData * /*unused*/) override
    {
        /* Print Info */
        showInfo();
        internalEvent(APP_ON_DESTROY);
    }

private:
    // generated by TAAG:
    // <https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20>
    static const std::string LOGO;
};

class LogoData : public AppData
{
    std::string getAppName() override { return "BootLogo"; }
    void *createApp() override { return new Logo; }
    void deleteApp(void *app) override { delete static_cast<Logo *>(app); }
};

} // namespace crisp::apps

#endif // CRISP_MODULE_LOGO_LOGO_H