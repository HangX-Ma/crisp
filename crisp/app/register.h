#ifndef CRISP_APP_REGISTER_H
#define CRISP_APP_REGISTER_H

#include "app/app.h"
#include "core/register_template.h"
#include <unordered_set>

namespace crisp
{

class AppRegister : public Register<AppData>
{
public:
    ~AppRegister() override;

    bool install(AppData *app_data, void *user_data) override;
    bool uninstall(AppData *app_data, bool free_mem) override;

    void clear(bool free_mem) override;
};

} // namespace crisp

#endif