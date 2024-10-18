#include "STT_OutputManager.hh"

void STT_OutputManager::OutputState(const STT_State &st, STT_Output& out) const {

    for(int m = 0; m < in.MatchDays(); m++){
        for(int t = 0; t < in.Teams(); t++){
            out.SetOpponentForTeamInMatchDay(m, t, st.GetOpponentForTeamInMatchday(t, m));
            out.SetAtHomeForMatchDay(m, t, st.AtHome(t, m));
        }
    }
}

void STT_OutputManager::InputState(STT_State &st, const STT_Output& out) const {
    for(int m = 0; m < in.MatchDays(); m++){
        for(int t = 0; t < in.Teams(); t++){
            if(!out.PlaysAtHomeInMatchDay(m, t)){continue;}
            st.SetOpponentForTeam(t, out.OpponentForTeamInMatchDay(m, t), m);
            st.SetAtHomeForMatchday(t, m, out.PlaysAtHomeInMatchDay(m, t));

        }
    }

    for(unsigned t = 0; t < in.Teams(); t++){
        st.ComputeLeftSideOfFirstGameHAForTeam(t);
        st.ComputeRightSideOfFirstGameHAForTeam(t);
    }
}