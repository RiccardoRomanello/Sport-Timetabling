#ifndef STT_INPUT_HH
#define STT_INPUT_HH

#include <iostream>
#include <vector>
#include <cstdlib>
#include "STT_Util.hh"

using namespace std;

//Rem: to forbid a match a v b in matchday m of second leg, just forbid the match b v a in matchday m of the first leg

class STT_Input{
    friend ostream& operator<<(ostream&, const STT_Input &);
    friend istream& operator>>(istream&, STT_Input &);
public:
    STT_Input(string fileName);
    bool TopTeam(unsigned team) const {return isATopTeam[team];};
    bool ForbiddenInAMatchDay(unsigned matchDay, unsigned matchId) const {return forbiddenInMatchDayBool[matchDay][matchId];};
    bool ForbiddenInAMatchDay(unsigned matchDay, unsigned firstTeam, unsigned secondTeam) const {return forbiddenInMatchDayBool[firstTeam][secondTeam];};
    bool ForbiddenTogether(unsigned firstMatchId, unsigned secondMatchId) const {return forbiddenTogetherBool[firstMatchId][secondMatchId] || forbiddenTogetherBool[secondMatchId][firstMatchId];} //handles also non-simmetric matrix for forbidden togethr
    bool ShouldPlayAtHome(unsigned team, unsigned matchDay) const {return atHomeBool[team][matchDay];}
    int ComplementOf(unsigned team) const {return complementary[team];};
    unsigned LongestSeqAtHome(unsigned team) const {return longestSeqAtHome[team];};
    unsigned LongestSeqAway(unsigned team) const {return longestSeqAway[team];};

    unsigned Teams() const {return teamsCount;}
    unsigned MatchDays() const {return matchDaysCount;}
    unsigned TotalMatchDays() const {return totalMatchDaysCount;}
    unsigned IDs() const {return matchIdsCount;}
    unsigned BigMatches() const {return bigMatchesPerMatchday;}

private:
    //matchDaysCount is the number of matchDays per 'leg'. 
    //totalMatchDaysCount is the complete number of matchDays (counting all the legs, in our case is 2, 'andata' and 'ritorno')
    unsigned teamsCount, matchDaysCount, matchIdsCount, bigMatchesPerMatchday, totalMatchDaysCount;

    vector<unsigned> topTeams; //set of top team, redundancy check
    vector<bool> isATopTeam; //isATopTeam[i] == 1 --> i is a topTeam

    vector<vector<bool> > forbiddenInMatchDayBool; //forbiddenInMatchDayBool[t][id] = 1 -> match id is forbidden in matchday t --> each list has size Theta(matchids)

    vector<vector<bool> > forbiddenTogetherBool; //forbiddenTogetherBool[id1][id2] = 1 -> match with id1 and id2 are forbidden together

    vector<int> complementary; //complementary[t1] = t2 -> t2 must play away when t1 is home and viceversa

    vector<vector<bool> > atHomeBool; //atHomeBool[T][t] = 1 -> team T should play matchDay t at home

    vector<int> longestSeqAtHome; //longetSeqAtHome[t] --> max amount of consecutive home games for team t
    vector<int> longestSeqAway; //longetSeqAway[t] --> max amount of consecutive away games for team t

};

#endif