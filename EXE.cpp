#include<iostream>
#include<string>
#include<vector>
#include <map>
#include "Token.hpp"
#include "ID.cpp"
using namespace std;

class EXE
{
private:
    map<string,int> Register;
public:
    EXE();
    void execute(vector<Token> tokens, int registers[], int &pc);
};

EXE::EXE()
{
    string x = "x";
    for(int i = 0;i<32;i++)
    {
        Register.insert({x+to_string(i),i});
    }
}

void EXE:: execute(vector<Token> tokens, int registers[], int &pc)
{
    // cout<<Register["x6"]<<endl;
    // cout<<tokens.size()<<endl;
    // cout<<"Execution start"<<endl;
    if(tokens[0].Name=="add")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]+registers[Register[sourc2.Name]];
        pc+=1;
    }
    else if(tokens[0].Name=="sub")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]-registers[Register[sourc2.Name]];
        pc+=1;
    }
    else if(tokens[0].Name=="addi")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token immediate = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]+stoi(immediate.Name);
        pc+=1;
    }
    //cout<<"Execution com"<<endl;
}

