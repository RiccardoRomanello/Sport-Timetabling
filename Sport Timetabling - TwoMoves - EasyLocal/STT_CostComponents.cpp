#include "STT_CostComponents.hh"

int RePlayedMatchesCostComponent::ComputeCost(const STT_State& state) const{
    int violations = 0; 
    for(unsigned t = 0; t < in.Teams(); t++){
        for(unsigned t1 = t + 1; t1 < in.Teams(); t1++){
            //each mach t v t1 does no have to be recounted
            violations += max(0, static_cast<int>(state.HowManyMatchesPlayedAgainst(t, t1)) - 1);
        }
    }
    return violations;
}

void RePlayedMatchesCostComponent::PrintViolations(const STT_State& state, ostream& os) const{
    
    for(unsigned t = 0; t < in.Teams(); t++){
        for(unsigned t1 = t + 1; t1 < in.Teams(); t1++){
            int wrongMatches = static_cast<int>(state.HowManyMatchesPlayedAgainst(t, t1)) - 1;
            if(wrongMatches > 0){
                os << "team " << t << " and team " << t1 << " are playing " << wrongMatches << " that should not exist" << endl;
            }
        }
    }
}

int NoMatchesPlayedCostComponent::ComputeCost(const STT_State& state) const{
    int violations = 0; 

    for(unsigned t = 0; t < in.Teams() - 1; t++){
        for(unsigned t1 = t + 1; t1 < in.Teams(); t1++){
            //a match t v t1 never played, is never played once, not twice (so no t1 = 0 ... but t1 = t + 1)
            if(state.HowManyMatchesPlayedAgainst(t, t1) == 0){
                violations += 1;
            }
        }
    }

    return violations;
}

void NoMatchesPlayedCostComponent::PrintViolations(const STT_State& state, ostream& os) const{
    
    for(unsigned t = 0; t < in.Teams(); t++){
        for(unsigned t1 = t + 1; t1 < in.Teams(); t1++){
            if(state.HowManyMatchesPlayedAgainst(t, t1) == 0){
                os << "Match " << t << " v " << t1 << " is never played;" << endl;
            }
        }
    }
}

int BigMatchesPerMatchDayCostComponent::ComputeCost(const STT_State& state) const{

    int violations = 0;  
    unsigned currentMatchDayBigMatches = 0; 

    for(unsigned mDay = 0; mDay < in.MatchDays(); mDay++){
        currentMatchDayBigMatches = 0; 
        for(unsigned t = 0; t < in.Teams(); t++){
            if(!state.AtHome(t, mDay)){continue;}// no double counting in the same matchday

            unsigned againstTeam = state.GetOpponentForTeamInMatchday(t, mDay);
            
            if(in.TopTeam(t) && in.TopTeam(againstTeam)){
                currentMatchDayBigMatches += 1;
            }
        }

        if(currentMatchDayBigMatches > in.BigMatches()){
            violations++;
        }
    }

    return violations;
}

void BigMatchesPerMatchDayCostComponent::PrintViolations(const STT_State& state, ostream& os) const{
    unsigned currentMatchDayBigMatches = 0; 

    for(unsigned mDay = 0; mDay < in.MatchDays(); mDay++){
        currentMatchDayBigMatches = 0; 
        for(unsigned t = 0; t < in.Teams(); t++){
            if(!state.AtHome(t, mDay)){continue;}// no double counting in the same matchday
            unsigned againstTeam = state.GetOpponentForTeamInMatchday(t, mDay);
            
            if(in.TopTeam(t) && in.TopTeam(againstTeam)){
                currentMatchDayBigMatches += 1;
            }
        }

        if(currentMatchDayBigMatches > in.BigMatches()){
            os << "Matchday " << mDay << " has " << currentMatchDayBigMatches << " big matches while the limit is " << in.BigMatches() << endl;
        }
    }

}

int ForbiddenInMatchDayCostComponent::ComputeCost(const STT_State& state) const{

    int violations = 0; 
    
    for(unsigned mDay = 0; mDay < in.MatchDays(); mDay++){
        for(unsigned t = 0; t < in.Teams(); t++){
            if(!state.AtHome(t, mDay)){continue;}// no double counting in the same matchday

            unsigned matchId = state.GetMatchIDForTeamInMatchDay(t, mDay);
            
            if(in.ForbiddenInAMatchDay(mDay, matchId)){
                violations++;
            }
        }
    }

    return violations;
}

void ForbiddenInMatchDayCostComponent::PrintViolations(const STT_State& state, ostream& os) const{


    for(unsigned mDay = 0; mDay < in.MatchDays(); mDay++){
        for(unsigned t = 0; t < in.Teams(); t++){
            if(!state.AtHome(t, mDay)){continue;}// no double counting in the same matchday
            unsigned secondTeam = state.GetOpponentForTeamInMatchday(t, mDay);

            unsigned matchId = state.GetMatchIDForTeamInMatchDay(t, mDay);
            
            if(in.ForbiddenInAMatchDay(mDay, matchId)){
                os << "Match with id " << matchId << " (" << t << ", " << secondTeam << ") is forbidden in matchday " << mDay << endl;
            }
        }
    }
}

int ComplementaryTeamsCostComponent::ComputeCost(const STT_State& state) const {
    
    int violations = 0;

    for(unsigned mDay = 0; mDay < in.MatchDays(); mDay++){
        for(unsigned t = 0; t < in.Teams(); t++){
            int complementOfT = in.ComplementOf(t);
            if(complementOfT != -1 && t > complementOfT){ //avoid double countings (and only for teams with actual complements)
                if(state.AtHome(t, mDay) && state.AtHome(complementOfT, mDay)){
                    violations++;
                }
            }
        }
    }

    return violations;
}

void ComplementaryTeamsCostComponent::PrintViolations(const STT_State& state, ostream& os) const {

    for(unsigned mDay = 0; mDay < in.MatchDays(); mDay++){
        for(unsigned t = 0; t < in.Teams(); t++){
            int complementOfT = in.ComplementOf(t);
            if(complementOfT != -1 && complementOfT > t){ //avoid wrong and double counting
                if(state.AtHome(t, mDay) && state.AtHome(complementOfT, mDay)){
                    os << "Both " << t << " and " << complementOfT << " are playing at home" << endl;
                }
            }
        }
    }
}

int SequenceOfHomeGamesCostComponent::ComputeCost(const STT_State& state) const {
    
    int violations = 0;
    int firstAtHome = -1;


    for(unsigned t = 0; t < in.Teams(); t++){
        for(unsigned mDay = 0; mDay < in.TotalMatchDays(); mDay++){
            if(state.AtHome(t, mDay) && firstAtHome == -1){
                firstAtHome = mDay;
            }

            if(!state.AtHome(t, mDay)){
               if(firstAtHome != -1){
                   int currentSeqLength = mDay - firstAtHome;
                   if(currentSeqLength > in.LongestSeqAtHome(t)){
                       violations += (currentSeqLength - in.LongestSeqAtHome(t));
                   }
                   firstAtHome = -1;
               }
            }
        }
        if(firstAtHome != -1){ //there is an unhadled seq of home games
            if(in.TotalMatchDays() - firstAtHome > in.LongestSeqAtHome(t)){
                violations += ((in.TotalMatchDays() - firstAtHome) - in.LongestSeqAtHome(t));
            }
        }

        firstAtHome = -1;
    }

    return violations;
}

void SequenceOfHomeGamesCostComponent::PrintViolations(const STT_State& state, ostream& os) const {
    int firstAtHome = -1;


    for(unsigned t = 0; t < in.Teams(); t++){
        for(unsigned mDay = 0; mDay < in.TotalMatchDays(); mDay++){
            if(state.AtHome(t, mDay) && firstAtHome == -1){
                firstAtHome = mDay;
            }

            if(!state.AtHome(t, mDay)){
               if(firstAtHome != -1){
                   int currentSeqLength = mDay - firstAtHome;
                   if(currentSeqLength > in.LongestSeqAtHome(t)){
                       cout << "Team " << t << " is playing a too long sequence of home games (" << firstAtHome << ", " << (mDay - 1) << ")" << endl; 
                   }
                   firstAtHome = -1;
               }
            }
        }
        if(firstAtHome != -1){ //there is an unhadled seq of home games
            if(in.TotalMatchDays() - firstAtHome > in.LongestSeqAtHome(t)){
                cout << "Team " << t << " is playing a too long sequence of home games (" << firstAtHome << ", " << (in.TotalMatchDays() - 1) << ")" << endl; 
            }
        }
        firstAtHome = -1;
    }

}

int SequenceOfAwayGamesCostComponent::ComputeCost(const STT_State& state) const {

    int violations = 0;
    int firstAway = -1;


    for(unsigned t = 0; t < in.Teams(); t++){
        for(unsigned mDay = 0; mDay < in.TotalMatchDays(); mDay++){
            if(!state.AtHome(t, mDay) && firstAway == -1){
                firstAway = mDay;
            }

            if(state.AtHome(t, mDay)){
               if(firstAway != -1){
                   int currentSeqLength = mDay - firstAway;
                   if(currentSeqLength > in.LongestSeqAway(t)){
                       violations += (currentSeqLength - in.LongestSeqAway(t));
                   }
                   firstAway = -1;
               }
            }
        }
        if(firstAway != -1){ //there is an unhadled seq of away games
            if(in.TotalMatchDays() - firstAway > in.LongestSeqAway(t)){
                violations += ((in.TotalMatchDays() - firstAway) - in.LongestSeqAway(t));
            }
        }
        firstAway = -1;
    }

    return violations;
}

void SequenceOfAwayGamesCostComponent::PrintViolations(const STT_State& state, ostream& os) const {
    int firstAway = -1;


    for(unsigned t = 0; t < in.Teams(); t++){
        for(unsigned mDay = 0; mDay < in.TotalMatchDays(); mDay++){
            if(!state.AtHome(t, mDay) && firstAway == -1){
                firstAway = mDay;
            }

            if(state.AtHome(t, mDay)){
                if(firstAway != -1){
                    int currentSeqLength = mDay - firstAway;
                    if(currentSeqLength > in.LongestSeqAway(t)){
                       cout << "Team " << t << " is playing a too long sequence of away games (" << firstAway << ", " << (mDay - 1) << ")" << endl; 
                    }
                    firstAway = -1;
                }
            }
        }
        if(firstAway != -1){ //there is an unhadled seq of away games
            if(in.TotalMatchDays() - firstAway > in.LongestSeqAway(t)){
                cout << "Team " << t << " is playing a too long sequence of away games (" << firstAway << ", " << (in.TotalMatchDays() - 1) << ")" << endl; 
            }
        }
        firstAway = -1;
    }

}

int ForbiddenTogetherCostComponent::ComputeCost(const STT_State& state) const {

    int violations = 0; 

    vector<unsigned> matchIds; //current matchday matchids
    for(int mDay = 0; mDay < in.TotalMatchDays(); mDay++){

        for(int t = 0; t < in.Teams(); t++){
            if(!state.AtHome(t, mDay)){continue;}
            matchIds.push_back(state.GetMatchIDForTeamInMatchDay(t, mDay));
        }

        for(int id1 = 0; id1 < matchIds.size() - 1; id1++){
            for(int id2 = id1 + 1; id2 < matchIds.size(); id2++){
                if(in.ForbiddenTogether(matchIds[id1], matchIds[id2])){
                    violations++;
                }
            }
        }
        matchIds.clear();
    }

    return violations;
}

void ForbiddenTogetherCostComponent::PrintViolations(const STT_State& state, ostream& os) const {

    for(int mDay = 0; mDay < in.TotalMatchDays(); mDay++){

        vector<unsigned> matchIds; 
        for(int t = 0; t < in.Teams(); t++){
            if(!state.AtHome(t, mDay)){continue;}
            matchIds.push_back(state.GetMatchIDForTeamInMatchDay(t, mDay));
        }

        for(int id1 = 0; id1 < matchIds.size() - 1; id1++){
            for(int id2 = id1 + 1; id2 < matchIds.size(); id2++){
                if(in.ForbiddenTogether(matchIds[id1], matchIds[id2])){
                    os << "Match id " << matchIds[id1] << " and " << matchIds[id2] << " are forbidden together; matchday (" << mDay << ")" << endl;  
                }
            }
        }

    }
}

int ShouldPlayAtHomeCostComponent::ComputeCost(const STT_State& state) const {

    int violations = 0;

    for(int t = 0; t < in.Teams(); t++){

        for(int m = 0; m < in.TotalMatchDays(); m++){
            if(!state.AtHome(t, m) && in.ShouldPlayAtHome(t, m)){
                violations++;
            }
        }
    }
    
    return violations;
}

void ShouldPlayAtHomeCostComponent::PrintViolations(const STT_State& state, ostream& os) const {
    
    for(int t = 0; t < in.Teams(); t++){
        
        for(int m = 0; m < in.TotalMatchDays(); m++){
            if(!state.AtHome(t, m) && in.ShouldPlayAtHome(t, m)){
                os << "Team " << t << " should play matchday " << m << " at home but it is not" << endl;
            }
        }
    }

}