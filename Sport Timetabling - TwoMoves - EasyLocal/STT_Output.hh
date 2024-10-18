#ifndef STT_OUTPUT_HH
#define STT_OUTPUT_HH

#include <iostream>
#include <cstdlib>
#include <vector>
#include "STT_Input.hh"
#include "STT_MatchDay.hh"

using namespace std;

class STT_Output{

    friend ostream& operator<<(ostream& os, const STT_Output& out);
    friend istream& operator>>(istream& is, STT_Output & out);

    public: 
        STT_Output(const STT_Input& i);

        void SetOpponentForTeamInMatchDay(unsigned matchDay, unsigned team, unsigned opponent);
        unsigned OpponentForTeamInMatchDay(unsigned matchDay, unsigned team) const {return tournament[matchDay].OpponentOfTeam(team);};

        void SetAtHomeForMatchDay(unsigned matchDay, unsigned team, bool atHome);
        bool PlaysAtHomeInMatchDay(unsigned matchDay, unsigned team) const { return tournament[matchDay].PlaysAtHome(team); };


    private:
        const STT_Input& in;

        vector<STT_MatchDay> tournament; 
};

#endif