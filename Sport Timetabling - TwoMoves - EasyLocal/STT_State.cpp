#include "STT_State.hh"
#include "STT_Input.hh"

#include <math.h>
#include <assert.h>

STT_State::STT_State(const STT_Input& i): 
    in(i),  
    tournamentMatchDays(in.MatchDays(), STT_MatchDay(i)),
    howManyTimesAgainst(in.Teams(), vector<unsigned>(in.Teams(), 0)),
    firstGameAsAway(in.Teams(), vector<pair<int, int>>(in.TotalMatchDays(), pair<int, int> (-1, in.TotalMatchDays()))),
    firstGameAsHome(in.Teams(), vector<pair<int, int>>(in.TotalMatchDays(), pair<int, int> (-1, in.TotalMatchDays())))
{}

void STT_State::Reset(){

    tournamentMatchDays = vector<STT_MatchDay>(in.MatchDays(), STT_MatchDay(in));
    howManyBigMatches = vector<unsigned>(in.MatchDays(), 0);
    firstGameAsAway = vector<vector<pair<int, int> > >(in.Teams(), vector<pair<int, int>>(in.TotalMatchDays(), pair<int, int> (-1, in.TotalMatchDays())));
    firstGameAsHome = vector<vector<pair<int, int> > >(in.Teams(), vector<pair<int, int>>(in.TotalMatchDays(), pair<int, int> (-1, in.TotalMatchDays())));
}

STT_State& STT_State::operator=(const STT_State& st)
{  
    tournamentMatchDays = st.tournamentMatchDays;
    howManyTimesAgainst = st.howManyTimesAgainst;
    firstGameAsAway = st.firstGameAsAway;
    firstGameAsHome = st.firstGameAsHome;
    return *this;
}

bool STT_State::AtHome(unsigned team, unsigned matchDay) const {

    if(matchDay < in.MatchDays()){ 
        //first leg
        return tournamentMatchDays[matchDay].PlaysAtHome(team);
    }else{
        //second leg
        return !tournamentMatchDays[matchDay%in.MatchDays()].PlaysAtHome(team);
    }
    
}

void STT_State::SetOpponentForTeam(unsigned team, unsigned opponent, unsigned matchDay){
    //called only on object init so NO - on howManyTimesAgainst
    tournamentMatchDays[matchDay].SetOpponentForTeam(team, opponent);
    howManyTimesAgainst[team][opponent] += 1;
    howManyTimesAgainst[opponent][team] += 1;
}


void STT_State::SetAtHomeForMatchday(unsigned team, unsigned matchDay, bool home){

    tournamentMatchDays[matchDay].SetOspitalityForTeam(team, home);
}

int STT_State::GetOpponentForTeamInMatchday(unsigned team, unsigned matchDay) const{
    //using the rem with MatchDays() so if there are 15 matchdays per leg, and I want the matchDay 21, i get the 6th matchday 
    return tournamentMatchDays[matchDay % in.MatchDays()].OpponentOfTeam(team);
}

int STT_State::GetMatchIDForTeamInMatchDay(unsigned team, unsigned matchDay) const {
    unsigned matchId = tournamentMatchDays[matchDay % in.MatchDays()].GetMatchIdForTeam(team);
    if(matchDay < in.MatchDays()){
        return matchId;
    }

    STT_Util util(in.Teams());

    pair<unsigned, unsigned> teams = util.GetTeamsFromMatchId(matchId);

    return util.GetMatchId(teams.second, teams.first); 
}

void STT_State::ComputeLeftSideOfFirstGameHAForTeam(unsigned team){
    //reset for recomputing
    for(int m = 0; m < in.TotalMatchDays(); m++){
        firstGameAsAway[team][m].first = -1;
        firstGameAsHome[team][m].first = -1;
    }

    if(AtHome(team, 0)){
        firstGameAsHome[team][0].first = 0;
    }else{
        firstGameAsAway[team][0].first = 0;
    }
    
    for(int m = 1; m < in.TotalMatchDays(); m++){
        pair<int, int> firstAwayForPrevMatchDay = firstGameAsAway[team][m-1];
        int leftFirstAway = firstAwayForPrevMatchDay.first;

        pair<int, int> firstHomeMatchForPrevMatchDay = firstGameAsHome[team][m-1];
        int leftFirstHome = firstHomeMatchForPrevMatchDay.first;

        if(AtHome(team, m-1)){
            //if in the prev matchDay team was playing home, then I use some sort of 'recursive'
            firstGameAsAway[team][m].first = leftFirstAway;
            //While, the first game at home is exactly m - 1
            firstGameAsHome[team][m].first = m - 1;
        }else{
            //The first game away for team is m - 1; 
            firstGameAsAway[team][m].first = m - 1;
            //While if m - 1 is not playing at home, to find the first game home I use some sort of 'recursive'
            firstGameAsHome[team][m].first = leftFirstHome;
        }
    }

}

void STT_State::ComputeRightSideOfFirstGameHAForTeam(unsigned team){

    //reset for recomputing
    for(int m = 0; m < in.TotalMatchDays(); m++){
        firstGameAsAway[team][m].second = in.TotalMatchDays();
        firstGameAsHome[team][m].second = in.TotalMatchDays();
    }

    if(AtHome(team, in.TotalMatchDays()-1)){
        firstGameAsHome[team][in.TotalMatchDays()-1].second = in.TotalMatchDays() - 1;
    }else{
        firstGameAsAway[team][in.TotalMatchDays()-1].second = in.TotalMatchDays() - 1;
    }   

    for(int m = static_cast<int>(in.TotalMatchDays()) - 2; m >= 0; m--){
        pair<int, int> firstAwayForNextMatchDay = firstGameAsAway[team][m+1];
        int rightFirstAway = firstAwayForNextMatchDay.second;

        pair<int, int> firstHomeMatchForNextMatchDay = firstGameAsHome[team][m+1];
        int rightFirstHome = firstHomeMatchForNextMatchDay.second;

        if(AtHome(team, m+1)){
            //if team plays m+1 at home, then I use the result of m + 1 to find the first game away
            firstGameAsAway[team][m].second = rightFirstAway;
            //Instead, to store the home games, I just store m + 1 
            firstGameAsHome[team][m].second = m+1;
        }else{
            //if team plays away the match m + 1, then i store it in the away array
            firstGameAsAway[team][m].second = m+1;
            //while, for the home games I use the result of m + 1
            firstGameAsHome[team][m].second = rightFirstHome;
        }
    }

}

//this should be called always with matchDay < in.MatchDays()
void STT_State::ChangeHospitalityForTeam(unsigned team, unsigned matchDay){
    assert (matchDay < in.MatchDays());

    unsigned opponent = tournamentMatchDays[matchDay].OpponentOfTeam(team);
    tournamentMatchDays[matchDay].ChangeHospitality(team);
    
    //keeping auxiliary data up to date
    ComputeLeftSideOfFirstGameHAForTeam(team);
    ComputeRightSideOfFirstGameHAForTeam(team);
    ComputeLeftSideOfFirstGameHAForTeam(opponent);
    ComputeRightSideOfFirstGameHAForTeam(opponent);

}

//this should be called always with matchDay < in.MatchDays()
void STT_State::SwapTeams(unsigned matchDay, unsigned firstTeam, unsigned secondTeam){
    assert (matchDay < in.MatchDays());
    /**
     * let x = firstTeam, y = secondTeam and m = matchDay
     * then there exists x' and y' such that in m, x v x' and y v y'.
     * So, with this change, we want the tournament to become y v x' and x v y'
     * x will play at home iff y was playing home and viceversa
    **/
    unsigned formerFirstTeamOpponent = tournamentMatchDays[matchDay].OpponentOfTeam(firstTeam);
    unsigned formerSecondTeamOpponent = tournamentMatchDays[matchDay].OpponentOfTeam(secondTeam);

    //updating sides data structures

    howManyTimesAgainst[firstTeam][formerFirstTeamOpponent] -= 1;
    howManyTimesAgainst[formerFirstTeamOpponent][firstTeam] -= 1;
    howManyTimesAgainst[secondTeam][formerSecondTeamOpponent] -= 1;
    howManyTimesAgainst[formerSecondTeamOpponent][secondTeam] -= 1;

    tournamentMatchDays[matchDay].SwapTeams(firstTeam, secondTeam);

    howManyTimesAgainst[firstTeam][formerSecondTeamOpponent] += 1;
    howManyTimesAgainst[formerSecondTeamOpponent][firstTeam] += 1;
    howManyTimesAgainst[secondTeam][formerFirstTeamOpponent] += 1;
    howManyTimesAgainst[formerFirstTeamOpponent][secondTeam] += 1;
    
    ComputeLeftSideOfFirstGameHAForTeam(firstTeam);
    ComputeRightSideOfFirstGameHAForTeam(firstTeam);
    ComputeLeftSideOfFirstGameHAForTeam(secondTeam);
    ComputeRightSideOfFirstGameHAForTeam(secondTeam);
}

bool operator==(const STT_State& firstState, const STT_State& secondState){
    for(int m = 0; m < firstState.in.MatchDays(); m++){
        for(int t = 0; t < firstState.in.Teams(); t++){
            if(firstState.GetOpponentForTeamInMatchday(t, m) != 
                secondState.GetOpponentForTeamInMatchday(t, m)){
                    return false;
            }

            if(firstState.AtHome(t, m) != secondState.AtHome(t, m)){
                    return false;
            }
        }
    }

    return true;
}

ostream& operator<<(ostream& os, const STT_State& out){
    os << "againstWho = [|";
    for(int m = 0; m < out.in.TotalMatchDays(); m++) {
        for(int t = 0; t < out.in.Teams(); t++){
            os << (out.GetOpponentForTeamInMatchday(t, m) + 1); 
            if(t != out.in.Teams() - 1){
                os << ", ";
            }
        }
        os << "|";
        if(m != out.in.TotalMatchDays() - 1){
            cout << endl;
        }
    }
    os << "];" << endl;
    os << "matchesOspitality = [|";
    for(int m = 0; m < out.in.TotalMatchDays(); m++) {
        for(int t = 0; t < out.in.Teams(); t++){
            os << (out.AtHome(t, m) ? "H" : "V");
            if(t != out.in.Teams() - 1){
                os << ", ";
            }
        }
        os << "|";
        if(m != out.in.TotalMatchDays() - 1){
            cout << endl;
        }
    }
    os << "];" << endl;

    return os;
}



