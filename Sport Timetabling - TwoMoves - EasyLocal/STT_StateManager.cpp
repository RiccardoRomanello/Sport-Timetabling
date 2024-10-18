#include "STT_StateManager.hh"

void STT_StateManager::RandomState(STT_State& state){
    
    state.Reset();

    for(int m = 0; m < in.MatchDays(); m++){
        vector<int> yetToPlay(in.Teams());
        iota(yetToPlay.begin(), yetToPlay.end(), 0);

        while(yetToPlay.size() != 0){
            int teamIndex = Random::Uniform(0, static_cast<int>(yetToPlay.size() - 1));
            int firstTeam = yetToPlay[teamIndex];
            yetToPlay.erase(yetToPlay.begin() + teamIndex);

            int opponentIndex = Random::Uniform(0, static_cast<int>(yetToPlay.size() - 1));
            int secondTeam = yetToPlay[opponentIndex];
            yetToPlay.erase(yetToPlay.begin() + opponentIndex);

            state.SetOpponentForTeam(firstTeam, secondTeam, m);

            state.SetAtHomeForMatchday(firstTeam, m, Random::Uniform(0, 1) == 1);
        }
    }

    for(unsigned t = 0; t < in.Teams(); t++){
        state.ComputeLeftSideOfFirstGameHAForTeam(t);
        state.ComputeRightSideOfFirstGameHAForTeam(t);
    }
}

bool STT_StateManager::CheckConsistency(const STT_State &st) const {

    //checking that playsAtHome and teamSideTournament are ok
    for(int m = 0; m < in.MatchDays(); m++){
        vector<bool> tagged(in.Teams(), false);
        for(int t = 0; t < in.Teams(); t++){
            if(st.AtHome(t, m) == st.AtHome(st.GetOpponentForTeamInMatchday(t, m), m)){
                cout << "Team " << t << " and team " << st.GetOpponentForTeamInMatchday(t, m) << " plays wrongly in matchday " << m << endl; 
                return false;
            }
            if(tagged[st.GetOpponentForTeamInMatchday(t, m)]){
                cout << "Team " << st.GetOpponentForTeamInMatchday(t, m) << " was already tagged" << endl;
                return false;
            }
            tagged[st.GetOpponentForTeamInMatchday(t, m)] = true;
        }
    }
    //Recomputing against team
    vector<vector<int>> againstTeamCountRecomputed(in.Teams(), vector<int>(in.Teams(), 0)); 
    for(int t = 0; t < in.Teams(); t++){
        for(int m = 0; m < in.MatchDays(); m++){
            if(!st.AtHome(t, m)){continue;}
            againstTeamCountRecomputed[t][st.GetOpponentForTeamInMatchday(t, m)]++;
            againstTeamCountRecomputed[st.GetOpponentForTeamInMatchday(t, m)][t]++;
        }
    }

    for(int t = 0; t < in.Teams(); t++){
        for(int t1 = 0; t1 < in.Teams(); t1++){
            if(againstTeamCountRecomputed[t][t1] != st.HowManyMatchesPlayedAgainst(t, t1)){
                cout << "The count of matches between " << t << " and " << t1 << " is wrong" << endl; 
                cout << "In data structure: " << st.HowManyMatchesPlayedAgainst(t, t1) << " while actually it is " << againstTeamCountRecomputed[t][t1] << endl;
                return false;
            }
        }
    }

    
    for(int m = 0; m < in.MatchDays(); m++){
        int bigMatchesInCurrentMatchday = 0; 
        for(int t = 0; t < in.Teams(); t++){
            if(!st.AtHome(t, m)){continue;}
            if(in.TopTeam(t) && in.TopTeam(st.GetOpponentForTeamInMatchday(t, m))){
                bigMatchesInCurrentMatchday++;
            }
        }
        if(bigMatchesInCurrentMatchday != st.CountOfBigMatches(m)){
            cout << "Matchdays in day " << m << " is wrong" << endl;
            return false;
        }
    }

    for(int t = 0; t < in.Teams(); t++){
        for(int m = 0; m < in.TotalMatchDays(); m++){
            
            int leftCloserAway = st.FirstGameAsAway(t, m).first;
            int rightCloserAway = st.FirstGameAsAway(t, m).second;
            //everything that is in leftCloserAway + 1, rightCloserAway - 1 is H

            for(int m1 = leftCloserAway + 1; m1 < rightCloserAway; m1++){
                if(m1 == m){continue;}

                if(!st.AtHome(t, m1)){
                    cout << "first game as away contains this index but actually team " << t << " is playing away" << endl; 
                    return false;
                }
            }
            if(leftCloserAway != -1 && st.AtHome(t, leftCloserAway)){
                cout << "This cannot be used as first game as away! it's an home game" << endl;
                return false;
            }

            if(rightCloserAway != in.TotalMatchDays() && st.AtHome(t, rightCloserAway)){
                cout << "This cannot be used as first game as away! it's an home game" << endl;
                return false;
            }
        }
    }

    for(int t = 0; t < in.Teams(); t++){
        for(int m = 0; m < in.TotalMatchDays(); m++){

            int leftCloserHome = st.FirstGameAsHome(t, m).first;
            int rightCloserHome = st.FirstGameAsHome(t, m).second;

            //everything that is in leftCloserHome + 1, rightCloserHome - 1 is V
            for(int m1 = leftCloserHome + 1; m1 < rightCloserHome; m1++){
                if(m1 == m){continue;}
                
                if(st.AtHome(t, m1)){
                    cout << "first game as home contains this index but actually team " << t << " is playing at home" << endl; 
                    return false;
                }
            }
            if(leftCloserHome != -1 && !st.AtHome(t, leftCloserHome)){
                cout << "This cannot be included in first game as home! it's an away game" << endl;
                return false;
            }
            if(rightCloserHome != in.TotalMatchDays() && !st.AtHome(t, rightCloserHome)){
                cout << "This cannot be included in first game as home! it's an away game" << endl;
                return false;
            }
        }
    }

    return true;
}
