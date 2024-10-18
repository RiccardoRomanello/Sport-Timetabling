#ifndef _STT_OUTPUT_MANAGER_HH
#define _STT_OUTPUT_MANAGER_HH

#include "STT_Input.hh"
#include "STT_Output.hh"
#include "STT_State.hh"

#include <easylocal.hh>

using namespace EasyLocal::Core;

class STT_OutputManager
  : public OutputManager<STT_Input, STT_Output, STT_State>
{
public:
  STT_OutputManager(const STT_Input& i)
    : OutputManager<STT_Input,STT_Output,STT_State>(i, "STT_OutputManager") {}
  void OutputState(const STT_State &st, STT_Output& out) const;
  void InputState(STT_State &st, const STT_Output& out) const;
};

#endif