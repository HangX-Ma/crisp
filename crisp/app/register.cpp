#include "app/register.h"

namespace crisp
{

AppRegister::~AppRegister() { clear(true); }

bool AppRegister::install(AppData *app_data, void *user_data)
{
    if (app_data == nullptr) {
        return false;
    }
    if (isInstalled(app_data)) {
        return false;
    }
    app_data->setUserData(user_data);
    objects.emplace(app_data);
    return true;
}

bool AppRegister::uninstall(AppData *app_data, bool free_mem)
{
    if (app_data == nullptr) {
        return false;
    }
    if (!isInstalled(app_data)) {
        return false;
    }

    objects.erase(app_data);
    if (free_mem) {
        delete app_data;
    }
    return true;
}

void AppRegister::clear(bool free_mem)
{
    if (free_mem) {
        for (auto item : objects) {
            delete item;
        }
    }
    objects.clear();
}

} // namespace crisp