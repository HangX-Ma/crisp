#ifndef CORE_APP_MANAGER_H
#define CORE_APP_MANAGER_H

#include "app/app.h"

namespace crisp
{

class AppManager
{

public:
    App *createApp();
    bool startApp();
    bool stopApp();
    bool destroyApp();
};

} // namespace crisp

#endif // CORE_APP_MANAGER_H