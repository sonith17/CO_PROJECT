#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "instruction.hpp"
#include <stdint.h>
#include "EXE.cpp"
#include <utility>


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


    void RunPipeline(int8_t memory[])
    {

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

    std::string instructionFetch(int pc)
    {
        std::string instructType = insType[pc];
        return instructType;
    }
    std::vector<std::string> instructionDecode(int pc , std::string instructType)
    {
        std::vector<std::string> result;
        if(instructType =="Rtype")
        {
            result.push_back((rtype[pc].opcode));
            result.push_back(std::to_string(rtype[pc].dest));
            result.push_back(std::to_string(rtype[pc].src1));
            result.push_back(std::to_string(rtype[pc].src2));
        }
        else if(instructType=="Itype")
        {
            result.push_back((itype[pc].opcode));
            result.push_back(std::to_string(itype[pc].dest));
            result.push_back(std::to_string(itype[pc].src1));
            result.push_back(std::to_string(itype[pc].immed));
        }
         else if(instructType=="Stype")
        {
            result.push_back((stype[pc].opcode));
            result.push_back(std::to_string(stype[pc].src1));
            result.push_back(std::to_string(stype[pc].immed));
            result.push_back(std::to_string(stype[pc].dest));
        }
         else if(instructType=="SBtype")
        {
            result.push_back((sbtype[pc].opcode));
            result.push_back(std::to_string(sbtype[pc].src1));
            result.push_back(std::to_string(sbtype[pc].src2));
            result.push_back((sbtype[pc].label));
        }
        else if(instructType=="Utype")
        {
            result.push_back((utype[pc].opcode));
            result.push_back(std::to_string(utype[pc].dest));
            result.push_back(std::to_string(utype[pc].immed));
        }
        else if(instructType=="UJ1type")
        {
            result.push_back((ujtype[pc].opcode));
            result.push_back(std::to_string(ujtype[pc].dest));
            result.push_back(std::to_string(ujtype[pc].immed));
        }
        return result;
    }

    std::vector<std::string> Execute(int pc, std::vector<std::string> IDResults){
        std::vector<std::string> exResults;
        if(IDResults.at(0) == "add")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] + registers[std::stoi(IDResults.at(3))])); // value
        }
        else if(IDResults.at(0) == "sub")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] - registers[std::stoi(IDResults.at(3))])); // value
        }
        else if(IDResults.at(0) == "and")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] & registers[std::stoi(IDResults.at(3))])); // value
        }
        else if(IDResults.at(0) == "or")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] | registers[std::stoi(IDResults.at(3))])); // value
        }
        else if(IDResults.at(0) == "xor")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] ^ registers[std::stoi(IDResults.at(3))])); // value
        }
        else if(IDResults.at(0) == "sll")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] >> registers[std::stoi(IDResults.at(3))])); // value
        }
        else if(IDResults.at(0) == "srl")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] << registers[std::stoi(IDResults.at(3))])); // value
        }
        else if(IDResults.at(0) == "slt")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            if(registers[std::stoi(IDResults.at(2))] < registers[std::stoi(IDResults.at(3))])
            {
                exResults.push_back(std::to_string(1)); // value
            }
            else{
                exResults.push_back(std::to_string(0)); // value
            }
        }
        else if(IDResults.at(0) == "addi")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3))));
        }
        else if(IDResults.at(0) == "andi")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] & std::stoi(IDResults.at(3))));
        }
        else if(IDResults.at(0) == "ori")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] | std::stoi(IDResults.at(3))));
        }
        else if(IDResults.at(0) == "xori")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] ^ std::stoi(IDResults.at(3))));
        }
        else if(IDResults.at(0) == "slli")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] << std::stoi(IDResults.at(3))));
        }
        else if(IDResults.at(0) == "srli")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] >> std::stoi(IDResults.at(3))));
        }
        else if(IDResults.at(0) == "slti")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            if(registers[std::stoi(IDResults.at(2))] < std::stoi(IDResults.at(3)))
            {
                exResults.push_back("1");
            }
            else
            {
                exResults.push_back("0");
            }
        }
        else if(IDResults.at(0) == "jalr")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(pc+1));

            pc = registers[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3));
            return exResults; 
        }
        else if(IDResults.at(0) == "lw")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3))));
        }
        else if(IDResults.at(0) == "sw")
        {
            exResults.push_back(IDResults.at(0));  // opcode
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(3))] + std::stoi(IDResults.at(2)))); // destination
            exResults.push_back((IDResults.at(1)));  // value
        }
        else if(IDResults.at(0) == "bne")
        {
            if(IDResults.at(1) != IDResults.at(2))
            {
                pc = labels[IDResults.at(3)];
            }
            exResults.push_back(IDResults.at(0));
            exResults.push_back("-1");
            exResults.push_back("-1");
        }
        else if(IDResults.at(0) == "beq")
        {
            if(IDResults.at(1) == IDResults.at(2))
            {
                pc = labels[IDResults.at(3)];
            }
            exResults.push_back(IDResults.at(0));
            exResults.push_back("-1");
            exResults.push_back("-1");
            return exResults;
        }
        else if(IDResults.at(0) == "bge")
        {
            if(IDResults.at(1) >= IDResults.at(2))
            {
                pc = labels[IDResults.at(3)];
            }
            exResults.push_back(IDResults.at(0));
            exResults.push_back("-1");
            exResults.push_back("-1");
            return exResults;
        }
        else if(IDResults.at(0) == "blt")
        {
            if(IDResults.at(1) < IDResults.at(2))
            {
                pc = labels[IDResults.at(3)];
            }
            exResults.push_back(IDResults.at(0));
            exResults.push_back("-1");
            exResults.push_back("-1");
            return exResults;
        }
        else if(IDResults.at(0) == "auipc")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(IDResults.at(2));
        }
        else if(IDResults.at(0) == "jal")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(pc+1));
            pc = std::stoi(IDResults.at(2));
            return exResults;
        }
        pc++;
        return exResults;
    }


    std::pair<std::string, std::string> MEM(int8_t memory[],std::vector<std::string> exResults)
    {
        std::pair<std::string, std::string> memResults;
        if(exResults.at(0)=="lw")
        {
            int address = std::stoi(exResults.at(2));
            int value = (int)memory[address]+(256*(int)memory[address+1])+(65536*(int)memory[address+2])+(16777216*(int)memory[address+3]);
            memResults.first = exResults.at(1);
            memResults.second = std::to_string(value);
        }
        else if(exResults.at(0) == "sw")
        {
            uint8_t *po;
            po = (uint8_t*)&registers[std::stoi(exResults.at(2))];
            int address = registers[std::stoi(exResults.at(1))];
            for(int i =0;i<4;i++)
            {
                memory[address+i] = *(po+i);
            }
            memResults.first = "-1";
            memResults.second = "-1";
        }
        else if(exResults.at(1) != "-1" && exResults.at(2) != "-1")
        {
            memResults.first = exResults.at(1);
            memResults.second = exResults.at(2);
        }
        return memResults;
    }

    void WriteBack(std::pair<std::string, std::string> memResults)
    {
        if(memResults.first != "-1" && memResults.second != "-1")
        {
            registers[std::stoi(memResults.first)] = std::stoi(memResults.second);
        }
    }
};
