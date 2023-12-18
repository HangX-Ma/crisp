#ifndef CRISP_APP_APP_H
#define CRISP_APP_APP_H

#include "tinyfsm/fsm.h"
#include <string>

namespace crisp
{

/**
 * Contains the essential elements of an app.
 * You can override those virtual method to set them.
 */
class AppData : public EventData
{
public:
    void setUserData(void *user_data) { user_data_ = user_data; }
    void *getUserData() { return user_data_; }
    AppData *getAddr() { return this; }

    virtual std::string getAppName() { return {}; };
    virtual void *getAppIcon() { return nullptr; }
    virtual void *createApp() { return nullptr; }
    virtual void deleteApp(void *app) { (void) app; }

private:
    void *user_data_{nullptr};
};

/**
 * @brief Contains states for life cycle control (FSM, Finite-Sate Machine)
 */
class App : public TinyFSM
{
public:
    App() : TinyFSM(APP_MAX_STATES) {}
    AppData *getInnerData() { return inner_data_; }
    void setInnerData(AppData *data) { inner_data_ = data; }

    void create(const AppData *data = nullptr);
    void start(const AppData *data = nullptr);
    void close(const AppData *data = nullptr);
    void destroy();
    void update();

    /* APIs to send state message */
    void setRunningBackgroundPermission(bool permission)
    {
        special_bits_ &= ~(1 << 4); // clear previous setting
        special_bits_ |= static_cast<uint8_t>(permission) << 4;
    }
    bool checkRunningBackgroundPermission() { return ((special_bits_ >> 4) & 0x1) != 0; }
    void sendGotoMsg(uint8_t bit) { special_bits_ |= (1 << bit); }
    void sendGotoStartMsg() { sendGotoMsg(0); }
    void sendGotoCloseMsg() { sendGotoMsg(1); }
    void sendGotoDestroyMsg() { sendGotoMsg(2); }
    bool checkGotoMsg(uint8_t bit)
    {
        bool ret = ((special_bits_ >> bit) & 0x1) != 0; // read bit
        special_bits_ &= ~(1 << bit);                   // clear bit
        return ret;
    }
    bool checkGotoStartMsg() { return checkGotoMsg(0); }
    bool checkGotoCloseMsg() { return checkGotoMsg(1); }
    bool checkGotoDestroyMsg() { return checkGotoMsg(2); }

    void changeCurrentState(StateType new_state) { setCurrentState(new_state); }

    enum States : StateType
    {
        APP_ON_CREATE,
        APP_ON_RESUME,
        APP_ON_RUNNING,
        APP_ON_RUNNING_BACKGROUND,
        APP_ON_PAUSE,
        APP_ON_DESTROY,
        APP_MAX_STATES,
    };

protected:
    /* Lifecycle methods */
    /* ref: <https://developer.android.com/guide/components/activities/activity-lifecycle> */
    virtual void onCreate(const AppData *) = 0;
    virtual void onResume(const AppData * /*unused*/) {}
    virtual void onRunning(const AppData * /*unused*/) {}
    virtual void onRunningBackground(const AppData * /*unused*/) {}
    virtual void onPause(const AppData * /*unused*/) {}
    virtual void onDestroy(const AppData * /*unused*/) {}

    FSMAction<App, AppData, &App::onCreate> app_create;
    FSMAction<App, AppData, &App::onResume> app_resume;
    FSMAction<App, AppData, &App::onRunning> app_running;
    FSMAction<App, AppData, &App::onRunningBackground> app_running_background;
    FSMAction<App, AppData, &App::onPause> app_pause;
    FSMAction<App, AppData, &App::onDestroy> app_destroy;

private:
    const FSMActionTable *getActionTable() override
    {
        // clang-format off
        static const FSMActionTable state_action_table[] = {
            {&app_create},
            {&app_resume},
            {&app_running},
            {&app_running_background},
            {&app_pause},
            {&app_destroy},
        };
        return &state_action_table[0];
        // clang-format on
    }

private:
    AppData *inner_data_; // You can get inner resource of this app
    // special bits:
    // - 0 bit: goto start
    // - 1 bit: goto close
    // - 2 bit: goto destroy
    // - 4 bit: background running permission
    uint8_t special_bits_{0};
};

} // namespace crisp

#endif // CORE_APP_APP_H