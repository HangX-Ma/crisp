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
    objects.emplace(device);

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

    objects.erase(device);
    if (free_mem) {
        delete device;
    }
    return true;
}

void DeviceRegister::clear(bool free_mem)
{
    if (free_mem) {
        for (auto item : objects) {
            delete item;
        }
    }
    objects.clear();
}

void DeviceRegister::init()
{
    for (auto item : objects) {
        item->init();
    }
}

void DeviceRegister::update()
{
    for (auto item : objects) {
        item->update();
    }
}

} // namespace crisp