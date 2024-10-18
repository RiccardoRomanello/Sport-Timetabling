#ifndef _STT_COST_COMPONENTS_HH_
#define _STT_COST_COMPONENTS_HH_

#include "STT_Input.hh"
#include "STT_State.hh"
#include "STT_Move.hh"
#include <easylocal.hh>
using namespace EasyLocal::Core;

class RePlayedMatchesCostComponent : public CostComponent<STT_Input, STT_State>
{
    public: 
        RePlayedMatchesCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 10, true, "ReplayedMatches") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

};

class NoMatchesPlayedCostComponent : public CostComponent<STT_Input, STT_State>
{
    public: 
        NoMatchesPlayedCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 10, true, "NoPlayedMatches") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

}; 

class BigMatchesPerMatchDayCostComponent : public CostComponent<STT_Input, STT_State> 
{
    public: 
        BigMatchesPerMatchDayCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 10, true, "BigMatchesPerMatchDay") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

};

class ForbiddenInMatchDayCostComponent : public CostComponent<STT_Input, STT_State> 
{
    public: 
        ForbiddenInMatchDayCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 10, true, "ForbiddenInMatchDay") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

};

class ComplementaryTeamsCostComponent : public CostComponent<STT_Input, STT_State> 
{
    public: 
        ComplementaryTeamsCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 10, true, "ComplementaryTeams") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

};

class SequenceOfHomeGamesCostComponent : public CostComponent<STT_Input, STT_State> 
{
    public: 
        SequenceOfHomeGamesCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 10, true, "SeqOfHomeGames") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

};

class SequenceOfAwayGamesCostComponent : public CostComponent<STT_Input, STT_State> 
{
    public: 
        SequenceOfAwayGamesCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 10, true, "SeqOfAwayGames") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

};

class ForbiddenTogetherCostComponent : public CostComponent<STT_Input, STT_State> 
{
    public: 
        ForbiddenTogetherCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 1, false, "ForbiddenTogether") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

};

class ShouldPlayAtHomeCostComponent : public CostComponent<STT_Input, STT_State> 
{
    public: 
        ShouldPlayAtHomeCostComponent(const STT_Input& in) 
            : CostComponent<STT_Input, STT_State> (in, 1, false, "ShouldPlayAtHome") {}
        int ComputeCost(const STT_State& state) const;
        void PrintViolations(const STT_State& state, ostream& os = cout) const;

};
#endif
