#include<iostream>
#include<string>
#include<vector>
#include <map>
#include <unordered_map>
#include "Token.hpp"
#include "ID.cpp"
using namespace std;

class EXE
{
private:
    map<string,int> Register;
public:
    EXE();
    int  execute(vector<Token> tokens, int registers[], int &pc,unordered_map<string, int > &labels);
};

EXE::EXE()
{
    string x = "x";
    for(int i = 0;i<32;i++)
    {
        Register.insert({x+to_string(i),i});
    }
}

int EXE:: execute(vector<Token> tokens, int registers[], int &pc,unordered_map<string, int > &labels)
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
    else if(tokens[0].Name[tokens[0].Name.length()-1]==':' && tokens.size()==1)
    {
        labels.insert({tokens[0].Name.substr(0,tokens[0].Name.length()-1),pc+1});
        pc+=1;
    }
    else if(tokens[0].Name=="bne")
    {
        Token sourc1 = tokens[1];
        Token sourc2 = tokens[2];
        Token destination = tokens[3];
        if(registers[Register[sourc1.Name]]!=registers[Register[sourc2.Name]])
        {
            pc = labels[destination.Name];
        }
        else
        {
            pc+=1;
        }
    }
    else if(tokens[0].Name=="sw")
    {
        Token sourc1 = tokens[1];
        Token immediate = tokens[2];
        Token destination = tokens[3];
        pc+=1;
        return stoi(immediate.Name)+registers[Register[sourc1.Name]];
        
    }
    else if(tokens[0].Name=="lw")
    {
        Token destination = tokens[1];
        Token immediate = tokens[2];
        Token sourc1 = tokens[3];
        pc+=1;
        cout<< "result id "<<stoi(immediate.Name)+registers[Register[sourc1.Name]]<<endl;
        return stoi(immediate.Name)+registers[Register[sourc1.Name]];
        
    }
    return -1;
    //cout<<"Execution com"<<endl;
}

