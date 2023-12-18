#ifndef CRISP_DEVICE_DEVICE_H
#define CRISP_DEVICE_DEVICE_H

#include <string>

namespace crisp
{

enum class DeviceType
{
    DEV_CUSTOM = 0,
    DEV_BUTTON,
    DEV_TOUCH_PAD,
    DEV_ENCODER,
    DEV_MOUSE,
    DEV_KEYBOARD
};

class Device
{
public:
    virtual ~Device() = default;

    void setUserData(void *user_data) { user_data_ = user_data; }
    void *getUserData() { return user_data_; }
    Device *getAddr() { return this; }

    const DeviceType &getDeviceType() { return device_type_; }
    void setDeviceType(DeviceType type) { device_type_ = type; }

    virtual void init() {}
    virtual void update() {}
    virtual std::string getDeviceName() { return {}; }

    void setUpdatePermission(bool permission) { can_update_ = permission; }
    bool getUpdatePermission() { return can_update_; }

private:
    DeviceType device_type_{DeviceType::DEV_CUSTOM};
    void *user_data_{nullptr};
    bool can_update_{true};
};

} // namespace crisp

#endif // CRISP_DEVICE_DEVICE_H