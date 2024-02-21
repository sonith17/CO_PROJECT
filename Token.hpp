
#ifndef TOKEN_H
#define TOKEN_H

#include<string>
using namespace std;

struct Token{
    int line;
    string Name;
    Token(int line,string Name)
    {
        this->line = line;
        this->Name = Name;
    }
    Token()
    {
        
    }

};


struct DataToken{
    int address;
    int size;
    string varName;
    string type;

    DataToken(int address,int size,string varname,string type)
    {
        this->address=address;
        this->size=size;
        this->varName=varName;
        this->type=type;
    }
    DataToken()
    {
        
    }
};

#endif