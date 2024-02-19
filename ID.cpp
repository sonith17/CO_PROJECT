#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"
using namespace std;


Token tokenize(string Name,int line)
{
     Token tr(line,Name);
     return tr;
}



void LabelTokenizer(unordered_map <string,int>& labels,string line,int lineNumber)
{
    string s ="";
    for(char c : line)
    {
        if(c==':')
        {
            cout<<"label  --"<<s<<endl;

            labels[s]=lineNumber;
            return;
        }
        if(c!=' ')s=s+c;
    }
}

unordered_map<string,int> get_labels(vector<string> Program)
{
    unordered_map <string,int> labels;
    for(int i=0;i<Program.size();i++)
    {
        LabelTokenizer(labels,Program.at(i),i);
    }
    return labels;
}

vector<Token> Tokenizer(string line,int pc)
{
    //cout<<"line is"<<line<<endl;
    vector<Token> tokens;
    string s;
    for(int i =0;i<line.length();i++)
    {
        //cout<<s<<" jj "<<endl;
        if(line[i]!= ' ' && line[i]!= ',' && !(line[i]==':') && line[i]!='(' && line[i]!=')')
        {
            s=s+line[i];
        }
        else if(line[i]==':')
        {
            s=s+line[i];
            //cout<<s<<"  jh " <<endl;
           if(s!="")
            {
                tokens.push_back(tokenize(s,pc));
            }
            s="";
        }
        else if(line[i]=='(')
        {
            //cout<<s<<" "<<i<<" b ------------------------------------------------------------------------------------------------------------------------"<<endl;
            if(s!="")
            {
                tokens.push_back(tokenize(s,pc));
            }
            s="";
            //cout<<"ki  "<<endl;
            
        }
        else if(line[i]==')')
        {

        }
        else
        {
            //cout<<s<<"  jh " <<endl;
            if(s!="")
            {
                tokens.push_back(tokenize(s,pc));
            }
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


