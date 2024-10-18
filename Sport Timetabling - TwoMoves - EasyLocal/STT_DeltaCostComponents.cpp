#include "STT_DeltaCostComponents.hh"
#include <assert.h>

/*Re-played matches delta cost component*/

int SwapRePlayedMatchesDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const{
    int firstTeamOfMove = move.FirstTeam();
    int secondTeamOfMove = move.SecondTeam();
    int matchDayOfMove = move.MatchDay();

    int currentOpponentOfFirstTeam = state.GetOpponentForTeamInMatchday(firstTeamOfMove, matchDayOfMove);
    int currentOpponentOfSecondTeam = state.GetOpponentForTeamInMatchday(secondTeamOfMove, matchDayOfMove);

    //now the matches are firstTeamOfMove v currentOpponentOfFirstTeam 
    //                    secondTeamOfMove v currentOpponentOfSecondTeam

    //what we will get is secondTeamOfMove v currentOpponentOfFirstTeam
    //                    firstTeamOfMove v currentOpponentOfSecondTeam

    //if firstTeamOfMove is playing twice against currentOpponentOfFirstTeam, 
    //I remove a violations 
    //if firstTeamOfMove would play twice against currentOpponentOfSecondTeam, 
    //I add a violation 

    //and equally for secondTeamOfMove

    int countFirstTeamAgainstItsOpponent = state.HowManyMatchesPlayedAgainst(firstTeamOfMove, currentOpponentOfFirstTeam);
    int countFirstTeamAgainstItsNewOpponent = state.HowManyMatchesPlayedAgainst(firstTeamOfMove, currentOpponentOfSecondTeam);

    int countSecondTeamAgainstItsOpponent = state.HowManyMatchesPlayedAgainst(secondTeamOfMove, currentOpponentOfSecondTeam);
    int countSecondTeamAgainstItsNewOpponent = state.HowManyMatchesPlayedAgainst(secondTeamOfMove, currentOpponentOfFirstTeam);

    int delta = 0;
    if(countFirstTeamAgainstItsOpponent >= 2){
        delta -= 1;
    }

    if(countFirstTeamAgainstItsNewOpponent >= 1){
        delta += 1;
    }

    if(countSecondTeamAgainstItsOpponent >= 2){
        delta -= 1;
    }

    if(countSecondTeamAgainstItsNewOpponent >= 1){
        delta += 1;
    }

    return delta;
}

int CHRePlayedMatchesDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    return 0;
}

/* No matches played */

int SwapNoMatchesPlayedDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const {
    int delta = 0;


    int firstTeamOfMove = move.FirstTeam();
    int secondTeamOfMove = move.SecondTeam();
    int matchDayOfMove = move.MatchDay();

    int currentOpponentOfFirstTeam = state.GetOpponentForTeamInMatchday(firstTeamOfMove, matchDayOfMove);
    int currentOpponentOfSecondTeam = state.GetOpponentForTeamInMatchday(secondTeamOfMove, matchDayOfMove);

    //I'm removing matches so I could introduce violations
    if(state.HowManyMatchesPlayedAgainst(firstTeamOfMove, currentOpponentOfFirstTeam) == 1){
        delta += 1;
    }
    if(state.HowManyMatchesPlayedAgainst(secondTeamOfMove, currentOpponentOfSecondTeam) == 1){
        delta += 1;
    }
    
    //I'm adding matches so I could remove violations

    if(state.HowManyMatchesPlayedAgainst(firstTeamOfMove, currentOpponentOfSecondTeam) == 0){
        delta -= 1;
    }
    if(state.HowManyMatchesPlayedAgainst(secondTeamOfMove, currentOpponentOfFirstTeam) == 0){
        delta -= 1;
    }

    return delta;

}

int CHNoMatchesPlayedDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    return 0;
}

/* Big matches per Matchday */

int SwapBigMatchesPerMatchDayDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const {
    int moveMatchDay = move.MatchDay();
    
    bool firstTeamTopTeam = in.TopTeam(move.FirstTeam());
    bool secondTeamTopTeam = in.TopTeam(move.SecondTeam());

    bool firstTeamOpponentTopTeam = in.TopTeam(state.GetOpponentForTeamInMatchday(move.FirstTeam(), moveMatchDay));
    bool secondTeamOpponentTopTeam = in.TopTeam(state.GetOpponentForTeamInMatchday(move.SecondTeam(), moveMatchDay));

    int bigMatchesCount = state.CountOfBigMatches(moveMatchDay); //O(1)

    bool firstTeamCurrentBigMatch = firstTeamTopTeam && firstTeamOpponentTopTeam;
    bool secondTeamCurrentBigMatch = secondTeamTopTeam && secondTeamOpponentTopTeam;

    int bigMatchesRemoved = 0; 
    bigMatchesRemoved += (firstTeamCurrentBigMatch == true);
    bigMatchesRemoved += (secondTeamCurrentBigMatch == true);

    bool firstTeamNewBigMatch = firstTeamTopTeam && secondTeamOpponentTopTeam;
    bool secondTeamNewBigMatch = secondTeamTopTeam && firstTeamOpponentTopTeam;

    int bigMatchesAdded = 0; 
    bigMatchesAdded += (firstTeamNewBigMatch == true);
    bigMatchesAdded += (secondTeamNewBigMatch == true);

    if(bigMatchesCount > in.BigMatches()){ //if the current state has a violations
        if(bigMatchesCount - bigMatchesRemoved + bigMatchesAdded > in.BigMatches()){
            //and I mantain it, then 0 delta
            return 0;
        }else{
            //if I remove it, then -1 delta
            return -1;
        }
    }else{
        //if the current state has no violations
        if(bigMatchesCount - bigMatchesRemoved + bigMatchesAdded > in.BigMatches()){
            //if I add it, then 1 delta
            return 1;
        }else{
            //if I don't modify it, then 0 delta
            return 0;
        }
    }
}

int CHBigMatchesPerMatchDayDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    return 0;
}

/* Forbidden in matchday */

int SwapForbiddenInMatchDayDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const {
    STT_Util util(in.Teams());

    int moveMatchDay = move.MatchDay();
    int firstTeamOfMoveMatchID = state.GetMatchIDForTeamInMatchDay(move.FirstTeam(), moveMatchDay);
    int secondTeamOfMoveMatchID = state.GetMatchIDForTeamInMatchDay(move.SecondTeam(), moveMatchDay);

    int delta = 0; 

    if(in.ForbiddenInAMatchDay(moveMatchDay, firstTeamOfMoveMatchID)){
        delta -= 1;
    }

    if(in.ForbiddenInAMatchDay(moveMatchDay, secondTeamOfMoveMatchID)){
        delta -= 1;
    }

    int firstTeamOpponent = state.GetOpponentForTeamInMatchday(move.FirstTeam(), moveMatchDay);
    int secondTeamOpponent = state.GetOpponentForTeamInMatchday(move.SecondTeam(), moveMatchDay);

    //new match is firstTeam v secondTeamOpponent
    //             secondTeam v firstTeamOpponent

    bool firstTeamNewMatchAtHome = state.AtHome(move.SecondTeam(), moveMatchDay);
    bool secondTeamNewMatchAtHome = state.AtHome(move.FirstTeam(), moveMatchDay);

    //computing the new matchids

    int firstTeamNewMatchId; 
    if(firstTeamNewMatchAtHome){
        firstTeamNewMatchId = util.GetMatchId(move.FirstTeam(), secondTeamOpponent);
    }else{
        firstTeamNewMatchId = util.GetMatchId(secondTeamOpponent, move.FirstTeam());
    }

    int secondTeamNewMatchId;
    if(secondTeamNewMatchAtHome){
        secondTeamNewMatchId = util.GetMatchId(move.SecondTeam(), firstTeamOpponent);
    }else{
        secondTeamNewMatchId = util.GetMatchId(firstTeamOpponent, move.SecondTeam());
    }

    //now checking if I would add some violations

    if(in.ForbiddenInAMatchDay(moveMatchDay, firstTeamNewMatchId)){
        delta += 1;
    }

    if(in.ForbiddenInAMatchDay(moveMatchDay, secondTeamNewMatchId)){
        delta += 1;
    }

    return delta;
}

int CHForbiddenInMatchDayDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    STT_Util util(in.Teams());

    int moveMatchDay = move.MatchDay();
    int team = move.Team();
    int currentMatchId = state.GetMatchIDForTeamInMatchDay(team, moveMatchDay);
    int delta = 0; 

    if(in.ForbiddenInAMatchDay(moveMatchDay, currentMatchId)){
        delta -= 1;
    }

    int newMatchId, opponentOfTeam;
    opponentOfTeam = state.GetOpponentForTeamInMatchday(team, move.MatchDay());
    if(state.AtHome(team, moveMatchDay)){
        //if it's currenty at home, it will become away after the move is performed 
        newMatchId = util.GetMatchId(opponentOfTeam, team);
    }else{
        //if it's away, it will become home
        newMatchId = util.GetMatchId(team, opponentOfTeam);
    }

    if(in.ForbiddenInAMatchDay(moveMatchDay, newMatchId)){
        delta += 1;
    }
    return delta;

}

/* Complementary teams */

int SwapComplementaryTeamsDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const {
    int delta = 0; 
    int complementOfFirstTeam = in.ComplementOf(move.FirstTeam());
    int complementOfSecondTeam = in.ComplementOf(move.SecondTeam());

    
    if(complementOfFirstTeam != -1 && state.AtHome(move.FirstTeam(), move.MatchDay()) && state.AtHome(complementOfFirstTeam, move.MatchDay())){
        delta -= 1;
    }

    if(complementOfSecondTeam != -1 && state.AtHome(move.SecondTeam(), move.MatchDay()) && state.AtHome(complementOfSecondTeam, move.MatchDay())){
        delta -= 1;
    }

    //now move.FirstTeam() will play home after the swap iff move.SecondTeam() is at home before the swap;
    //now move.SecondTeam() will play home after the swap iff move.FirstTeam() is at home before the swap;
    
    //if FirstTeam will keep being at home (so if SecondTeam WAS at home), and the complement of firstteam is at home, 
    //add violation
    if(complementOfFirstTeam != -1 && state.AtHome(move.SecondTeam(), move.MatchDay()) && state.AtHome(complementOfFirstTeam, move.MatchDay())) {
        delta += 1;
    }
    
    //if SecondTeam will keep being at home (so if FirstTeam WAS at home) and the complement of second team is at home,
    //add violation
    if(complementOfSecondTeam != -1 && state.AtHome(move.FirstTeam(), move.MatchDay()) && state.AtHome(complementOfSecondTeam, move.MatchDay())) {
        delta += 1;
    }

    if(complementOfFirstTeam == move.SecondTeam()){ //this implies that complement of second team is equal to first team
        delta /= 2;
    }

    return delta;
}

int CHComplementaryTeamsDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    int delta = 0; 
    int complementOfTeam = in.ComplementOf(move.Team());
    int team = move.Team();
    int opponent = state.GetOpponentForTeamInMatchday(move.Team(), move.MatchDay());

    if(complementOfTeam == opponent){
        return 0;
    }
    
    if(state.AtHome(team, move.MatchDay()) && complementOfTeam != -1 && state.AtHome(complementOfTeam, move.MatchDay())){
        //team is at home, it's complement too, but team will go away so one violation is removed
        delta = -1;
    }

    if(!state.AtHome(team, move.MatchDay()) && complementOfTeam != -1 && state.AtHome(complementOfTeam, move.MatchDay())){
        //team is away, will become home so one violation is added
        delta += 1;
    }

    //now handling changes to the opponent of team
    
    int complementOfOpponent = in.ComplementOf(opponent);
    if(state.AtHome(opponent, move.MatchDay()) && complementOfOpponent != -1 && state.AtHome(complementOfOpponent, move.MatchDay())){
        delta -= 1;
    }

    if(!state.AtHome(opponent, move.MatchDay()) && complementOfOpponent != -1 && state.AtHome(complementOfOpponent, move.MatchDay())){
        delta += 1;
    }
    return delta;
}

/* Sequence Of Home Games */

int SequenceOfHomeGamesUtility::JoinHomeSequences(int team, int matchDay, int bound, const STT_State& state) {
    int delta = 0; 
    int firstHome = state.FirstGameAsAway(team, matchDay).first + 1;
    int lastHome = state.FirstGameAsAway(team, matchDay).second - 1;
    //everything in [firstHome, matchDay-1] is H and everything in [matchDay+1, lastHome] is H for team, but matchDay will flip from V to H
    //I will add an H in matchday joining left and right side

    
    int lengthOfOldLeftSeq = max(0, matchDay - firstHome); //length of old seq of H at the left of team
    int lengthOfOldRightSeq = max(0, lastHome - matchDay); //length of old seq of H at the right of team
    int lengthOfNewSeq = lengthOfOldLeftSeq + lengthOfOldRightSeq + 1; //new seq length

    //bound = in.LongestSeqAtHome(team)

    delta += max(0, static_cast<int>(lengthOfNewSeq - bound));
    delta -= max(0, static_cast<int>(lengthOfOldLeftSeq - bound));
    delta -= max(0, static_cast<int>(lengthOfOldRightSeq - bound));

    return delta;
}

int SequenceOfHomeGamesUtility::SplitHomeSequences(int team, int matchDay, int bound, const STT_State& state) {
    int delta = 0; 
    int firstHome = state.FirstGameAsAway(team, matchDay).first + 1;
    int lastHome = state.FirstGameAsAway(team, matchDay).second - 1;

    //everything in [firstHome, lastHome] is H for team, but an H will become a V
    //will be splitted in firstHome ... matchDay - 1 and matchDay + 1 ... lastHome 

    int lengthOfNewLeftSeq = max(0, matchDay - firstHome); //left side new part
    int lengthOfNewRightSeq = max(0, lastHome - matchDay); //right side new part;
    int lengthOfOldSeq = lengthOfNewLeftSeq + lengthOfNewRightSeq + 1; //new seq length

    //bound = in.LongestSeqAtHome(team)

    delta -= max(0, static_cast<int>(lengthOfOldSeq - bound));
    delta += max(0, static_cast<int>(lengthOfNewLeftSeq - bound));
    delta += max(0, static_cast<int>(lengthOfNewRightSeq - bound));

    return delta;
}

int SwapSequenceOfHomeGamesDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const {
    int delta = 0; 

    bool firstTeamPlaysAtHome = state.AtHome(move.FirstTeam(), move.MatchDay());
    bool secondTeamPlaysAtHome = state.AtHome(move.SecondTeam(), move.MatchDay());
    bool firstTeamWillPlayAtHome = secondTeamPlaysAtHome;
    bool secondTeamWillPlayAtHome = firstTeamPlaysAtHome;

    int firstTeamBound = in.LongestSeqAtHome(move.FirstTeam());
    int secondTeamBound = in.LongestSeqAtHome(move.SecondTeam());

    if(firstTeamPlaysAtHome && !firstTeamWillPlayAtHome){
        delta += SequenceOfHomeGamesUtility::SplitHomeSequences(move.FirstTeam(), move.MatchDay(), firstTeamBound, state);
        delta += SequenceOfHomeGamesUtility::JoinHomeSequences(move.FirstTeam(), move.MatchDay() + in.MatchDays(), firstTeamBound, state);
    } 

    if(!firstTeamPlaysAtHome && firstTeamWillPlayAtHome){
        delta += SequenceOfHomeGamesUtility::JoinHomeSequences(move.FirstTeam(), move.MatchDay(), firstTeamBound, state);
        delta += SequenceOfHomeGamesUtility::SplitHomeSequences(move.FirstTeam(), move.MatchDay() + in.MatchDays(), firstTeamBound, state);
    }

    if(secondTeamPlaysAtHome && !secondTeamWillPlayAtHome){
        delta += SequenceOfHomeGamesUtility::SplitHomeSequences(move.SecondTeam(), move.MatchDay(), secondTeamBound, state);
        delta += SequenceOfHomeGamesUtility::JoinHomeSequences(move.SecondTeam(), move.MatchDay() + in.MatchDays(), secondTeamBound, state);
    }

    if(!secondTeamPlaysAtHome && secondTeamWillPlayAtHome){
        delta += SequenceOfHomeGamesUtility::JoinHomeSequences(move.SecondTeam(), move.MatchDay(), secondTeamBound, state);
        delta += SequenceOfHomeGamesUtility::SplitHomeSequences(move.SecondTeam(), move.MatchDay() + in.MatchDays(), secondTeamBound, state);
    }

    return delta;
}

int CHSequenceOfHomeGamesDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    bool teamPlaysAtHome = state.AtHome(move.Team(), move.MatchDay());
    int delta = 0;

    int teamBound = in.LongestSeqAtHome(move.Team());

    if(teamPlaysAtHome){
        //if it plays at home, a 'change ospitality' will put it away so I split home sequences
        delta += SequenceOfHomeGamesUtility::SplitHomeSequences(move.Team(), move.MatchDay(), teamBound, state);
        delta += SequenceOfHomeGamesUtility::JoinHomeSequences(move.Team(), move.MatchDay() + in.MatchDays(), teamBound, state);
    }else{
        //if it plays away, a 'change ospitality' will put it home so i Join home sequences
        delta += SequenceOfHomeGamesUtility::JoinHomeSequences(move.Team(), move.MatchDay(), teamBound, state);
        delta += SequenceOfHomeGamesUtility::SplitHomeSequences(move.Team(), move.MatchDay() + in.MatchDays(), teamBound, state);
    }

    bool opponentPlaysAtHome = !teamPlaysAtHome;
    int opponent = state.GetOpponentForTeamInMatchday(move.Team(), move.MatchDay());

    int opponentBound = in.LongestSeqAtHome(opponent);

    if(opponentPlaysAtHome){
        delta += SequenceOfHomeGamesUtility::SplitHomeSequences(opponent, move.MatchDay(), opponentBound, state);
        delta += SequenceOfHomeGamesUtility::JoinHomeSequences(opponent, move.MatchDay() + in.MatchDays(), opponentBound, state);
    }else{
        delta += SequenceOfHomeGamesUtility::JoinHomeSequences(opponent, move.MatchDay(), opponentBound, state);
        delta += SequenceOfHomeGamesUtility::SplitHomeSequences(opponent, move.MatchDay() + in.MatchDays(), opponentBound, state);
    }

    return delta;
}

/* Sequence of Away games */

int SequenceOfAwayGamesUtility::JoinAwaySequences (int team, int matchDay, int bound, const STT_State& state) {
    
    int delta = 0;
    
    int firstAway = state.FirstGameAsHome(team, matchDay).first + 1;
    int lastAway = state.FirstGameAsHome(team, matchDay).second - 1;

    //everything in [firstAway, matchDay-1] is V and everything in [matchDay+1, lastAway] is V for team; and matchDay will flip from H to V
    //I will add a V in matchday joining left and right side

    int lengthOfOldLeftSeq = max(0, matchDay - firstAway); //length of the old seq at the left of matchDay
    int lengthOfOldRightSeq = max(0, lastAway - matchDay); //length of the old seq at the right of matchDay
    int lengthOfNewSeq = lengthOfOldLeftSeq + lengthOfOldRightSeq + 1;

    //bound = in.LongestSeqAway(team)

    delta += max(0, static_cast<int>(lengthOfNewSeq - bound));
    delta -= max(0, static_cast<int>(lengthOfOldLeftSeq - bound));
    delta -= max(0, static_cast<int>(lengthOfOldRightSeq - bound));

    return delta;
}

int SequenceOfAwayGamesUtility::SplitAwaySequences (int team, int matchDay, int bound, const STT_State& state) {
    int delta = 0;
    
    int firstAway = state.FirstGameAsHome(team, matchDay).first + 1;
    int lastAway = state.FirstGameAsHome(team, matchDay).second - 1;

    //everything in [firstAway, lastAway] is V for team and matchDay will flip from V to H
    //will be splitted in firstAway ... matchDay - 1 and matchDay + 1 ... lastAway 

    int lengthOfNewLeftSeq = max(0, matchDay - firstAway); //length of the new seq of V at the left of matchDay
    int lengthOfNewRightSeq = max(0, lastAway - matchDay); //length of the new seq of V at the right of matchDay
    int lengthOfOldSeq = lengthOfNewLeftSeq + lengthOfNewRightSeq + 1;

    delta -= max(0, static_cast<int>(lengthOfOldSeq - bound));
    delta += max(0, static_cast<int>(lengthOfNewLeftSeq - bound));
    delta += max(0, static_cast<int>(lengthOfNewRightSeq - bound));

    return delta;
}

int SwapSequenceOfAwayGamesDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const {
    
    int delta = 0; 

    bool firstTeamPlaysAtHome = state.AtHome(move.FirstTeam(), move.MatchDay());
    bool secondTeamPlaysAtHome = state.AtHome(move.SecondTeam(), move.MatchDay());
    bool firstTeamWillPlayAtHome = secondTeamPlaysAtHome;
    bool secondTeamWillPlayAtHome = firstTeamPlaysAtHome;

    int firstTeamBound = in.LongestSeqAway(move.FirstTeam());
    int secondTeamBound = in.LongestSeqAway(move.SecondTeam());


    if(!firstTeamPlaysAtHome && firstTeamWillPlayAtHome){
        delta += SequenceOfAwayGamesUtility::SplitAwaySequences(move.FirstTeam(), move.MatchDay(), firstTeamBound, state); // from Away to Home for matchday in firstLeg
        delta += SequenceOfAwayGamesUtility::JoinAwaySequences(move.FirstTeam(), move.MatchDay() + in.MatchDays(), firstTeamBound, state); // from Home to Away for matchday in secondLeg
    }
    
    if(firstTeamPlaysAtHome && !firstTeamWillPlayAtHome){
        delta += SequenceOfAwayGamesUtility::JoinAwaySequences(move.FirstTeam(), move.MatchDay(), firstTeamBound, state);
        delta += SequenceOfAwayGamesUtility::SplitAwaySequences(move.FirstTeam(), move.MatchDay() + in.MatchDays(), firstTeamBound, state);
    } 

    if(secondTeamPlaysAtHome && !secondTeamWillPlayAtHome){
        delta += SequenceOfAwayGamesUtility::JoinAwaySequences(move.SecondTeam(), move.MatchDay(), secondTeamBound, state);
        delta += SequenceOfAwayGamesUtility::SplitAwaySequences(move.SecondTeam(), move.MatchDay() + in.MatchDays(), secondTeamBound, state);
    }

    if(!secondTeamPlaysAtHome && secondTeamWillPlayAtHome){
        delta += SequenceOfAwayGamesUtility::SplitAwaySequences(move.SecondTeam(), move.MatchDay(), secondTeamBound, state);
        delta += SequenceOfAwayGamesUtility::JoinAwaySequences(move.SecondTeam(), move.MatchDay() + in.MatchDays(), secondTeamBound, state);
    }

    return delta;
}

int CHSequenceOfAwayGamesDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    bool teamPlaysAtHome = state.AtHome(move.Team(), move.MatchDay());
    int delta = 0;

    int teamBound = in.LongestSeqAway(move.Team());

    if(!teamPlaysAtHome){
        //if it plays away, a 'change ospitality' will put it home so I split away sequences
        delta += SequenceOfAwayGamesUtility::SplitAwaySequences(move.Team(), move.MatchDay(), teamBound, state);
        delta += SequenceOfAwayGamesUtility::JoinAwaySequences(move.Team(), move.MatchDay() + in.MatchDays(), teamBound, state); // if team will play home in m for firstLeg, it will play away for secondLeg, so Join 
    }else{
        //if it plays home, a 'change ospitality' will put it away so i Join away sequences
        delta += SequenceOfAwayGamesUtility::JoinAwaySequences(move.Team(), move.MatchDay(), teamBound, state);
        delta += SequenceOfAwayGamesUtility::SplitAwaySequences(move.Team(), move.MatchDay() + in.MatchDays(), teamBound, state); // as above I must think for the second leg
    }

    bool opponentPlaysAtHome = !teamPlaysAtHome;
    int opponent = state.GetOpponentForTeamInMatchday(move.Team(), move.MatchDay());

    int opponentBound = in.LongestSeqAway(opponent);

    if(!opponentPlaysAtHome){
        delta += SequenceOfAwayGamesUtility::SplitAwaySequences(opponent, move.MatchDay(), opponentBound, state);
        delta += SequenceOfAwayGamesUtility::JoinAwaySequences(opponent, move.MatchDay() + in.MatchDays(), opponentBound, state);
    }else{
        delta += SequenceOfAwayGamesUtility::JoinAwaySequences(opponent, move.MatchDay(), opponentBound, state);
        delta += SequenceOfAwayGamesUtility::SplitAwaySequences(opponent, move.MatchDay() + in.MatchDays(), opponentBound, state); 
    }

    return delta;
}

/* Forbidden Together */

int SwapForbiddenTogetherDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const{
    int delta = 0; 
    STT_Util util(in.Teams());

    int currentFirstTeamMatchId = state.GetMatchIDForTeamInMatchDay(move.FirstTeam(), move.MatchDay());
    int currentSecondTeamMatchId = state.GetMatchIDForTeamInMatchDay(move.SecondTeam(), move.MatchDay());

    int currentSecondLegFirstTeamMatchId = state.GetMatchIDForTeamInMatchDay(move.FirstTeam(), move.MatchDay() + in.MatchDays());
    int currentSecondLegSecondTeamMatchId = state.GetMatchIDForTeamInMatchDay(move.SecondTeam(), move.MatchDay() + in.MatchDays());

    int futureOpponentOfFirstTeam = state.GetOpponentForTeamInMatchday(move.SecondTeam(), move.MatchDay());
    int futureOpponentOfSecondTeam = state.GetOpponentForTeamInMatchday(move.FirstTeam(), move.MatchDay());

    // cout << "Current matches: " << move.firstTeam << " v " << state.GetOpponentForTeamInMatchday(move.FirstTeam(), move.MatchDay()) << ", " << move.secondTeam << " v " << state.GetOpponentForTeamInMatchday(move.SecondTeam(), move.MatchDay()) << endl;

    int futureFirstTeamMatchId;
    int futureSecondTeamMatchId;

    int futureSecondLegFirstTeamMatchId;
    int futureSecondLegSecondTeamMatchId;

    if(state.AtHome(move.SecondTeam(), move.MatchDay())){
        futureFirstTeamMatchId = util.GetMatchId(move.FirstTeam(), futureOpponentOfFirstTeam);
        futureSecondLegFirstTeamMatchId = util.GetMatchId(futureOpponentOfFirstTeam, move.FirstTeam());
    }else{
        futureFirstTeamMatchId = util.GetMatchId(futureOpponentOfFirstTeam, move.FirstTeam());
        futureSecondLegFirstTeamMatchId = util.GetMatchId(move.FirstTeam(), futureOpponentOfFirstTeam);
    }

    if(state.AtHome(move.FirstTeam(), move.MatchDay())){
        futureSecondTeamMatchId = util.GetMatchId(move.SecondTeam(), futureOpponentOfSecondTeam);
        futureSecondLegSecondTeamMatchId = util.GetMatchId(futureOpponentOfSecondTeam, move.SecondTeam());
    }else{
        futureSecondTeamMatchId = util.GetMatchId(futureOpponentOfSecondTeam, move.SecondTeam());
        futureSecondLegSecondTeamMatchId = util.GetMatchId(move.SecondTeam(), futureOpponentOfSecondTeam);
    }


    if(in.ForbiddenTogether(currentFirstTeamMatchId, currentSecondTeamMatchId)){
        delta -= 1; //eventually checking if those two matchids are forbidden together. because in the loop we will skip their iteration
    }

    if(in.ForbiddenTogether(futureFirstTeamMatchId, futureSecondTeamMatchId)){
        delta += 1; //eventually checking if those two matchIds are forbidden together. because there will be no loop iteration checking this
    }

    for(int t = 0; t < in.Teams(); t++){
        if(!state.AtHome(t, move.MatchDay())){continue;} // avoud double counting

        int matchId = state.GetMatchIDForTeamInMatchDay(t, move.MatchDay());

        if(matchId == currentFirstTeamMatchId || matchId == currentSecondTeamMatchId){continue;} //For the remove of violations we already counted above, but we don't want to count hipotetic 'introductions' of violations
                                                                                                 //with these two ids that will disappear

        if(in.ForbiddenTogether(matchId, currentFirstTeamMatchId)){
            delta -= 1;
        }
        if(in.ForbiddenTogether(matchId, currentSecondTeamMatchId)){
            delta -= 1;
        }

        if(in.ForbiddenTogether(matchId, futureFirstTeamMatchId)){
            delta += 1;
        }
        if(in.ForbiddenTogether(matchId, futureSecondTeamMatchId)){
            delta += 1;
        }
    }

    //handling second leg 

    if(in.ForbiddenTogether(currentSecondLegFirstTeamMatchId, currentSecondLegSecondTeamMatchId)){
        delta -= 1; //eventually checking if those two matchids are forbidden together. because in the loop we will skip their iteration
    }

    if(in.ForbiddenTogether(futureSecondLegFirstTeamMatchId, futureSecondLegSecondTeamMatchId)){
        delta += 1; //eventually checking if those two matchIds are forbidden together. because there will be no loop iteration checking this
    }

    for(int t = 0; t < in.Teams(); t++){
        if(!state.AtHome(t, move.MatchDay() + in.MatchDays())){continue;} // avoud double counting

        int matchId = state.GetMatchIDForTeamInMatchDay(t, move.MatchDay() + in.MatchDays());

        if(matchId == currentSecondLegFirstTeamMatchId || matchId == currentSecondLegSecondTeamMatchId){continue;} //For the remove of violations we already counted above, but we don't want to count hipotetic 'introductions' of violations
                                                                                                 //with these two ids that will disappear

        if(in.ForbiddenTogether(matchId, currentSecondLegFirstTeamMatchId)){
            delta -= 1;
        }
        if(in.ForbiddenTogether(matchId, currentSecondLegSecondTeamMatchId)){
            delta -= 1;
        }

        if(in.ForbiddenTogether(matchId, futureSecondLegFirstTeamMatchId)){
            delta += 1;
        }
        if(in.ForbiddenTogether(matchId, futureSecondLegSecondTeamMatchId)){
            delta += 1;
        }
    }

    return delta;
}

int CHForbiddenTogetherDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    STT_Util util(in.Teams());

    int moveMatchDay = move.MatchDay();
    int team = move.Team();
    int currentMatchId = state.GetMatchIDForTeamInMatchDay(team, moveMatchDay);
    int currentSecondLegMatchId = state.GetMatchIDForTeamInMatchDay(team, moveMatchDay + in.MatchDays());
    int delta = 0; 

    int newMatchId, newSecondLegMatchId, opponentOfTeam;
    opponentOfTeam = state.GetOpponentForTeamInMatchday(team, move.MatchDay());
    if(state.AtHome(team, moveMatchDay)){
        //if it's currenty at home, it will become away after the move is performed 
        newMatchId = util.GetMatchId(opponentOfTeam, team);
        newSecondLegMatchId = util.GetMatchId(team, opponentOfTeam);
    }else{
        //if it's away, it will become home
        newMatchId = util.GetMatchId(team, opponentOfTeam);
        newSecondLegMatchId = util.GetMatchId(opponentOfTeam, team);
    }

    for(int t = 0; t < in.Teams(); t++){
        if(!state.AtHome(t, moveMatchDay)){continue;}
        int matchId = state.GetMatchIDForTeamInMatchDay(t, moveMatchDay);
        if(matchId == currentMatchId){continue;};

        if(in.ForbiddenTogether(currentMatchId, matchId)){
            delta -= 1;
        }
        if(in.ForbiddenTogether(newMatchId, matchId)){
            delta += 1;
        }
    }

    // handling second leg

    for(int t = 0; t < in.Teams(); t++){
        if(!state.AtHome(t, moveMatchDay + in.MatchDays())){continue;}
        int matchId = state.GetMatchIDForTeamInMatchDay(t, moveMatchDay + in.MatchDays());
        if(matchId == currentSecondLegMatchId){continue;};

        if(in.ForbiddenTogether(currentSecondLegMatchId, matchId)){
            delta -= 1;
        }
        if(in.ForbiddenTogether(newSecondLegMatchId, matchId)){
            delta += 1;
        }
    }

    return delta;
}

/* Should Play At Home */

int SwapShouldPlayAtHomeDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_Swap& move) const {

    int delta = 0;

    int secondLegMatchday = in.MatchDays() + move.MatchDay(); //if a match is played in matchday m, and hospitality might change, also matchday m + in.MatchDays() will be effected

    bool firstTeamAtHome = state.AtHome(move.FirstTeam(), move.MatchDay());
    bool secondTeamAtHome = state.AtHome(move.SecondTeam(), move.MatchDay());

    bool firstTeamAtHomeSecondLeg = !firstTeamAtHome;
    bool secondTeamAtHomeSecondLeg = !secondTeamAtHome;

    if(!firstTeamAtHome && in.ShouldPlayAtHome(move.FirstTeam(), move.MatchDay())){
        //if firstTeam was not playing at home, but it should have, I'm removing one violation
        delta -= 1;
    }

    if(!secondTeamAtHome && in.ShouldPlayAtHome(move.SecondTeam(), move.MatchDay())){
        //if second team was not playing at home, but it should have, I'm removing one violation
        delta -= 1;
    }

    if(!firstTeamAtHomeSecondLeg && in.ShouldPlayAtHome(move.FirstTeam(), secondLegMatchday)){
        //if firstTeam was not playing at home in the second leg, but it should have, I'm removing one violation
        delta -= 1;
    }

    if(!secondTeamAtHomeSecondLeg && in.ShouldPlayAtHome(move.SecondTeam(), secondLegMatchday)){
        //if second team was not playing at home in the second leg , but it should have, I'm removing one violation
        delta -= 1;
    }

    if(!secondTeamAtHome && in.ShouldPlayAtHome(move.FirstTeam(), move.MatchDay())){
        //if first team WILL not play at home, but it should have, I'm adding one violation
        delta += 1;
    }
    
    if(!firstTeamAtHome && in.ShouldPlayAtHome(move.SecondTeam(), move.MatchDay())){
        //if second team WILL not play at home, but it should have, I'm adding one violation
        delta += 1;
    }

    if(secondTeamAtHome && in.ShouldPlayAtHome(move.FirstTeam(), secondLegMatchday)){
        //if the first team WILL play at home match in matchday m (so away for second leg), but in the second leg it should have been home, add one violation
        delta += 1;
    }

    if(firstTeamAtHome && in.ShouldPlayAtHome(move.SecondTeam(), secondLegMatchday)){
        //if the second team WILL play at home match in matchday m (so away for second leg), but in the second leg it should have been home, add one violation
        delta += 1;
    }

    return delta;
}

int CHShouldPlayAtHomeDeltaCostComponent::ComputeDeltaCost(const STT_State& state, const STT_Move_ChangeHospitality& move) const {
    int delta = 0; 
    bool teamPlaysAtHome = state.AtHome(move.Team(), move.MatchDay());
    int secondLegMatchday = in.MatchDays() + move.MatchDay();

    if(teamPlaysAtHome && in.ShouldPlayAtHome(move.Team(), move.MatchDay())){
        //from home it will change to away, so one violation
        delta += 1;
    }

    if(!teamPlaysAtHome && in.ShouldPlayAtHome(move.Team(), secondLegMatchday)){
        //if team does not play home in matchday m, then it will in the secondLeg. So if it plays home in the second leg (but it is changing hospitality) and it should play home, add one violation
        delta += 1;
    }

    if(!teamPlaysAtHome && in.ShouldPlayAtHome(move.Team(), move.MatchDay())){
        //if it does not play home, but it changes hospitality and it should play at home, remove 1 violation
        delta -= 1;
    }

    if(teamPlaysAtHome && in.ShouldPlayAtHome(move.Team(), secondLegMatchday)){
        //if team plays home, then in the second leg it is away. So by changing hospitality, now in the second leg it is home, and if it has to play home, remove one violation
        delta -= 1;
    }

    bool opponentPlaysAtHome = !teamPlaysAtHome;
    int opponent = state.GetOpponentForTeamInMatchday(move.Team(), move.MatchDay());

    //same ideas as above for computing +1 and -1

    if(opponentPlaysAtHome && in.ShouldPlayAtHome(opponent, move.MatchDay())){
        delta += 1;
    }

    if(!opponentPlaysAtHome && in.ShouldPlayAtHome(opponent, secondLegMatchday)){
        delta += 1;
    }

    if(!opponentPlaysAtHome && in.ShouldPlayAtHome(opponent, move.MatchDay())){
        delta -= 1;
    }

    if(opponentPlaysAtHome && in.ShouldPlayAtHome(opponent, secondLegMatchday)){
        delta -= 1;
    }

    return delta;
}
