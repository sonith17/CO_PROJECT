#include <iostream>
#include <string>
#include <vector>
#include "EXE.cpp"

using namespace std;

class core{
    public:
    int registers[32]={0};
    int programCounter =0;
    vector<string> Program;
    void Print()
    {
        for(int i=0;i<32;i++)
        {
            cout<<registers[i]<<" ";
        }
        cout<<endl;
    }
    int executeLine(int memory[])
    {
        //cout<<programCounter<<Program.size()<<endl;
        if(programCounter>=Program.size()) return 0;
        //cout<<"JU"<<endl;
        vector <Token> LineTokens = Tokenizer(Program[programCounter],programCounter);
        //cout<<"JU1"<<LineTokens.size()<<endl;
        EXE executer;
        executer.execute(LineTokens,registers,programCounter);
        return 1;
    }
    

};