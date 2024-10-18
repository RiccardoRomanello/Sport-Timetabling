#ifndef STT_MOVE_HH
#define STT_MOVE_HH

#include <iostream>
#include <vector>
#include "STT_Input.hh"

class STT_Move_Swap{

    friend bool operator==(const STT_Move_Swap& firstMove, const STT_Move_Swap& secondMove);
    friend bool operator!=(const STT_Move_Swap& firstMove, const STT_Move_Swap& secondMove);
    friend bool operator<(const STT_Move_Swap& firstMove, const STT_Move_Swap& secondMove);
    friend ostream& operator<<(ostream& os, const STT_Move_Swap &move);
    friend istream& operator>>(istream& is, STT_Move_Swap &move);

    public: 
        STT_Move_Swap(unsigned team1 = 0, unsigned team2 = 1, unsigned m = 0);
        unsigned FirstTeam() const {return firstTeam;};
        unsigned SecondTeam() const {return secondTeam;};
        unsigned MatchDay() const {return matchDay;};
        bool IsATeamSwap() const {return firstTeam != secondTeam;};
        bool IsAChangeOspitality() const {return firstTeam == secondTeam;};

        unsigned firstTeam, secondTeam, matchDay;
        /*
        * <f, s, m> -> then in matchday m, f v fo and s v so. 
        * -> After the move we get f v so and s v fo. with ospitality of f that becomes the one of s and viceversa
        */
        
};

class STT_Move_ChangeHospitality{
    friend bool operator==(const STT_Move_ChangeHospitality& firstMove, const STT_Move_ChangeHospitality& secondMove);
    friend bool operator!=(const STT_Move_ChangeHospitality& firstMove, const STT_Move_ChangeHospitality& secondMove);
    friend bool operator<(const STT_Move_ChangeHospitality& firstMove, const STT_Move_ChangeHospitality& secondMove);
    friend ostream& operator<<(ostream& os, const STT_Move_ChangeHospitality &move);
    friend istream& operator>>(istream& is, STT_Move_ChangeHospitality &move);

    public: 
        STT_Move_ChangeHospitality(unsigned t = 0, unsigned m = 0);
        unsigned Team() const {return team;};
        unsigned MatchDay() const {return matchDay;};
        unsigned team, matchDay;
        /*
        * <f, m> -> change ospitality for f in matchDay m
        * if in matchday m f is against fo and f is at home, then after this move f will be Away and fo at home (and viceversa)
        */
};


#endif