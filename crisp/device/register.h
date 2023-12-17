#ifndef CRISP_DEVICE_REGISTER_H
#define CRISP_DEVICE_REGISTER_H

#include "core/register_template.h"
#include "device/device.h"
#include <unordered_set>

namespace crisp
{

class DeviceRegister : public Register<Device>
{
public:
    ~DeviceRegister() override;

    bool install(Device *device, void *user_data) override;
    bool uninstall(Device *device, bool free_mem) override;

    void clear(bool free_mem) override;

    void init();
    void update();
};

} // namespace crisp

#endif // CRISP_DEVICE_REGISTER_H