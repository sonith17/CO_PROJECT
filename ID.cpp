#include <iostream>
#include <string>
#include <vector>
#include "Token.hpp"
using namespace std;


Token tokenize(string Name,int line)
{
     Token tr(line,Name);
     return tr;
}

vector<Token> Tokenizer(string line,int pc)
{
    //cout<<"line is"<<line<<endl;
    vector<Token> tokens;
    string s;
    for(int i =0;i<line.length();i++)
    {
        //cout<<s<<" jj "<<endl;
        if(line[i]!= ' '&& line[i]!= ',')
        {
            s=s+line[i];
        }
        else
        {
            tokens.push_back(tokenize(s,pc));
            s="";
        }
    }
    if(s!="")
    {
        tokens.push_back(tokenize(s,pc));
    }
    //cout<<"jijrf "<<tokens.size()<<endl;
    return tokens;
}


