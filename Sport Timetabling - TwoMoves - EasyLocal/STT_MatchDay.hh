#ifndef STT_MATCHDAY_HH
#define STT_MATCHDAY_HH

#include <iostream>
#include <vector>
#include "STT_Input.hh"
#include "STT_Util.hh"
#include "STT_Match.hh"

using namespace std;

class STT_MatchDay{
    friend ostream& operator<<(ostream& os, const STT_MatchDay& md);
    public: 
        STT_MatchDay(const STT_Input& i);

        STT_MatchDay& operator=(const STT_MatchDay& md);

        //general getter

        unsigned OpponentOfTeam(unsigned team) const {return matchDay[team].Opponent();};
        bool PlaysAtHome(unsigned team) const {return matchDay[team].TeamPlaysAtHome();};
        unsigned BigMatchesCount() const {return bigMatches;};

        int GetMatchIdForTeam(unsigned team) const ;

        //setters

        void SetOpponentForTeam(unsigned team, unsigned opponent);
        void SetOpponentForTeam(unsigned team, unsigned opponent, bool isFirstTeamAtHome);
        void SetOspitalityForTeam(unsigned team, bool isAtHome);

        //modifiers

        void SwapTeams(unsigned firstTeam, unsigned secondTeam);
        void ChangeHospitality(unsigned team);


    private: 

        const STT_Input& in;
        
        vector<STT_Match> matchDay;

        unsigned bigMatches; //count of big matches in this MatchDay

        void UpdateBigMatchesCount(unsigned team, int multiplier);
};

#endif