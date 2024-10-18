#include "STT_Input.hh"
#include <fstream>

STT_Input::STT_Input(string fileName){
    const unsigned MAX_DIM = 200;
    char dummyChar, dummyBuffer[MAX_DIM];

    ifstream fileIS(fileName);
    if(!fileIS)
    {
        cerr << "Cannot open input file " <<  fileName << endl;
        exit(1);
    }

    fileIS >> dummyBuffer >> dummyChar >> teamsCount >> dummyChar;

    matchDaysCount = teamsCount - 1;
    totalMatchDaysCount = matchDaysCount + matchDaysCount; // total number of matches is first leg (andata) + second leg (ritorno)

    matchIdsCount = teamsCount * teamsCount;

    isATopTeam.resize(teamsCount, 0);

    forbiddenInMatchDayBool.resize(matchDaysCount, vector<bool>(matchIdsCount, 0));

    forbiddenTogetherBool.resize(matchIdsCount, vector<bool>(matchIdsCount, 0));

    complementary.resize(teamsCount, -1);

    //only this one needs to take care of the whole tournament (both andata and ritorno)
    atHomeBool.resize(teamsCount, vector<bool>(totalMatchDaysCount, 0));

    longestSeqAtHome.resize(teamsCount);
    longestSeqAway.resize(teamsCount);

    fileIS >> dummyBuffer >> dummyChar >> bigMatchesPerMatchday >> dummyChar;

    unsigned t, m, id, id1, id2; //loops vars for team, matchDay and ids
    
    int x;

    fileIS.ignore(MAX_DIM, '[');

    for(t = 0; t < teamsCount; t++){
        fileIS >> x >> dummyChar; //number, space and comma (eventually ] and ;)
        isATopTeam[t] = (x == 1);
        topTeams.push_back(t);
    }

    fileIS.ignore(MAX_DIM, '[');
    fileIS >> dummyChar; //| read
    for(m = 0; m < matchDaysCount; m++){
        for(id = 0; id < matchIdsCount; id++){
            fileIS >> x >> dummyChar; //reading comma after number or eventually the |

            forbiddenInMatchDayBool[m][id] = (x == 1);
        }
    }
    fileIS >> dummyChar >> dummyChar; //reading ] and ;

    fileIS.ignore(MAX_DIM, '[');
    fileIS >> dummyChar; //| read
    for(id1 = 0; id1 < matchIdsCount; id1++){
        for(id2 = 0; id2 < matchIdsCount; id2++){
            fileIS >> x >> dummyChar;

            forbiddenTogetherBool[id1][id2] = x;

        }
    }
    fileIS >> dummyChar >> dummyChar; //reading ] and ;


    fileIS.ignore(MAX_DIM, '[');
    fileIS >> dummyChar; //| read

    for(t = 0; t < teamsCount; t++){
        for(unsigned t1 = 0; t1 < teamsCount; t1++){
            fileIS >> x >> dummyChar;
            if(x == 1){
                complementary[t] = t1;
                complementary[t1] = t;
            }
        }
    }
    fileIS >> dummyChar >> dummyChar; //reading ] and ;

    fileIS.ignore(MAX_DIM, '[');
    fileIS >> dummyChar; //| read
    for(m = 0; m < totalMatchDaysCount; m++){
        for(t = 0; t < teamsCount; t++){
            fileIS >> x >> dummyChar;
            atHomeBool[t][m] = x;
        }
    }
    fileIS >> dummyChar >> dummyChar; //reading ] and ;

    unsigned count;

    fileIS.ignore(MAX_DIM, '[');
    for(t = 0; t < teamsCount; t++){
        fileIS >> count >> dummyChar; //number, space and comma (eventually ] and ;)
        longestSeqAtHome[t] = count;
    }

    fileIS.ignore(MAX_DIM, '[');
    for(t = 0; t < teamsCount; t++){
        fileIS >> count >> dummyChar;
        longestSeqAway[t] = count;
    }
}

ostream& operator<<(ostream& os, const STT_Input &in){

    os << "T = " << in.Teams() << ";" << endl;
    os << "bigMatchesPerMatchDay = " << in.BigMatches() << "; " << endl;  
    os << "topTeam = ["; 
    for(int i = 0; i < in.Teams(); i++){
        os << (in.TopTeam(i) ? 1 : 0);
        if(i != in.Teams()-1){
            os << ", ";
        }
    }
    os << "];" << endl; 

    cout << "forbiddenInMatchday = [|";
    
    for(int m = 0; m < in.MatchDays(); m++){
        for(int id = 0; id < in.IDs(); id++){
            os << (in.ForbiddenInAMatchDay(m, id) ? 1 : 0); 
            if(id != in.IDs() - 1){
                os << ", ";
            }
        }
        os << "|";
        if(m != in.MatchDays() - 1){
            os << endl;
        }
    }
    os << "];" << endl ;

    for(int id1 = 0; id1 < in.IDs(); id1++){
        for(int id2 = 0; id2 < in.IDs(); id2++){
            os << (in.ForbiddenTogether(id1, id2) ? 1 : 0);
            if(id2 != in.IDs() - 1){
                os << ", ";
            }
        }
        os << "|";
        if(id1 != in.IDs() - 1){
            os << endl;
        }
    }
    os << "];" << endl;

    os << "complementary = [|";

    for(int t = 0; t < in.Teams(); t++){
        for(int t2 = 0; t2 < in.Teams(); t2++){
            if(in.ComplementOf(t) == t2){
                os << "1";
            }else{
                os << "0";
            }
            if(t2 != in.Teams() - 1){
                os << ", ";
            } 
        }
        os << "|";
        if(t != in.Teams() - 1){
            os << endl;
        }
    }
    os << "];" << endl;

    os << "atHome = [|"; 
    
    for(int m = 0; m < in.TotalMatchDays(); m++){
        for(int t = 0; t < in.Teams(); t++){
            os << (in.ShouldPlayAtHome(t, m) ? 1 : 0);
            if(t != in.Teams() - 1){
                os << ", ";
            }
        }
        os << "|";
        if(m != in.TotalMatchDays() - 1){
            os << endl;
        }
    }
    os << "];" << endl;

    os << "longestSeqAtHome = ["; 

    for(int t = 0; t < in.Teams(); t++){
        os << in.LongestSeqAtHome(t);
        if(t != in.Teams() - 1){
            os << ", ";
        }
    }
    os << "];" << endl;

    os << "longestSeqAsVisitor = ["; 

    for(int t = 0; t < in.Teams(); t++){
        os << in.LongestSeqAway(t);
        if(t != in.Teams() - 1){
            os << ", ";
        }
    }
    os << "];" << endl;

    return os;
}

istream& operator>>(istream& is, STT_Input &in){
    const unsigned MAX_DIM = 200;
    char dummyChar, dummyBuffer[MAX_DIM];

    is >> dummyBuffer >> dummyChar >> in.teamsCount >> dummyChar;

    in.matchDaysCount = in.teamsCount - 1;
    in.totalMatchDaysCount = in.matchDaysCount + in.matchDaysCount; // total number of matches is first leg (andata) + second leg (ritorno)

    in.matchIdsCount = in.teamsCount * in.teamsCount;

    in.isATopTeam.resize(in.teamsCount, 0);

    in.forbiddenInMatchDayBool.resize(in.matchDaysCount, vector<bool>(in.matchIdsCount, 0));

    in.forbiddenTogetherBool.resize(in.matchIdsCount, vector<bool>(in.matchIdsCount, 0));

    in.complementary.resize(in.teamsCount, -1);

    //only this one needs to take care of the whole tournament (both andata and ritorno)
    in.atHomeBool.resize(in.teamsCount, vector<bool>(in.totalMatchDaysCount, 0));

    in.longestSeqAtHome.resize(in.teamsCount);
    in.longestSeqAway.resize(in.teamsCount);

    is >> dummyBuffer >> dummyChar >> in.bigMatchesPerMatchday >> dummyChar;

    unsigned t, m, id, id1, id2; //loops vars for team, matchDay and ids
    
    int x;

    is.ignore(MAX_DIM, '[');

    for(t = 0; t < in.teamsCount; t++){
        is >> x >> dummyChar; //number, space and comma (eventually ] and ;)
        in.isATopTeam[t] = (x == 1);
        in.topTeams.push_back(t);
    }

    is.ignore(MAX_DIM, '[');
    is >> dummyChar; //| read
    for(m = 0; m < in.matchDaysCount; m++){
        for(id = 0; id < in.matchIdsCount; id++){
            is >> x >> dummyChar; //reading comma after number or eventually the |

            in.forbiddenInMatchDayBool[m][id] = (x == 1);
        }
    }
    is >> dummyChar >> dummyChar; //reading ] and ;

    is.ignore(MAX_DIM, '[');
    is >> dummyChar; //| read
    for(id1 = 0; id1 < in.matchIdsCount; id1++){
        for(id2 = 0; id2 < in.matchIdsCount; id2++){
            is >> x >> dummyChar;

            in.forbiddenTogetherBool[id1][id2] = x;

        }
    }
    is >> dummyChar >> dummyChar; //reading ] and ;


    is.ignore(MAX_DIM, '[');
    is >> dummyChar; //| read

    for(t = 0; t < in.teamsCount; t++){
        for(unsigned t1 = 0; t1 < in.teamsCount; t1++){
            is >> x >> dummyChar;
            if(x == 1){
                in.complementary[t] = t1;
                in.complementary[t1] = t;
            }
        }
    }
    is >> dummyChar >> dummyChar; //reading ] and ;

    is.ignore(MAX_DIM, '[');
    is >> dummyChar; //| read
    for(m = 0; m < in.totalMatchDaysCount; m++){
        for(t = 0; t < in.teamsCount; t++){
            is >> x >> dummyChar;
            in.atHomeBool[t][m] = x;
        }
    }
    is >> dummyChar >> dummyChar; //reading ] and ;

    unsigned count;

    is.ignore(MAX_DIM, '[');
    for(t = 0; t < in.teamsCount; t++){
        is >> count >> dummyChar; //number, space and comma (eventually ] and ;)
        in.longestSeqAtHome[t] = count;
    }

    is.ignore(MAX_DIM, '[');
    for(t = 0; t < in.teamsCount; t++){
        is >> count >> dummyChar;
        in.longestSeqAway[t] = count;
    }

    return is;
}