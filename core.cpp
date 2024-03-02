#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "instruction.hpp"
#include <stdint.h>
#include "EXE.cpp"


class Core{
    public:
    int registers[32]={0};
    int programCounter =0;
    std::vector<std::string> Program;
    std::map<int,std::string> insType;
    std::map<int,RType> rtype;
    std::map<int,IType> itype;
    std::map<int,SType> stype;
    std::map<int,SBType> sbtype;
    std::map<int,UType> utype;
    std::map<int,UJ1Type> ujtype;

    std::map<std::string, int> labels;
    std::map<std::string,DataToken> dataLabels;

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
        std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
        for(int i=0;i<32;i++)
        {
            std::cout<< "|" << registers[i]<<" ";
        }
        std::cout << "|";
        std::cout<<std::endl;
        std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
    }
    int executeLine(int8_t memory[])
    {
        if(programCounter>=Program.size()) return 0;
        std::vector <Token> LineTokens = Tokenizer(Program[programCounter],programCounter);
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
                storeWord(address, LineTokens, memory);
            }
            else if(LineTokens[0].Name=="lw")
            {
                loadWord(address, LineTokens, memory);
            }
        }
        return 1;
    }

    void loadWord(int address, std::vector<Token> LineTokens, int8_t memory[])
    {
        int value = (int)memory[address]+(256*(int)memory[address+1])+(65536*(int)memory[address+2])+(16777216*(int)memory[address+3]);
        registers[std::stoi(LineTokens[1].Name.substr(1,LineTokens[1].Name.length()-1))] = value;
    }

    void storeWord(int address, std::vector<Token> LineTokens, int8_t memory[])
    {
        uint8_t *po;
        po = (uint8_t*)&registers[std::stoi(LineTokens[1].Name.substr(1,LineTokens[1].Name.length()-1))];
        for(int i =0;i<4;i++)
        {
            memory[address+i] = *(po+i);
        }
    }
    
    void printDataLabels()
    {
        std::map<std::string, DataToken>::iterator it = dataLabels.begin();
        while (it != dataLabels.end()) {
            std::cout << "Key: " << it->first << ", address: " << it->second.address << ", size: " << it->second.size << ", varName: " << it->second.varName << ", type: " << it->second.type << std::endl;
            ++it;
        }
    }

    void printLabels(){
        std::map<std::string, int>::iterator it = labels.begin();
        while(it != labels.end())
        {
            std::cout << "Key: " << it -> first << ", Value: " << it -> second << std::endl;
            ++it;
        }
    }
};