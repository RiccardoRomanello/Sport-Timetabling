#ifndef STT_UTIL_HH
#define STT_UTIL_HH

#include <iostream>
#include <vector>

using namespace std;

class STT_Util{
    public:
        STT_Util(int teamsCount);
        unsigned GetMatchId(unsigned homeTeam, unsigned awayTeam) const ;
        pair<unsigned, unsigned> GetTeamsFromMatchId(int matchiId) const ;

    private:
        int teamsCount;
};

#endif