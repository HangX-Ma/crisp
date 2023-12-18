#include "tinyfsm/fsm.h"
#include <cassert>

namespace crisp
{

TinyFSM::TinyFSM(StateType max_states_num, StateType initial_state)
    : max_states_num_(max_states_num), current_state_(initial_state), new_state_(initial_state)
{
    // runtime correctness check
    assert(max_states_num_ < EVENT_IGNORED);
}

void TinyFSM::externalEvent(StateType new_state, const EventData *pData)
{
    // If we are supposed to ignore this event
    if (new_state == EVENT_IGNORED) {
        delete pData; // delete a null pointer not affect anything
    }
    else {
        // Generate the event
        internalEvent(new_state, pData);
        // Execute the state engine. This function call will only return
        // when all state machine events are processed.
        stateEngine();
    }
}

void TinyFSM::internalEvent(StateType new_state, const EventData *pData)
{
    if (pData == nullptr) {
        pData = new NoEventData();
    }

    pEvent_data_ = pData;
    is_event_generated_ = true;
    new_state_ = new_state;
}

void TinyFSM::stateEngine()
{
    const FSMActionTable *action_tbl_ptr = getActionTable();
    if (action_tbl_ptr != nullptr) {
        stateEngine(action_tbl_ptr);
    }
}

void TinyFSM::stateEngine(const FSMActionTable *pActionTable)
{
    const EventData *data_ptr = nullptr;

    // While events are being generated keep executing states
    while (is_event_generated_) {
        assert(new_state_ < max_states_num_);

        // Get the pointer from the action table
        const FSMActionBase *action = pActionTable[new_state_].action;
        data_ptr = pEvent_data_;     // Copy of event data pointer
        pEvent_data_ = nullptr;      // Event data used up, reset the pointer
        is_event_generated_ = false; // Event used up, reset the flag

        // Switch to the new current state
        setCurrentState(new_state_);

        // Execute the state action passing in event data
        assert(action != nullptr);
        action->execute(this, data_ptr);

        // If event data was used, then delete it
        if (data_ptr != nullptr) {
            delete data_ptr;
            data_ptr = nullptr;
        }
    }
}

} // namespace crisp