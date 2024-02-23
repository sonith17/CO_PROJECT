#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include "EXE.cpp"

using namespace std;

class Core{
    public:
    int registers[32]={0};
    int programCounter =0;
    vector<string> Program;
    map<string, int> labels;
    map<string,DataToken> dataLabels;

    void getLabels()
    {
        labels = get_labels(Program);
    }
    void getData(int8_t memory[])
    {
        if(isData(Program.at(0)))
        {
            int i =1,memPointer=256;
            while(i<Program.size() && !isText(Program.at(i)))
            {
                storeData(memory,Program.at(i),dataLabels ,memPointer);
                i++;
            }
            i++;
            programCounter = i;
        }
        if(isText(Program.at(0)))
        {
            programCounter++;
        }
        
    }

    void printRegisters()
    {
        cout << "------------------------------------------------------------------------------------------------------" << endl;
        for(int i=0;i<32;i++)
        {
            cout<< "|" << registers[i]<<" ";
        }
        cout << "|";
        cout<<endl;
        cout << "------------------------------------------------------------------------------------------------------" << endl;
    }
    int executeLine(int8_t memory[])
    {
        if(programCounter>=Program.size()) return 0;
        vector <Token> LineTokens = Tokenizer(Program[programCounter],programCounter);
        if(LineTokens.size()==0)
        {
            programCounter++;
            return 1;
        }
        EXE executer;
        int address = executer.execute(LineTokens,registers,programCounter,labels,dataLabels);
        if(address != -1)
        {
            if(LineTokens[0].Name=="sw")
            {
                uint8_t *po;
                po = (uint8_t*)&registers[stoi(LineTokens[1].Name.substr(1,LineTokens[1].Name.length()-1))];
                for(int i =0;i<4;i++)
                {
                    memory[address+i] = *(po+i);
                }
            }
            else if(LineTokens[0].Name=="lw")
            {
                int value = (int)memory[address]+(256*(int)memory[address+1])+(65536*(int)memory[address+2])+(16777216*(int)memory[address+3]);
                registers[stoi(LineTokens[1].Name.substr(1,LineTokens[1].Name.length()-1))] = value;
            }
        }
        return 1;
    }
    
    void printDataLabels()
    {
        map<string, DataToken>::iterator it = dataLabels.begin();
        while (it != dataLabels.end()) {
            cout << "Key: " << it->first << ", address: " << it->second.address << ", size: " << it->second.size << ", varName: " << it->second.varName << ", type: " << it->second.type << endl;
            ++it;
        }
    }

    void printLabels(){
        map<string, int>::iterator it = labels.begin();
        while(it != labels.end())
        {
            cout << "Key: " << it -> first << ", Value: " << it -> second << endl;
            ++it;
        }
    }
};