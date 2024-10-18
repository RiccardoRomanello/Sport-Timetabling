#include "STT_MatchDay.hh"

STT_MatchDay::STT_MatchDay(const STT_Input& i) :
    in(i),
    matchDay()
{
    this->bigMatches = 0;
    matchDay.reserve(in.Teams());
    for(unsigned i = 0; i < in.Teams(); i++){
        //all matches are of the form (i v in.Teams)
        matchDay.push_back(STT_Match(i, in.Teams(), true));
    }
}

STT_MatchDay& STT_MatchDay::operator=(const STT_MatchDay& md){
    matchDay = md.matchDay;
    bigMatches = md.bigMatches;

    return *this;
}

void STT_MatchDay::UpdateBigMatchesCount(unsigned team, int bias){
    //bias is +1 for the new pairings, while -1 to remove pairings
    unsigned opponent = OpponentOfTeam(team);
    if(team < in.Teams() && opponent < in.Teams()){
        if(in.TopTeam(team) && in.TopTeam(opponent)){
            bigMatches += bias;
        }
    }
}

int STT_MatchDay::GetMatchIdForTeam(unsigned team) const {
    unsigned opponent = OpponentOfTeam(team);

    if(!PlaysAtHome(team)){
        swap(team, opponent);
    }

    STT_Util util(in.Teams());
    
    return util.GetMatchId(team, opponent);
}

void STT_MatchDay::SetOpponentForTeam(unsigned team, unsigned opponent){

    UpdateBigMatchesCount(team, -1); // eventually remove 1

    matchDay[team].ChangeOpponent(opponent);
    matchDay[opponent].ChangeOpponent(team);

    UpdateBigMatchesCount(team, +1); // eventually add 1

}

void STT_MatchDay::SetOspitalityForTeam(unsigned team, bool isAtHome){

    matchDay[team].ChangeHospitality(isAtHome);
    matchDay[OpponentOfTeam(team)].ChangeHospitality(!isAtHome);

}

void STT_MatchDay::SetOpponentForTeam(unsigned team, unsigned opponent, bool isFirstTeamAtHome){
    SetOpponentForTeam(team, opponent);
    SetOspitalityForTeam(team, isFirstTeamAtHome);
}

void STT_MatchDay::SwapTeams(unsigned firstTeam, unsigned secondTeam){
    unsigned formerFirstTeamOpponent, formerSecondTeamOpponent;
    
    formerFirstTeamOpponent = OpponentOfTeam(firstTeam);
    formerSecondTeamOpponent = OpponentOfTeam(secondTeam);

    SetOpponentForTeam(firstTeam, formerSecondTeamOpponent);
    SetOpponentForTeam(secondTeam, formerFirstTeamOpponent);

    bool wasFirstTeamAtHome = PlaysAtHome(firstTeam);
    bool wasSecondTeamAtHome = PlaysAtHome(secondTeam);

    SetOspitalityForTeam(firstTeam, wasSecondTeamAtHome);
    SetOspitalityForTeam(secondTeam, wasFirstTeamAtHome);

}

void STT_MatchDay::ChangeHospitality(unsigned team){

    matchDay[team].FlipHospitality();
    matchDay[OpponentOfTeam(team)].FlipHospitality(); 

}

ostream& operator<<(ostream& os, const STT_MatchDay& md) {
    os << "-----------" << endl;
    for(unsigned t = 0; t < md.in.Teams(); t++){
        if(!md.PlaysAtHome(t)){continue;}
        string teamBias = t < 10 ? " " : "";
        unsigned opponent = md.OpponentOfTeam(t);
        string opponentBias = opponent < 10 ? " " : "";
        os << "| " << t << teamBias << " | " << opponent << opponentBias << " |" << endl;
        
    }
    os << "-----------" << endl;
    

    return os;
}
