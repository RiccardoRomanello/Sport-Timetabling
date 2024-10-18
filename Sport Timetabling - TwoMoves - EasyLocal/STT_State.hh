#ifndef STT_STATE_HH
#define STT_STATE_HH

#include <iostream>
#include <vector>
#include "STT_Input.hh"
#include "STT_MatchDay.hh"

using namespace std;

//we just remember the first leg (andata), then all the others are computed from this one

class STT_State{
    friend ostream& operator<<(ostream& os, const STT_State& out);
    friend bool operator==(const STT_State& firstState, const STT_State& secondState);
    public: 
        STT_State(const STT_Input& i);

        STT_State& operator=(const STT_State& st);

        void Reset();

        //moves
        void SwapTeams(unsigned matchDay, unsigned firstTeam, unsigned secondTeam);
        void ChangeHospitalityForTeam(unsigned team, unsigned matchDay);

        //setters
        void SetOpponentForTeam(unsigned team, unsigned opponent, unsigned matchDay); //also sets side data structures
        void SetAtHomeForMatchday(unsigned team, unsigned matchDay, bool home); //sets !home for team opponent
        
        //getters
        int GetOpponentForTeamInMatchday(unsigned team, unsigned matchDay) const;
        int GetMatchIDForTeamInMatchDay(unsigned team, unsigned matchDay) const;
        bool AtHome(unsigned team, unsigned matchDay) const; 
        unsigned CountOfBigMatches(unsigned matchDay) const {return tournamentMatchDays[matchDay].BigMatchesCount();};
        unsigned HowManyMatchesPlayedAgainst(unsigned team, unsigned opponent) const {return howManyTimesAgainst[team][opponent];};

        //side structures
        void ComputeLeftSideOfFirstGameHAForTeam(unsigned team);
        void ComputeRightSideOfFirstGameHAForTeam(unsigned team);

        pair<int, int> FirstGameAsAway(unsigned team, unsigned matchDay) const {return firstGameAsAway[team][matchDay];};
        pair<int, int> FirstGameAsHome(unsigned team, unsigned matchDay) const {return firstGameAsHome[team][matchDay];};

    private:

        const STT_Input& in;
        //stores opponents and hospitality for each matchday
        vector<STT_MatchDay> tournamentMatchDays;

        //howManyTimesAgainst[team1][team2] = how many times team1 plays against team2
        vector<vector<unsigned> > howManyTimesAgainst; 

        //howManyBigMatches[m] --> #of bigmatches in matchday m
        vector<unsigned> howManyBigMatches; 

        //firstGameAsAway[team][matchDay].first = closest game, before matchDay, in which team is away
        //firstGameAsAway[team][matchDay].second = closest game, after matchDay, in which team is away
        //given t and m, everything in [firstGameAsAway[t][m].first + 1, firstGameAsAway[t][m].second - 1] are home games
        vector<vector<pair<int, int> > > firstGameAsAway;

        //firstGameAsHome[team][matchDay].first = closest game, before matchDay, in which team is home
        //firstGameAsHome[team][matchDay].second = closest game, after matchDay, in which team is home
        //given t a team and m a matchDay, everything in [firstGameAsHome[t][m].first + 1, firstGameAsHome[t][m].second - 1] are away games
        vector<vector<pair<int, int> > > firstGameAsHome;

    
};

#endif