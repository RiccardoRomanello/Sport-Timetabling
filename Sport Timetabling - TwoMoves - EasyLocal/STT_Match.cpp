#include "STT_Match.hh"
#include "assert.h"

STT_Match::STT_Match(unsigned team, unsigned opponent, bool teamPlaysHome){
    this -> team = team;
    this->opponent = opponent;
    hospitality = teamPlaysHome ? STT_Hospitality::HOME : STT_Hospitality::AWAY;

}

STT_Match& STT_Match::operator=(const STT_Match& m) {
    team = m.team;
    opponent = m.opponent;
    hospitality = m.hospitality;

    return *this;
}

void STT_Match::ChangeHospitality(bool teamPlaysHome){
    hospitality = STT_Hospitality::AWAY;
    
    if(teamPlaysHome){
        hospitality = STT_Hospitality::HOME;
    }
}

void STT_Match::FlipHospitality(){
    switch (hospitality)
    {
        case STT_Hospitality::HOME :
            hospitality = STT_Hospitality::AWAY;
            break;
        
        case STT_Hospitality::AWAY :
            hospitality = STT_Hospitality::HOME;
            break;

        default:
            assert (false);
    }
}

void STT_Match::ChangeOpponent(unsigned newOpponent){
    opponent = newOpponent;
}

ostream& operator<<(ostream& os, const STT_Match& md){
    if(md.hospitality == STT_Hospitality::AWAY){return os;}

    string teamBias = md.team < 10 ? " " : "";
    unsigned opponent = md.opponent;
    string opponentBias = opponent < 10 ? " " : "";
    os << "| " << md.team << teamBias << " | " << opponent << opponentBias << " |";
    return os;
}