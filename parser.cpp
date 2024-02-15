#include <iostream>
#include <string>
#include <vector>

using namespace std;

int parse(string line,int registers[],int memory[],int pc)
{
    vector<string> tokens;
    int i =0;
    string token ="";
    while(i<line.length()){
        if(isalnum(line[i]))
        {
            token = token+line[i];
        }
        else
        {
            if(token!=""){
                tokens.push_back(token);
                cout<<token<<endl;
            }
            token = "";
        }
        i++;
    }
    if(token!="")
    {
        tokens.push_back(token);
        cout<<token<<endl;
    }
    if(tokens[0]== "add"){
        pc = pc+1;
    }
    return pc;
}