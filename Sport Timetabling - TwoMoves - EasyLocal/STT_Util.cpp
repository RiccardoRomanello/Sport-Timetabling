#include "STT_Util.hh"

#include <math.h>

STT_Util::STT_Util(int teams) {
    teamsCount = teams;
}

unsigned STT_Util::GetMatchId(unsigned homeTeam, unsigned awayTeam) const {
    return homeTeam*teamsCount + awayTeam;
}

pair<unsigned, unsigned> STT_Util::GetTeamsFromMatchId(int matchId) const { 
    unsigned homeTeam, awayTeam;

    homeTeam = floor(static_cast<float>(matchId)/static_cast<float>(teamsCount));
    awayTeam = matchId % teamsCount;

    return pair<unsigned, unsigned> (homeTeam, awayTeam);
}
