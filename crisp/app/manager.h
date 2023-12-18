#ifndef CRISP_APP_MANAGER_H
#define CRISP_APP_MANAGER_H

#include "app/app.h"
#include <queue>
#include <vector>

namespace crisp
{

class AppManager
{
public:
    App *createApp(AppData *appdata);
    bool startApp(App *app);
    bool closeApp(App *app);
    bool destroyApp(App **app);
    void clear();
    void update();

    size_t size() { return app_list_.size(); }
    void printInfo();

private:
    int generateAppId();
    int getAppIndexFromList(App *app);

private:
    struct AppPacker
    {
        App *app{nullptr};
        int id;
    };

    std::vector<AppPacker> app_list_;
    std::queue<int> recycled_ids_;
    int next_id_{0};
};

} // namespace crisp

#endif // CRISP_APP_MANAGER_H