#include "STT_NeighborhoodExplorer.hh"

/* Swap move */

void STT_SwapNeighborhoodExplorer::MakeMove(STT_State &state, const STT_Move_Swap& move) const {
    state.SwapTeams(move.MatchDay(), move.FirstTeam(), move.SecondTeam());
}

void STT_SwapNeighborhoodExplorer::RandomMove(const STT_State &state, STT_Move_Swap& move) const {

    unsigned matchDay = Random::Uniform(0, static_cast<int>(in.MatchDays() - 1));

    unsigned firstTeam = Random::Uniform(0, static_cast<int>(in.Teams()-1));

    //not going to generate a move that contains a team and its current opponent (or team vs team)
    vector<unsigned> possibleOpponents;
    for(int i = 0; i < in.Teams(); i++){
        if(i != firstTeam && i != state.GetOpponentForTeamInMatchday(firstTeam, matchDay)){
            possibleOpponents.push_back(i);
        }
    }

    unsigned secondTeamIndex = Random::Uniform(0, static_cast<int>(possibleOpponents.size()-1));
    unsigned secondTeam = possibleOpponents[secondTeamIndex];

    if(firstTeam > secondTeam){
        swap(firstTeam, secondTeam);
    }

    move.firstTeam = firstTeam;
    move.secondTeam = secondTeam;
    move.matchDay = matchDay;

    possibleOpponents.clear();
}

void STT_SwapNeighborhoodExplorer::FirstMove(const STT_State &state, STT_Move_Swap& move) const {
    move.matchDay = 0;
    move.firstTeam = 0; 
    if(state.GetOpponentForTeamInMatchday(move.firstTeam, move.matchDay) == 1){
        move.secondTeam = 2;
    }else{
        move.secondTeam = 1;
    }
}

bool STT_SwapNeighborhoodExplorer::NextMove(const STT_State &state, STT_Move_Swap& move) const {
    bool noMoreMoves = false;
    do{
        if(move.secondTeam < in.Teams() - 1){
            move.secondTeam++; // eventualmente é Feasible a controllare se la mossa é ok
        }else{
            //move.secondTeam == in.Teams() - 1 
            if(move.firstTeam < in.Teams() - 2){
                move.firstTeam += 1;
                move.secondTeam = move.firstTeam + 1; // eventualmente é Feasible a controllare se la mossa é ok
            }else{
                //move.firstTeam == in.Teams() - 1 
                if(move.matchDay < in.MatchDays() - 1){
                    move.matchDay++;
                    move.firstTeam = 0; 
                    move.secondTeam = 1; // eventualmente é Feasible a controllare se la mossa é ok
                }else{
                    noMoreMoves = true;
                }
            }
        }
    }
    while(!noMoreMoves && !FeasibleMove(state, move));
    
    return !noMoreMoves;
        
}

bool STT_SwapNeighborhoodExplorer::FeasibleMove(const STT_State &state, const STT_Move_Swap& move) const {
    return state.GetOpponentForTeamInMatchday(move.firstTeam, move.matchDay) != move.secondTeam;
}

/* Change Hospitality */

void STT_ChangeHospitalityNeighborhoodExplorer::MakeMove(STT_State &state, const STT_Move_ChangeHospitality& move) const {
    state.ChangeHospitalityForTeam(move.Team(), move.MatchDay());
}

void STT_ChangeHospitalityNeighborhoodExplorer::RandomMove(const STT_State &state, STT_Move_ChangeHospitality& move) const {

    unsigned matchDay = Random::Uniform(0, static_cast<int>(in.MatchDays() - 1));
    unsigned team = Random::Uniform(0, static_cast<int>(in.Teams()-1));

    move.team = team;
    move.matchDay = matchDay;

}

void STT_ChangeHospitalityNeighborhoodExplorer::FirstMove(const STT_State &state, STT_Move_ChangeHospitality& move) const {
    move.team = 0;
    move.matchDay = 0;
}

bool STT_ChangeHospitalityNeighborhoodExplorer::NextMove(const STT_State &state, STT_Move_ChangeHospitality& move) const {

    bool noMoreMoves = false;

    if(move.team < in.Teams() - 1){
        move.team++;
    }else{
        if(move.matchDay < in.MatchDays() - 1){
            move.team = 0;
            move.matchDay += 1;
        }else{
            noMoreMoves = true;
        }
    }

    return !noMoreMoves;

}

bool STT_ChangeHospitalityNeighborhoodExplorer::FeasibleMove(const STT_State &state, const STT_Move_ChangeHospitality& move) const {
    return true;
}