#include "STT_Move.hh"

/* Swap move */

STT_Move_Swap::STT_Move_Swap(unsigned team1, unsigned team2, unsigned m){
    firstTeam = team1;
    secondTeam = team2;
    matchDay = m;
}

bool operator==(const STT_Move_Swap& firstMove, const STT_Move_Swap& secondMove){
    return firstMove.matchDay == secondMove.matchDay && firstMove.firstTeam == secondMove.firstTeam && firstMove.secondTeam == secondMove.secondTeam;
}

bool operator!=(const STT_Move_Swap& firstMove, const STT_Move_Swap& secondMove)
{
  return !(firstMove == secondMove);
}

bool operator<(const STT_Move_Swap& firstMove, const STT_Move_Swap& secondMove){
    return firstMove.matchDay < secondMove.matchDay || 
    (firstMove.matchDay == secondMove.matchDay && firstMove.firstTeam < secondMove.firstTeam) || 
    (firstMove.matchDay == secondMove.matchDay && firstMove.firstTeam == secondMove.firstTeam && firstMove.secondTeam < secondMove.secondTeam);
}

ostream& operator<<(ostream& os, const STT_Move_Swap& move)
{
  os << "<" << move.FirstTeam() << "," << move.SecondTeam() << "," << move.MatchDay() << ">";
  return os;
}


istream& operator>>(istream& is, STT_Move_Swap& move)
{
  char ch;
  is >> ch >> move.firstTeam >> ch >> move.secondTeam >> ch >> move.matchDay >> ch;
  return is;
}

/* Change hospitality move */

STT_Move_ChangeHospitality::STT_Move_ChangeHospitality(unsigned t, unsigned m){
    team = t;
    matchDay = m;
}

bool operator==(const STT_Move_ChangeHospitality& firstMove, const STT_Move_ChangeHospitality& secondMove){
    return firstMove.matchDay == secondMove.matchDay && firstMove.team == secondMove.team;
}

bool operator!=(const STT_Move_ChangeHospitality& firstMove, const STT_Move_ChangeHospitality& secondMove)
{
  return !(firstMove == secondMove);
}

bool operator<(const STT_Move_ChangeHospitality& firstMove, const STT_Move_ChangeHospitality& secondMove){
    return firstMove.matchDay < secondMove.matchDay || 
    (firstMove.matchDay == secondMove.matchDay && firstMove.team < secondMove.team);
}

ostream& operator<<(ostream& os, const STT_Move_ChangeHospitality& move)
{
  os << "<" << move.Team() << "," << move.MatchDay() << ">";
  return os;
}


istream& operator>>(istream& is, STT_Move_ChangeHospitality& move)
{
  char ch;
  is >> ch >> move.team >> ch >> move.matchDay >> ch;
  return is;
}

