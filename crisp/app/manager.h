#ifndef CRISP_APP_MANAGER_H
#define CRISP_APP_MANAGER_H

#include "app/app.h"
#include <vector>

namespace crisp
{

class AppManager
{
public:
    App *createApp(AppData *app_data);
    bool startApp(App *app);
    bool closeApp(App *app);
    bool destroyApp(App *app);
    void clear();
    void update();

    size_t getMgrAppNum() { return app_list_.size(); }

private:
    int getAppIndexFromList(App *app);

private:
    std::vector<App *> app_list_;
};

} // namespace crisp

#endif // CRISP_APP_MANAGER_H