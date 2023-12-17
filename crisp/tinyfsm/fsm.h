#ifndef CRISP_TINYFSM_FSM_H
#define CRISP_TINYFSM_FSM_H

#include <cassert>
#include <cstdint>

namespace crisp
{

class EventData
{
public:
    virtual ~EventData() = default;
};
using NoEventData = EventData;

class TinyFSM;
class FSMActionBase
{
public:
    /** Called by the state machine engine to execute a state action. */
    virtual void execute(TinyFSM *fsm, const EventData *data) const = 0;
};

/** FSMAction takes three template arguments:
 *  - A state machine class
 *  - a state function event data type (derived from EventData)
 *  - a state machine member function pointer.
 */
template <class FSM, class Data, void (FSM::*Func)(const Data *)>
class FSMAction : public FSMActionBase
{
public:
    void execute(TinyFSM *fsm, const EventData *data) const override
    {
        // Downcast the state machine and event data to the correct derived type
        auto derived_fsm = static_cast<FSM *>(fsm);

        // dynamic_cast is used to check whether the type between
        // `Data` and `EventData` are the same or not
        const Data *derived_data = dynamic_cast<const Data *>(data);
        assert(derived_data != nullptr);

        (derived_fsm->*Func)(derived_data);
    }
};

struct FSMActionTable
{
    const FSMActionBase *const action;
};

class TinyFSM
{
public:
    using StateType = uint8_t;

    enum : uint8_t
    {
        EVENT_IGNORED = 0xFE,
        EVENT_CANNOT_HAPPEN
    };

    explicit TinyFSM(StateType max_states_num, StateType initial_state = 0);

    virtual ~TinyFSM() = default;

    StateType getCurrentState() { return current_state_; }
    uint8_t getMaxStatesNum() { return max_states_num_; }

protected:
    /** External state machine event. */
    void externalEvent(StateType new_state, const EventData *pData = nullptr);

    /**
     * Internal state machine event. These events are generated while executing
     * within a state machine state.
     */
    void internalEvent(StateType new_state, const EventData *pData = nullptr);

private:
    void setCurrentState(StateType new_state) { current_state_ = new_state; }
    /**
     * @brief Gets the state action table as defined in the derived class.
     * @return An array of `FSMActionTable` pointers with the array size max_states_num
     */
    virtual const FSMActionTable *getActionTable() = 0;

    /**
     * State machine engine that executes the external event and, optionally, all
     * internal events generated during state execution.
     */
    void stateEngine();
    void stateEngine(const FSMActionTable *pActionTable);

private:
    const uint8_t max_states_num_;
    StateType current_state_;        // The current state machine state.
    StateType new_state_;            // The new state the state machine has yet to transition to.
    bool is_event_generated_{false}; // Set to TRUE when an event is generated.
    const EventData *pEvent_data_{nullptr}; // The state event data pointer.
};

} // namespace crisp

#endif // CRISP_TINYFSM_FSM_H