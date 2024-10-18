#ifndef _STT_NEIGH_EXPL_HH
#define _STT_NEIGH_EXPL_HH

#include "STT_Input.hh"
#include "STT_State.hh"
#include "STT_Move.hh"
#include <easylocal.hh>

using namespace EasyLocal::Core;

class STT_SwapNeighborhoodExplorer
  :  public NeighborhoodExplorer<STT_Input, STT_State, STT_Move_Swap>
{
public:
  STT_SwapNeighborhoodExplorer(const STT_Input& in, 
                     StateManager<STT_Input, STT_State >& sttsm)
    : NeighborhoodExplorer<STT_Input,STT_State,STT_Move_Swap>(in, sttsm, "STT_SwapNeighborhoodExplorer")
  {}
  void MakeMove(STT_State &state, const STT_Move_Swap& move) const;
  void RandomMove(const STT_State &state, STT_Move_Swap& move) const;
  void FirstMove(const STT_State &state, STT_Move_Swap& move) const;
  bool NextMove(const STT_State &state, STT_Move_Swap& move) const;
  bool FeasibleMove(const STT_State &state, const STT_Move_Swap& move) const;
};

class STT_ChangeHospitalityNeighborhoodExplorer
  :  public NeighborhoodExplorer<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
public:
  STT_ChangeHospitalityNeighborhoodExplorer(const STT_Input& in, 
                     StateManager<STT_Input, STT_State >& sttsm)
    : NeighborhoodExplorer<STT_Input,STT_State,STT_Move_ChangeHospitality>(in, sttsm, "STT_ChangeHospitalityNeighborhoodExplorer")
  {}
  void MakeMove(STT_State &state, const STT_Move_ChangeHospitality& move) const;
  void RandomMove(const STT_State &state, STT_Move_ChangeHospitality& move) const;
  void FirstMove(const STT_State &state, STT_Move_ChangeHospitality& move) const;
  bool NextMove(const STT_State &state, STT_Move_ChangeHospitality& move) const;
  bool FeasibleMove(const STT_State &state, const STT_Move_ChangeHospitality& move) const;
};


#endif