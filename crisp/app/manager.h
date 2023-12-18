#ifndef CRISP_APP_MANAGER_H
#define CRISP_APP_MANAGER_H

#include "app/app.h"
#include <vector>

namespace crisp
{

class AppManager
{
public:
    App *createApp(AppData *appdata);
    bool startApp(App *app);
    bool closeApp(App *app);
    bool destroyApp(App *app);
    void clear();
    void update();

    size_t getMgrAppNum() { return app_list_.size(); }
    struct AppPacker
    {
        App *app{nullptr};
    };

private:
    int getAppIndexFromList(App *app);

private:
    std::vector<AppPacker> app_list_;
};

} // namespace crisp

#endif // CRISP_APP_MANAGER_H