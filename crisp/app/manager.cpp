#include "app/manager.h"

namespace crisp
{

App *AppManager::createApp(AppData *app_data)
{
    if (app_data == nullptr) {
        return nullptr;
    }
    App *new_app = static_cast<App *>(app_data->createApp());
    if (new_app == nullptr) {
        return nullptr;
    }
    new_app->setInnerData(app_data);
    new_app->create();

    app_list_.emplace_back(new_app);

    return new_app;
}

bool AppManager::startApp(App *app)
{
    if (app_list_.empty()) {
        return false;
    }
    int idx = getAppIndexFromList(app);
    if (idx < 0) {
        return false;
    }
    app_list_[idx]->start();
    return true;
}

bool AppManager::closeApp(App *app)
{
    if (app_list_.empty()) {
        return false;
    }
    int idx = getAppIndexFromList(app);
    if (idx < 0) {
        return false;
    }
    app_list_[idx]->close();
    return true;
}

bool AppManager::destroyApp(App *app)
{
    if (app_list_.empty()) {
        return false;
    }
    int idx = getAppIndexFromList(app);
    if (idx < 0) {
        return false;
    }
    app_list_[idx]->destroy();
    app_list_.erase(app_list_.begin() + idx);
    return true;
}

void AppManager::update()
{
    for (auto app : app_list_) {
        app->update();
    }
}

void AppManager::clear()
{
    for (auto app : app_list_) {
        app->destroy();
    }
    app_list_.clear();
}

int AppManager::getAppIndexFromList(App *app)
{
    if (app == nullptr) {
        return -1;
    }

    for (size_t idx = 0; idx < app_list_.size(); idx += 1) {
        if (app_list_[idx] == app) {
            return static_cast<int>(idx);
        }
    }
    return -1;
}

} // namespace crisp