#ifndef CRISP_DEVICE_REGISTER_H
#define CRISP_DEVICE_REGISTER_H

#include "device/device.h"
#include <unordered_set>

namespace crisp
{

class DeviceRegister
{
public:
    ~DeviceRegister();

    bool install(Device *device, void *user_data = nullptr);
    bool uninstall(Device *device, bool free_mem = true);

    void clear(bool free_mem = true);

    void init();
    void update();
    bool isInstalled(Device *device) { return device_set_.count(device) != 0; }
    std::size_t size() { return device_set_.size(); }

private:
    std::unordered_set<Device *> device_set_;
};

} // namespace crisp

#endif // CRISP_DEVICE_REGISTER_H