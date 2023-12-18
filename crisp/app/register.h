#ifndef CRISP_APP_REGISTER_H
#define CRISP_APP_REGISTER_H

#include "app/app.h"
#include <unordered_set>

namespace crisp
{

class AppRegister
{
public:
    ~AppRegister();

    bool install(AppData *appdata, void *user_data = nullptr);
    bool uninstall(AppData *appdata, bool free_mem = true);

    void clear(bool free_mem = true);

    bool isInstalled(AppData *appdata) { return appdata_set_.count(appdata) != 0; }
    std::size_t size() { return appdata_set_.size(); }
    const std::unordered_set<AppData *> &getInstalledAppList() { return appdata_set_; }

private:
    std::unordered_set<AppData *> appdata_set_;
};

} // namespace crisp

#endif