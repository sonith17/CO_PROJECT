#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include "Token.hpp"
using namespace std;


Token tokenize(string Name,int line)
{
     Token tr(line,Name);
     return tr;
}

vector<string> tokenizeDataLine(string DataLine)
{
    vector<string> tokens;
    string s ="";
    bool str = false;
    for(int i =0;i<DataLine.length();i++)
    {
        if(DataLine[i] == '#')
        {
            return tokens;
        }
        if(DataLine[i]!=' '&& !str && DataLine[i]!='"')
        {
            s=s+DataLine[i];
        }
        else if(str)
        {
            s=s+DataLine[i];
        }
        else if(DataLine[i]=='"')
        {
            str = true;
            s=s+DataLine[i];
            
        }
        else
        {
            if(s!=""){
            tokens.push_back(s);
            s="";
            }
        }
    }
    if(s!="")
    {
        tokens.push_back(s);
        s="";
    }
    return tokens;
}

void storeAsHex(int8_t memory[],string value,int address)
{
    int length  = value.length();
    unsigned int hexa;
    stringstream ss;
    ss << std::hex << value;
    ss >> hexa;
    uint8_t *po;
    po = (uint8_t*)&hexa;
    for(int i =0;i<4;i++)
    {
        memory[address+i] = *(po+i);
    }
}




void storeData(int8_t memory[],string DataLine,map<string,DataToken> &dataLabels,int &address)
{
    vector<string> tokens  = tokenizeDataLine(DataLine);
    if(tokens.size() == 0)
    {
        return;
    }
    int size = tokens.size();
    if(tokens.at(0)==".string"|| tokens.at(0)==".word")
    {
        if(tokens.at(0)==".string")
        {
            for(int i=1;i<tokens.at(1).length()-2;i++)
            {
                memory[address]= (int)tokens.at(1)[i];
                address++;
            }
        }
        else
        {
            for(int i =1;i<size;i++)
            {
                storeAsHex(memory,tokens.at(i),address);
                address+=4;
            }
        }
    }
    else
    {
        string label = tokens.at(0).substr(0,tokens.at(0).length()-1);
        
        tokens.erase(tokens.begin());
        size--;
        if(tokens.at(0)==".string")
        { 
            int preAddress = address; 
            for(int i=1;i<tokens.at(1).length()-2;i++)
            {
                memory[address]= (int)tokens.at(1)[i];
                address++;
            }
            DataToken dataLabel(preAddress,address-preAddress,label,"string");
            dataLabels[label]=dataLabel;
        }
        else
        {
            int preAddress = address; 
            for(int i =1;i<size;i++)
            {
                storeAsHex(memory,tokens.at(i),address);
                address+=4;
            }
            DataToken dataLabel(preAddress,address-preAddress,label,"word");
            dataLabels[label]=dataLabel;
        }
    }
}


bool isData(string ProgramLine)
{
    string s ="";
    for(int i=0;i<ProgramLine.length();i++)
    {
        if(ProgramLine[i]!=' ')
        {
            s=s+ProgramLine[i];
        }
    }
    if(s==".data")
    {
        return true;
    }
    return false;
}

bool isText(string ProgramLine)
{
    string s ="";
    for(int i=0;i<ProgramLine.length();i++)
    {
        if(ProgramLine[i]!=' ')
        {
            s=s+ProgramLine[i];
        }
    }
    if(s==".text")
    {
        return true;
    }
    return false;


}



void LabelTokenizer(map <string,int>& labels,string line,int lineNumber)
{
    string s ="";
    for(char c : line)
    {
        if(c==':')
        {
            labels[s]=lineNumber; // labels of the whole program has been stored here.
            return;
        }
        if(c!=' ')s=s+c;
    }
}

map<string,int> get_labels(vector<string> Program)
{
    map <string,int> labels;
    for(int i=0;i<Program.size();i++)
    {
        LabelTokenizer(labels,Program.at(i),i);
    }
    return labels;
}

vector<Token> Tokenizer(string line,int pc)
{
    vector<Token> tokens;
    string s;
    for(int i =0;i<line.length();i++)
    {
        if(line[i]!= ' ' && line[i]!= ',' && !(line[i]==':') && line[i]!='(' && line[i]!=')' && line[i]!='#')
        {
            s=s+line[i];
        }
        else if(line[i]=='#')
        {
            return tokens;
        }
        else if(line[i]==':')
        {
            s=s+line[i];
           if(s!="")
            {
                tokens.push_back(tokenize(s,pc));
            }
            s="";
        }
        else if(line[i]=='(')
        {
            if(s!="")
            {
                tokens.push_back(tokenize(s,pc));
            }
            s="";
        }
        else if(line[i]==')')
        {

        }
        else
        {
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
    return tokens;
}

