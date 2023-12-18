#include "app/manager.h"
#include "app.h"
#include "spdlog/spdlog.h"

namespace crisp
{

int AppManager::generateAppId()
{
    int ret;
    if (!recycled_ids_.empty()) {
        ret = recycled_ids_.front();
        recycled_ids_.pop();
    }
    else {
        ret = next_id_;
        next_id_ += 1;
    }
    return ret;
}

void AppManager::printInfo()
{
    if (app_list_.empty()) {
        spdlog::info("No managed app ...");
        return;
    }

    spdlog::info("Show managed App info");
    for (const auto &packer : app_list_) {
        spdlog::info("-> name_id: {}_{}", packer.app->getInnerData()->getAppName(), packer.id);
    }
}

App *AppManager::createApp(AppData *appdata)
{
    if (appdata == nullptr) {
        return nullptr;
    }
    App *new_app = static_cast<App *>(appdata->createApp());
    if (new_app == nullptr) {
        return nullptr;
    }
    new_app->setInnerData(appdata);
    new_app->create();

    app_list_.emplace_back(AppPacker{new_app, generateAppId()});

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
    app_list_[idx].app->start();
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
    app_list_[idx].app->close();
    return true;
}

bool AppManager::destroyApp(App **app)
{
    if (app_list_.empty()) {
        return false;
    }
    int idx = getAppIndexFromList(*app);
    if (idx < 0) {
        return false;
    }
    recycled_ids_.push(app_list_[idx].id); // recycle app id
    app_list_[idx].app->destroy();
    app_list_.erase(app_list_.begin() + idx);
    (*app)->getInnerData()->deleteApp(*app);
    *app = nullptr;
    return true;
}

void AppManager::update()
{
    for (auto iter = app_list_.begin(); iter != app_list_.end();) {
        auto app = iter->app;
        /* app require to start */
        if (app->checkGotoStartMsg()) {
            app->start();
            goto update_label;
        }
        /* app require to close */
        if (app->checkGotoCloseMsg()) {
            if (app->checkRunningBackgroundPermission()) {
                app->close();
            }
            else {
                app->destroy();
            }
            goto update_label;
        }
        /* app require to destroy */
        if (app->checkGotoDestroyMsg()) {
            app->destroy();
        }
    update_label:
        app->update();
        /* we need to remove the app from list if it has been destroyed */
        if (app->getCurrentState() == App::APP_ON_DESTROY) {
            recycled_ids_.push(iter->id); // recycle app id
            app->getInnerData()->deleteApp(app);
            iter = app_list_.erase(iter);
            continue;
        }
        iter++;
    }
}

void AppManager::clear()
{
    for (auto app_packer : app_list_) {
        app_packer.app->destroy();
    }
    while (!recycled_ids_.empty()) {
        recycled_ids_.pop();
    }
    next_id_ = 0;
    app_list_.clear();
}

int AppManager::getAppIndexFromList(App *app)
{
    if (app == nullptr) {
        return -1;
    }

    for (size_t idx = 0; idx < app_list_.size(); idx += 1) {
        if (app_list_[idx].app == app) {
            return static_cast<int>(idx);
        }
    }
    return -1;
}

} // namespace crisp