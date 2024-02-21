#include <iostream>
#include<sstream>
#include <string>
#include <vector>
#include <map>
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
    for(int i =0;i<DataLine.length();i++)
    {
        if(DataLine[i]!=' ')
        {
             //cout<<s<<"qwert !"<<DataLine[i]<<"!asdf"<<endl;
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
        //cout<<s<<endl;
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
    int size = tokens.size();
    if(tokens.at(0)==".string"|| tokens.at(0)==".word")
    {
        if(tokens.at(0)==".string")
        {
            for(int i=1;i<tokens.at(1).length()-1;i++)
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
            for(int i=1;i<tokens.at(1).length()-1;i++)
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
            DataToken dataLabel(preAddress,4,label,"word");
            dataLabels[label]=dataLabel;
            //cout<<" da ta ---------------------------------------------------------------------------------------l "<<dataLabel.address<<endl;
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
            cout<<"label  --"<<s<<endl;

            labels[s]=lineNumber;
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

