#ifndef CRISP_CORE_CRISP_H
#define CRISP_CORE_CRISP_H

#include "app/app.h"
#include "app/register.h"
#include "app/manager.h"
#include "device/register.h"
#include "simplekv/simplekv.h"
#include "spdlog/spdlog.h"

namespace crisp
{
class Crisp;
struct CrispData
{
    Crisp *crisp{nullptr};
    simple_kv::SimpleKV *database{nullptr};
};

class CrispInner
{
public:
    AppData *boot_logo{nullptr};
    CrispData *user_data{nullptr};

    ~CrispInner()
    {
        if (free_user_data_mem_) {
            delete user_data;
        }
        if (free_boot_logo_mem_) {
            delete boot_logo;
        }
    }
    void setUserDataFreeFlag(bool val) { free_user_data_mem_ = val; }
    void setUserBootLogoFreeFlag(bool val) { free_boot_logo_mem_ = val; }

private:
    bool free_user_data_mem_{false};
    bool free_boot_logo_mem_{false};
};

class Crisp
{
public:
    ~Crisp();

    void init();
    void update();

    void registerDBCallBack(std::function<void(simple_kv::SimpleKV &)> callback)
    {
        database_callback_ = std::move(callback);
    }

    void setUserData(CrispData *user_data) { crisp_inner_.user_data = user_data; }
    CrispData *getUserData() { return crisp_inner_.user_data; }

    /* device register */
    bool installDevice(Device *device)
    {
        return device_register_.install(device, crisp_inner_.user_data);
    }
    bool uninstallDevice(Device *device) { return device_register_.uninstall(device, true); }
    void clearDeviceRegister() { device_register_.clear(true); };
    std::size_t getInstalledDeviceNum() { return device_register_.size(); }
    const std::unordered_set<Device *> &getInstalledDeviceList()
    {
        return device_register_.getInstalledDeviceList();
    }

    /* app register */
    bool installApp(AppData *appdata)
    {
        return app_register_.install(appdata, crisp_inner_.user_data);
    }
    bool uninstallApp(AppData *appdata) { return app_register_.uninstall(appdata, true); }
    void clearAppRegister() { app_register_.clear(true); }
    std::size_t getInstalledAppNum() { return app_register_.size(); }
    const std::unordered_set<AppData *> &getInstalledAppList()
    {
        return app_register_.getInstalledAppList();
    }

    /* App manager */
    App *createApp(AppData *appdata) { return app_manager_.createApp(appdata); }
    bool startApp(App *app) { return app_manager_.startApp(app); }
    bool closeApp(App *app) { return app_manager_.closeApp(app); }
    bool destroyApp(App **app) { return app_manager_.destroyApp(app); }
    void clearAppManager() { app_manager_.clear(); };
    std::size_t getManagedAppNum() { return app_manager_.size(); }

private:
    void internalSetUpDatabase();

private:
    DeviceRegister device_register_;
    AppRegister app_register_;
    AppManager app_manager_;
    simple_kv::SimpleKV database_;
    CrispInner crisp_inner_;
    std::function<void(simple_kv::SimpleKV &)> database_callback_{nullptr};
};

} // namespace crisp

#endif // CRISP_CORE_CRISP_H