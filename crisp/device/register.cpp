#include "device/register.h"

namespace crisp
{

DeviceRegister::~DeviceRegister() { clear(true); }

bool DeviceRegister::install(Device *device, void *user_data)
{
    if (device == nullptr) {
        return false;
    }
    if (isInstalled(device)) {
        return false;
    }
    device->setUserData(user_data);
    device->init();
    device_set_.emplace(device);

    return true;
}

bool DeviceRegister::uninstall(Device *device, bool free_mem)
{
    if (device == nullptr) {
        return false;
    }
    if (!isInstalled(device)) {
        return false;
    }

    device_set_.erase(device);
    if (free_mem) {
        delete device;
    }
    return true;
}

void DeviceRegister::clear(bool free_mem)
{
    if (free_mem) {
        for (auto item : device_set_) {
            delete item;
        }
    }
    device_set_.clear();
}

void DeviceRegister::init()
{
    for (auto item : device_set_) {
        item->init();
    }
}

void DeviceRegister::update()
{
    for (auto item : device_set_) {
        item->update();
    }
}

} // namespace crisp