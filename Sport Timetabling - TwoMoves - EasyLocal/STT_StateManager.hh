#ifndef _STT_STATE_MANAGER_HH_
#define _STT_STATE_MANAGER_HH_

#include "STT_State.hh"
#include "STT_Input.hh"

#include <easylocal.hh>

using namespace EasyLocal::Core;

class STT_StateManager : public StateManager<STT_Input, STT_State> {
    public: 
        STT_StateManager(const STT_Input& in) : StateManager<STT_Input, STT_State>(in, "STT_StateManager") {}
        void RandomState(STT_State &state);
        bool CheckConsistency(const STT_State &state) const;

};

#endif