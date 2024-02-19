#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "EXE.cpp"

using namespace std;

class Core{
    public:
    int registers[32]={0};
    int programCounter =0;
    vector<string> Program;
    unordered_map<string, int> labels;

    void getLabels()
    {
        labels = get_labels(Program);
    }


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
        
        if(programCounter>=Program.size()) return 0;
        //cout<<"JU"<<endl;

        vector <Token> LineTokens = Tokenizer(Program[programCounter],programCounter);

        for(auto x:LineTokens)
        {
            cout<<x.Name<<endl;
        }
        EXE executer;
        // //programCounter++;
        int address = executer.execute(LineTokens,registers,programCounter,labels);
        if(address != -1)
        {
            if(LineTokens[0].Name=="sw")
            {
                memory[address]=registers[stoi(LineTokens[1].Name.substr(1,LineTokens[1].Name.length()-1))];
            }
            else if(LineTokens[0].Name=="lw")
            {
                registers[stoi(LineTokens[1].Name.substr(1,LineTokens[1].Name.length()-1))] = memory[address];
            }
        }
        return 1;
    }
    

};