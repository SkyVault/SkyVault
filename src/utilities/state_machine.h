#ifndef SKYVAULT_STATE_MACHINE_H
#define SKYVAULT_STATE_MACHINE_H

struct SM {
    inline SM(int start) : state(start) { }

    inline int CurrentState(){ return state; }
    inline void SetState(int s) { 
        last = state;
        state = s;
    }

    inline int LastState()      { return last; } 
    inline bool JustSwitched()  { return last != state; } 
    inline void Tick()          { last = state; }

private:
    int state{0};
    int last{0};
};

#endif//SKYVAULT_STATE_MACHINE_H 
