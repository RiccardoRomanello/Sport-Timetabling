#ifndef STT_MATCH_HH
#define STT_MATCH_HH

#include <iostream>
#include <vector>

using namespace std;

enum class STT_Hospitality{
    HOME, 
    AWAY
};

class STT_Match{
    friend ostream& operator<<(ostream& os, const STT_Match& md);
    public: 
        STT_Match(unsigned team, unsigned opponent, bool teamPlaysHome);
        STT_Match& operator=(const STT_Match& m);

        unsigned Opponent() const { return opponent; };
        unsigned TeamPlaysAtHome() const {return hospitality == STT_Hospitality::HOME;};

        void FlipHospitality();
        void ChangeHospitality(bool teamPlaysHome);
        void ChangeOpponent(unsigned newOpponent);
    private: 
        unsigned team;

        //opponent team
        unsigned opponent;

        //hospitality for this team
        STT_Hospitality hospitality;

};

#endif 