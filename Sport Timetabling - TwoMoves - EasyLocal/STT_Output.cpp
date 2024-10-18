#include "STT_Output.hh"

STT_Output::STT_Output(const STT_Input& i) : 
    in(i), tournament(in.MatchDays(), STT_MatchDay(i))
{
}

void STT_Output::SetOpponentForTeamInMatchDay(unsigned matchDay, unsigned team, unsigned opponent){
    tournament[matchDay].SetOpponentForTeam(team, opponent);
}

void STT_Output::SetAtHomeForMatchDay(unsigned matchDay, unsigned team, bool atHome) {
    tournament[matchDay].SetOspitalityForTeam(team, atHome);
}

ostream& operator<<(ostream& os, const STT_Output& out){
    // for(int m = 0; m < out.in.MatchDays(); m++){
    //     os << "-----------" << endl;
    //     os << "| MD   " << (m + 1) << ((m+1) < 10 ? " " : "") << " |   " << endl;
    //     os << out.tournament[m] << endl;
    // }

    for(int m = 0; m < out.in.MatchDays(); m++){
        os << "-----------   ";
    }
    os << endl;
    for(int m = 0; m < out.in.MatchDays(); m++){
        os << "| MD   " << (m + 1) << ((m+1) < 10 ? " " : "") << " |   ";
    }
    os << endl;
    for(int m = 0; m < out.in.MatchDays(); m++){
        //os << out.tournament[m] << endl;
        os << "-----------   " ;
    }
    os << endl;
    vector<unsigned> startHere(out.in.MatchDays(), 0);

    for(unsigned t = 0; t < out.in.Teams()/2; t++){
        for(unsigned m = 0; m < out.in.MatchDays(); m++){
            for(unsigned t1 = startHere[m]; t1 < out.in.Teams(); t1++){
                if(!out.PlaysAtHomeInMatchDay(m, t1)){continue;}
                string teamBias = t1 < 10 ? " " : "";
                unsigned opponent = out.tournament[m].OpponentOfTeam(t1);
                string opponentBias = opponent < 10 ? " " : "";
                os << "| " << t1 << teamBias << " | " << opponent << opponentBias << " |   ";
                startHere[m] = t1 + 1;
                break;
            }
            if(m == out.in.MatchDays() - 1){
                os << endl;
            }
        }
    }
    for(int m = 0; m < out.in.MatchDays(); m++){
        //os << out.tournament[m] << endl;
        os << "-----------   " ;
    }

    os << endl;

    return os;
}

//fix once decided how to format the output --> FIX WITH OUTPUT decided
istream& operator>>(istream& is, STT_Output & out){
    for(int m = 0; m < out.in.MatchDays(); m++){
        for(int t = 0; t < out.in.Teams(); t++){
            char ch; 
            int team;
            int opponent;
            char HV;

            is >> team >> ch >> opponent >> ch >> HV >> ch;
            out.SetOpponentForTeamInMatchDay(m, t, opponent);
            out.SetAtHomeForMatchDay(m, t, (HV == 'H'));
            
        }
    }
    return is;
}
