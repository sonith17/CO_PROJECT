
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

};

#endif