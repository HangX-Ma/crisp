#include "app/app.h"
#include "tinyfsm/fsm.h"

namespace crisp
{

void App::create(const AppData *data) { externalEvent(APP_ON_CREATE, data); }

void App::start(const AppData *data)
{
    static const StateType transitions[] = {
        APP_ON_RESUME, // ON_CREATE
        EVENT_IGNORED, // ON_RESUME
        EVENT_IGNORED, // ON_RUNNING
        APP_ON_RESUME, // ON_RUNNING_BACKGROUND
        APP_ON_RESUME, // ON_PAUSE
        EVENT_IGNORED, // ON_DESTROY
    };
    externalEvent(transitions[getCurrentState()], data);
    assert((sizeof(transitions) / sizeof(StateType)) == APP_MAX_STATES);
}

void App::close()
{
    static const StateType transitions[] = {
        EVENT_IGNORED, // ON_CREATE
        APP_ON_PAUSE,  // ON_RESUME
        APP_ON_PAUSE,  // ON_RUNNING
        EVENT_IGNORED, // ON_RUNNING_BACKGROUND
        EVENT_IGNORED, // ON_PAUSE
        EVENT_IGNORED, // ON_DESTROY
    };
    externalEvent(transitions[getCurrentState()]);
    assert((sizeof(transitions) / sizeof(StateType)) == APP_MAX_STATES);
}

void App::destroy()
{
    close();
    static const StateType transitions[] = {
        APP_ON_DESTROY, // ON_CREATE
        CANNOT_HAPPEN,  // ON_RESUME
        CANNOT_HAPPEN,  // ON_RUNNING
        APP_ON_DESTROY, // ON_RUNNING_BACKGROUND
        APP_ON_DESTROY, // ON_PAUSE
        EVENT_IGNORED,  // ON_DESTROY
    };
    externalEvent(transitions[getCurrentState()]);
    assert((sizeof(transitions) / sizeof(StateType)) == APP_MAX_STATES);
}

void App::update()
{
    static const StateType transitions[] = {
        EVENT_IGNORED,             // ON_CREATE
        APP_ON_RUNNING,            // ON_RESUME
        APP_ON_RUNNING,            // ON_RUNNING
        APP_ON_RUNNING_BACKGROUND, // ON_RUNNING_BACKGROUND
        APP_ON_RUNNING_BACKGROUND, // ON_PAUSE
        APP_ON_DESTROY,            // ON_DESTROY
    };
    externalEvent(transitions[getCurrentState()]);
    assert((sizeof(transitions) / sizeof(StateType)) == APP_MAX_STATES);
}

}; // namespace crisp