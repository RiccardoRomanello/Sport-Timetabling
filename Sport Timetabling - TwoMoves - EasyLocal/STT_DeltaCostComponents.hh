#ifndef _STT_DELTA_COST_COMPONENTS_HH_
#define _STT_DELTA_COST_COMPONENTS_HH_

#include "STT_Input.hh"
#include "STT_State.hh"
#include "STT_Move.hh"
#include "STT_CostComponents.hh"
#include <easylocal.hh>
using namespace EasyLocal::Core;

/* Replayed matches */

class SwapRePlayedMatchesDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapRePlayedMatchesDeltaCostComponent(const STT_Input& in, RePlayedMatchesCostComponent& rpmCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, rpmCC, "RePlayedMatchesDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHRePlayedMatchesDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHRePlayedMatchesDeltaCostComponent(const STT_Input& in, RePlayedMatchesCostComponent& rpmCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, rpmCC, "RePlayedMatchesDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};

/* No matches played */

class SwapNoMatchesPlayedDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapNoMatchesPlayedDeltaCostComponent(const STT_Input& in, NoMatchesPlayedCostComponent& npmCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, npmCC, "NoMatchesPlayedDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHNoMatchesPlayedDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHNoMatchesPlayedDeltaCostComponent(const STT_Input& in, NoMatchesPlayedCostComponent& npmCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, npmCC, "NoMatchesPlayedDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};

/* Big matches per matchday */

class SwapBigMatchesPerMatchDayDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapBigMatchesPerMatchDayDeltaCostComponent(const STT_Input& in, BigMatchesPerMatchDayCostComponent& bmCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, bmCC, "BigMatchesPerMatchDayDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHBigMatchesPerMatchDayDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHBigMatchesPerMatchDayDeltaCostComponent(const STT_Input& in, BigMatchesPerMatchDayCostComponent& bmCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, bmCC, "BigMatchesPerMatchDayDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};

/* Forbidden in matchday */

class SwapForbiddenInMatchDayDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapForbiddenInMatchDayDeltaCostComponent(const STT_Input& in, ForbiddenInMatchDayCostComponent& fmCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, fmCC, "ForbiddenInMatchDayDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHForbiddenInMatchDayDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHForbiddenInMatchDayDeltaCostComponent(const STT_Input& in, ForbiddenInMatchDayCostComponent& fmCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, fmCC, "ForbiddenInMatchDayDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};

/* Complementary teams */

class SwapComplementaryTeamsDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapComplementaryTeamsDeltaCostComponent(const STT_Input& in, ComplementaryTeamsCostComponent& ctCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, ctCC, "ComplementaryTeamsDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHComplementaryTeamsDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHComplementaryTeamsDeltaCostComponent(const STT_Input& in, ComplementaryTeamsCostComponent& ctCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, ctCC, "ComplementaryTeamsDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};

/* Sequence of home games */

class SequenceOfHomeGamesUtility {
    public:
        static int JoinHomeSequences (int team, int matchDay, int bound, const STT_State& state) ;
        static int SplitHomeSequences (int team, int matchDay, int bound, const STT_State& state)  ;
};

class SwapSequenceOfHomeGamesDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapSequenceOfHomeGamesDeltaCostComponent(const STT_Input& in, SequenceOfHomeGamesCostComponent& shgCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, shgCC, "SequenceOfHomeGamesDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHSequenceOfHomeGamesDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHSequenceOfHomeGamesDeltaCostComponent(const STT_Input& in, SequenceOfHomeGamesCostComponent& shgCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, shgCC, "SequenceOfHomeGamesDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};

/* Sequence of away games */

class SequenceOfAwayGamesUtility {
    public : 
        static int JoinAwaySequences (int team, int matchDay, int bound, const STT_State& state) ;
        static int SplitAwaySequences (int team, int matchDay, int bound, const STT_State& state) ;

};

class SwapSequenceOfAwayGamesDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapSequenceOfAwayGamesDeltaCostComponent(const STT_Input& in, SequenceOfAwayGamesCostComponent& sagCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, sagCC, "SequenceOfAwayGamesDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHSequenceOfAwayGamesDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHSequenceOfAwayGamesDeltaCostComponent(const STT_Input& in, SequenceOfAwayGamesCostComponent& sagCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, sagCC, "SequenceOfAwayGamesDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};

/* Forbidden together */

class SwapForbiddenTogetherDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapForbiddenTogetherDeltaCostComponent(const STT_Input& in, ForbiddenTogetherCostComponent& ftCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, ftCC, "ForbiddenTogetherDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHForbiddenTogetherDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHForbiddenTogetherDeltaCostComponent(const STT_Input& in, ForbiddenTogetherCostComponent& ftCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, ftCC, "ForbiddenTogetherDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};

/* Should play at home*/

class SwapShouldPlayAtHomeDeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        SwapShouldPlayAtHomeDeltaCostComponent(const STT_Input& in, ShouldPlayAtHomeCostComponent& sphCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, sphCC, "ShouldPlayAtHomeDeltaCostComponent for Swap move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const; 
};

class CHShouldPlayAtHomeDeltaCostComponent: public DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>
{
    public: 
        CHShouldPlayAtHomeDeltaCostComponent(const STT_Input& in, ShouldPlayAtHomeCostComponent& sphCC) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_ChangeHospitality>(in, sphCC, "ShouldPlayAtHomeDeltaCostComponent for Change Hospitality move")  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const; 
};








//--------------------------------------------------------------------------------------------------------------------------------------

class STT_DeltaCostComponent : public DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>
{
    public: 
        STT_DeltaCostComponent(const STT_Input& in, CostComponent<STT_Input, STT_State>& cc, string name) 
        : DeltaCostComponent<STT_Input, STT_State, STT_Move_Swap>(in, cc, name)  {}
        int ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const;
        virtual int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const = 0;
        virtual int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const = 0;   
};

class RePlayedMatchesDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    RePlayedMatchesDeltaCostComponent(const STT_Input& in, RePlayedMatchesCostComponent& rpmCC)
    : STT_DeltaCostComponent(in, rpmCC, "RePlayedMatchesDeltaCostComponent")  {}
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
};

class NoMatchesPlayedDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    NoMatchesPlayedDeltaCostComponent(const STT_Input& in, NoMatchesPlayedCostComponent& npmCC)
    : STT_DeltaCostComponent(in, npmCC, "NoMatchesPlayedDeltaCostComponent")  {}
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
};

class BigMatchesPerMatchDayDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    BigMatchesPerMatchDayDeltaCostComponent(const STT_Input& in, BigMatchesPerMatchDayCostComponent& bmCC)
    : STT_DeltaCostComponent(in, bmCC, "BigMatchesDeltaCostComponent")  {}
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
};

class ForbiddenInMatchDayDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    ForbiddenInMatchDayDeltaCostComponent(const STT_Input& in, ForbiddenInMatchDayCostComponent& fmCC)
    : STT_DeltaCostComponent(in, fmCC, "ForbiddenMatchDayDeltaCostComponent")  {}
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
};

class ComplementaryTeamsDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    ComplementaryTeamsDeltaCostComponent(const STT_Input& in, ComplementaryTeamsCostComponent& ctCC)
    : STT_DeltaCostComponent(in, ctCC, "ComplementaryTeamsDeltaCostComponent")  {}
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
}; 

class SequenceOfHomeGamesDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    SequenceOfHomeGamesDeltaCostComponent(const STT_Input& in, SequenceOfHomeGamesCostComponent& shgCC)
    : STT_DeltaCostComponent(in, shgCC, "SequenceOfHomeGamesDeltaCostComponent")  {}
    int JoinHomeSequences (int team, int matchDay, const STT_State& state) const ;
    int SplitHomeSequences (int team, int matchDay, const STT_State& state) const ;
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
};  

class SequenceOfAwayGamesDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    SequenceOfAwayGamesDeltaCostComponent(const STT_Input& in, SequenceOfAwayGamesCostComponent& sagCC)
    : STT_DeltaCostComponent(in, sagCC, "SequenceOfAwayGamesDeltaCostComponent")  {}
    int JoinAwaySequences (int team, int matchDay, const STT_State& state) const ;
    int SplitAwaySequences (int team, int matchDay, const STT_State& state) const ;
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
};  

class ForbiddenTogetherDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    ForbiddenTogetherDeltaCostComponent(const STT_Input& in, ForbiddenTogetherCostComponent& ftCC)
    : STT_DeltaCostComponent(in, ftCC, "ForbiddenTogetherDeltaCostComponent")  {}
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
};  

class ShouldPlayAtHomeDeltaCostComponent : public STT_DeltaCostComponent
{
public:
    ShouldPlayAtHomeDeltaCostComponent(const STT_Input& in, ShouldPlayAtHomeCostComponent& sphCC)
    : STT_DeltaCostComponent(in, sphCC, "ShouldPlayAtHomeDeltaCostComponent")  {}
    int DeltaForSwapTeam(const STT_State& state, const STT_Move_Swap& move) const;
    int DeltaForChangeOspitality(const STT_State& state, const STT_Move_Swap& move) const;
};  


#endif