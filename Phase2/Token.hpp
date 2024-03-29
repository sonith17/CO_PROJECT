
#ifndef TOKEN_H
#define TOKEN_H

#include<string>

struct Token{
    int line;
    std::string Name;
    Token(int line,std::string Name)
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
    std::string varName;
    std::string type;

    DataToken(int address,int size,std::string varname,std::string type)
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