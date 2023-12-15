#ifndef CORE_APP_APP_H
#define CORE_APP_APP_H

#include "tinyfsm/fsm.h"

namespace crisp
{

class AppInner
{
};

/**
 * @brief Contains states for life cycle control (FSM, Finite-Sate Machine)
 */
class App : public TinyFSM
{

public:
    /* Lifecycle methods */
    /* ref: <https://developer.android.com/guide/components/activities/activity-lifecycle> */
    virtual void onCreate(){};
    virtual void onStart(){};
    virtual void onResume(){};
    virtual void onPause(){};
    virtual void onStop(){};
    virtual void onDestroy(){};

private:
    enum States
    {
        ON_CREATE,
        ON_START,
        ON_RESUME,
        ON_PAUSE,
        ON_STOP,
        ON_DESTROY,
        APP_MAX_STATES,
    };
};

} // namespace crisp

#endif // CORE_APP_APP_H