#include "app/register.h"

namespace crisp
{

AppRegister::~AppRegister() { clear(true); }

bool AppRegister::install(AppData *appdata, void *user_data)
{
    if (appdata == nullptr) {
        return false;
    }
    if (isInstalled(appdata)) {
        return false;
    }
    appdata->setUserData(user_data);
    appdata_set_.emplace(appdata);
    return true;
}

bool AppRegister::uninstall(AppData *appdata, bool free_mem)
{
    if (appdata == nullptr) {
        return false;
    }
    if (!isInstalled(appdata)) {
        return false;
    }

    appdata_set_.erase(appdata);
    if (free_mem) {
        delete appdata;
    }
    return true;
}

void AppRegister::clear(bool free_mem)
{
    if (free_mem) {
        for (auto item : appdata_set_) {
            delete item;
        }
    }
    appdata_set_.clear();
}

} // namespace crisp