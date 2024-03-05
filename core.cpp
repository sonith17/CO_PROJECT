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
    bool write[32];
    bool read[32];
    int programCounter =1;
    int NumOfStalls = 0;
    std::vector<std::string> Program;
    std::map<int,std::string> insType;
    std::map<int,RType> rtype;
    std::map<int,IType> itype;
    std::map<int,SType> stype;
    std::map<int,SBType> sbtype;
    std::map<int,UType> utype;
    std::map<int,UJ1Type> ujtype;
    std::vector<std::pair<std::pair<int,int>,int>> pipeline ;
    std::vector<std::vector<int>> Times ;
    std::map<std::string, int> labels;
    std::map<std::string,DataToken> dataLabels;
    bool BranchTaken = false;
    std::string instyp;
    std::vector<std::string> decoded;
    std::vector<std::string> executed;
    std::pair<std::string, std::string> memorized;
    bool toexecute = true;
    bool isRAWHarzad = false;
    std::vector<int> writeRegister;
    std::vector<int> readRegister;
    bool dataForward = true;

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


    int RunPipeline(int8_t memory[])
    {
        std::cout<<programCounter<<"    kl  "<<insType.size()<<std::endl;
        if(programCounter<=insType.size())pipeline.push_back({{programCounter,1},1});
        std::cout << "INSTYP: " << this->instyp << std::endl;
        std::cout << "DECODED:" << std::endl;
        for(auto x: decoded)
        {
            std::cout << x << std::endl;
        }
        std::cout << "Executed: " << std::endl;
        for(auto x: executed)
        {
            std::cout << x << std::endl;
        }
        std::cout << "Memorized: " << std::endl;
        std::cout << memorized.first << "----"<< memorized.second<< std::endl;

        std::cout << "WriteBack" << std::endl;
        for(auto x : pipeline)
        {
            std::cout<<x.first.first<<" pipierc "<<x.first.second<<" "<<x.second<<std::endl;
        }

        std::cout<<"i1"<<std::endl;
        std::cout<<"pr "<<programCounter<<std::endl;
        if(programCounter>insType.size() && pipeline.empty()) return 0;
        int nxtPC;
        bool stall = false;
        std::cout<<"i5"<<std::endl;
        std::string s = insType[programCounter];
        std::cout<<"i17 "<<s<<std::endl;
        if(s=="Rtype")
        {
            std::vector<int> latency = {rtype[programCounter].IF_t,rtype[programCounter].ID_t,rtype[programCounter].EX_t,rtype[programCounter].MEM_t,rtype[programCounter].WB_t};
            Times.push_back(latency);
        }
        if(s=="Itype")
        {
            std::vector<int> latency = {itype[programCounter].IF_t,itype[programCounter].ID_t,itype[programCounter].EX_t,itype[programCounter].MEM_t,itype[programCounter].WB_t};
            Times.push_back(latency);
        }
        if(s=="Stype")
        {
            std::vector<int> latency = {stype[programCounter].IF_t,stype[programCounter].ID_t,stype[programCounter].EX_t,stype[programCounter].MEM_t,stype[programCounter].WB_t};
            Times.push_back(latency);
        }
        if(s=="SBtype")
        {
            std::vector<int> latency = {sbtype[programCounter].IF_t,sbtype[programCounter].ID_t,sbtype[programCounter].EX_t,sbtype[programCounter].MEM_t,sbtype[programCounter].WB_t};
            Times.push_back(latency);
        }
        if(s=="Utype")
        {
            std::cout<<"i16 "<<std::endl;
            std::vector<int> latency = {utype[programCounter].IF_t,utype[programCounter].ID_t,utype[programCounter].EX_t,utype[programCounter].MEM_t,utype[programCounter].WB_t};
            Times.push_back(latency);
        }
        if(s=="UJ1type")
        {
            std::vector<int> latency = {ujtype[programCounter].IF_t,ujtype[programCounter].ID_t,ujtype[programCounter].EX_t,ujtype[programCounter].MEM_t,ujtype[programCounter].WB_t};
            Times.push_back(latency);
        }
        std::cout<<"i6"<<std::endl;
        
        for(int i =0;i<this->pipeline.size();i++)
        {
            std::cout<<"i20 "<<pipeline.size()<<std::endl;
            if(pipeline[i].first.second ==1)
            {
               instyp = instructionFetch(pipeline[i].first.first);
               std::cout<<instyp<<"-------------****-"<<std::endl;
                if(!stall) pipeline[i].second--;
            }
            if(pipeline[i].first.second ==2)
            {
                std::cout<<"fghj"<<std::endl;
                decoded = instructionDecode(pipeline[i].first.first,instyp);
                std::cout<<"fghj  "<<decoded.size()<<std::endl;
                if(!stall) pipeline[i].second--;
            }
            if(pipeline[i].first.second ==3)
            {
                std::cout<<"i21 "<<std::endl;
                int currPC = pipeline[i].first.first;
                std::cout<<"i22 "<<std::endl;
                executed = Execute(pipeline[i].first.first,decoded);
                std::cout<<"i23 "<<std::endl;
                nxtPC = pipeline[i].first.first;
                std:: cout<<nxtPC<<")))))))))))))))))))))"<<currPC<<std::endl;
                if(nxtPC-currPC!=1)
                {

                    BranchTaken = true;
                }
                pipeline[i].first.first = currPC;
                pipeline[i].second--;
                if(pipeline[i].second>0)
                {
                    stall = true;
                }
                else
                {
                    stall = false;
                }
            }
            if(pipeline[i].first.second ==4)
            {
               memorized = MEM(memory,executed);
                pipeline[i].second--;
            }
            if(pipeline[i].first.second ==5)
            {
                WriteBack(memorized);
                pipeline[i].second--;
            }
        }
        std::cout<<"i7"<<std::endl;
        if(pipeline[0].first.second==5 && pipeline[0].second==0)
        {
            pipeline.erase(pipeline.begin());
            Times.erase(Times.begin());
        }
        if(pipeline[0].first.first==-1)
        {
            pipeline.erase(pipeline.begin());
            Times.erase(Times.begin());
            NumOfStalls++;
        }
        std::cout<<"i8"<<std::endl;
        for(int i =0;i<this->pipeline.size();i++)
        {
            std::cout<<"i3"<<std::endl;
            if(pipeline[i].second==0 && (pipeline[i].first.second==3||pipeline[i].first.second==4))
            {
                pipeline[i].first.second++;
                std::cout<<"i11"<<std::endl;
                pipeline[i].second = Times[i][pipeline[i].first.second-1];
            }
             if(pipeline[i].second==0 && (pipeline[i].first.second==1||pipeline[i].first.second==2) && stall)
            {
               
            }
            else if(pipeline[i].second==0 && (pipeline[i].first.second==1||pipeline[i].first.second==2) && !stall)
            {
                pipeline[i].first.second++;
                std::cout<<"i12 "<<i<<" "<<Times.size()<<std::endl;
                pipeline[i].second = Times[i][pipeline[i].first.second-1];
                std::cout<<"i14"<<std::endl;
            }
        }
        std::cout<<"i9"<<std::endl;
        if(stall)
        {
            NumOfStalls++;
        }
        else
        {
            if(BranchTaken)
            {
                pipeline.pop_back();
                pipeline.pop_back();
                Times.pop_back();
                Times.pop_back();
                pipeline.push_back({{-1,-1},-1});
                pipeline.push_back({{-1,-1},-1});
                Times.push_back({-1,-1,-1,-1,-1});
                Times.push_back({-1,-1,-1,-1,-1});
                programCounter = nxtPC;
                //pipeline.push_back({{programCounter,1},1});
                BranchTaken = false;
                NumOfStalls+=2;
            }
            else
            {
                
                programCounter++;
               // pipeline.push_back({{programCounter,1},1});
                
            }
        }
        std::cout<<"i10"<<std::endl;
        std::cout<<"stalls is "<<NumOfStalls<<std::endl;
        printRegisters();
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

    std::string instructionFetch(int pc)
    {
        std::string instructType = insType[pc];
        std::cout<<instructType<<"*********"<<pc<<std::endl;
        return instructType;
    }
    std::vector<std::string> instructionDecode(int pc , std::string instructType)
    {
        std::vector<std::string> result;
        std::cout<<instructType<<"---------------"<<std::endl;
        if(instructType =="Rtype")
        {
            result.push_back((rtype[pc].opcode));
            result.push_back(std::to_string(rtype[pc].dest));
            result.push_back(std::to_string(rtype[pc].src1));
            result.push_back(std::to_string(rtype[pc].src2));
            writeRegister.push_back(rtype[pc].dest);
            readRegister.push_back(rtype[pc].src1);
            readRegister.push_back(rtype[pc].src2);
        }
        else if(instructType=="Itype")
        {
            result.push_back((itype[pc].opcode));
            result.push_back(std::to_string(itype[pc].dest));
            result.push_back(std::to_string(itype[pc].src1));
            result.push_back(std::to_string(itype[pc].immed));
            writeRegister.push_back(itype[pc].dest);
            readRegister.push_back(itype[pc].src1);
        }
         else if(instructType=="Stype")
        {
            result.push_back((stype[pc].opcode));
            result.push_back(std::to_string(stype[pc].src1));
            result.push_back(std::to_string(stype[pc].immed));
            result.push_back(std::to_string(stype[pc].dest));
            writeRegister.push_back(stype[pc].dest);
            readRegister.push_back(stype[pc].src1);
        }
         else if(instructType=="SBtype")
        {
            result.push_back((sbtype[pc].opcode));
            result.push_back(std::to_string(sbtype[pc].src1));
            result.push_back(std::to_string(sbtype[pc].src2));
            result.push_back((sbtype[pc].label));
            readRegister.push_back(sbtype[pc].src1);
            readRegister.push_back(sbtype[pc].src2);
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

    std::vector<std::string> Execute(int &pc, std::vector<std::string> IDResults){
        std::vector<std::string> exResults;
        std::cout<<"i26 "<<IDResults.size()<<std::endl;
        if(IDResults.at(0) == "add")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] + registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] + registers[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "sub")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] - registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] - registers[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "and")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] & registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] & registers[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "or")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] | registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] | registers[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "xor")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] ^ registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] ^ registers[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "sll")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] >> registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] >> registers[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "srl")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] << registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] << registers[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "slt")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            if(registers[std::stoi(IDResults.at(2))] < registers[std::stoi(IDResults.at(3))])
            {
                exResults.push_back(std::to_string(1)); // value
                if(this->dataForward)
                {
                    registers[std::stoi(IDResults.at(1))] = 1;
                }

            }
            else{
                exResults.push_back(std::to_string(0)); // value
                if(this->dataForward)
                {
                    registers[std::stoi(IDResults.at(1))] = 0;
                }
            }
        }
        else if(IDResults.at(0) == "addi")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "andi")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] & std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] & std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "ori")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] | std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] | std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "xori")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] ^ std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] ^ std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "slli")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] >> std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] >> std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "srli")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] << std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = registers[std::stoi(IDResults.at(2))] << std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "slti")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            if(registers[std::stoi(IDResults.at(2))] < std::stoi(IDResults.at(3)))
            {
                exResults.push_back("1");
                if(this->dataForward)
                {
                    registers[std::stoi(IDResults.at(1))] = 1;
                }
            }
            else
            {
                exResults.push_back("0");
                if(this->dataForward)
                {
                    registers[std::stoi(IDResults.at(1))] = 0;
                }
            }
        }
        else if(IDResults.at(0) == "jalr")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(pc+1));
            if(this->dataForward)
            {
                registers[std::stoi(IDResults.at(1))] = pc+1;
            }
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
            if(registers[stoi(IDResults.at(1))] != registers[stoi(IDResults.at(2))])
            {
                pc = labels[IDResults.at(3)];
            }
            else
            {
                pc +=1;
            }
            exResults.push_back(IDResults.at(0));
            exResults.push_back("-1");
            exResults.push_back("-1");
        }
        else if(IDResults.at(0) == "beq")
        {
            if(registers[stoi(IDResults.at(1))] == registers[stoi(IDResults.at(2))])
            {
                pc = labels[IDResults.at(3)];
            }
            else
            {
                pc +=1;
            }
            exResults.push_back(IDResults.at(0));
            exResults.push_back("-1");
            exResults.push_back("-1");
            return exResults;
        }
        else if(IDResults.at(0) == "bge")
        {
            if(registers[stoi(IDResults.at(1))] >= registers[stoi(IDResults.at(2))])
            {
                pc = labels[IDResults.at(3)];
            }
            else
            {
                pc +=1;
            }
            exResults.push_back(IDResults.at(0));
            exResults.push_back("-1");
            exResults.push_back("-1");
            return exResults;
        }
        else if(IDResults.at(0) == "blt")
        {
            if(registers[stoi(IDResults.at(1))] < registers[stoi(IDResults.at(2))])
            {
                pc = labels[IDResults.at(3)];
            }
            else
            {
                pc +=1;
            }
            exResults.push_back(IDResults.at(0));
            exResults.push_back("-1");
            exResults.push_back("-1");
            return exResults;
        }
        else if(IDResults.at(0) == "auipc")
        {
            std::cout<<"i24 "<<exResults.size()<<std::endl;
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(IDResults.at(2));
        }
        else if(IDResults.at(0) == "jal")
        {
            std::cout<<"bupdated --------------------------------"<<pc<<std::endl;
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(pc+1));
            pc = std::stoi(IDResults.at(2));
            std::cout<<"updated --------------------------------"<<pc<<std::endl;
            return exResults;
        }
        pc++;
        return exResults;
    }


    std::pair<std::string, std::string> MEM(int8_t memory[],std::vector<std::string> exResults)
    {
        std::pair<std::string, std::string> memResults;
        std::cout<<exResults[0]<<"-------------------------"<<exResults[1]<<" "<<exResults[2]<<std::endl;
        if(exResults.at(0)=="lw")
        {
            int address = std::stoi(exResults.at(2));
            int value = (int)memory[address]+(256*(int)memory[address+1])+(65536*(int)memory[address+2])+(16777216*(int)memory[address+3]);
            memResults.first = exResults.at(1);
            memResults.second = std::to_string(value);
        }
        else if(exResults.at(0) == "sw")
        {
            std::cout<<" I came "<<std::endl;
            uint8_t *po;
            po = (uint8_t*)&registers[std::stoi(exResults.at(2))];
            int address = std::stoi(exResults.at(1));
            for(int i =0;i<4;i++)
            {
                memory[address+i] = *(po+i);
            }
            memResults.first = "-1";
            memResults.second = "-1";
             std::cout<<" I came1 "<<std::endl;
        }
        else if(exResults.at(1) != "-1" && exResults.at(2) != "-1")
        {
            memResults.first = exResults.at(1);
            memResults.second = exResults.at(2);
        }
        else 
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
            std::cout<<"i26 "<<memResults.first<<" "<<memResults.second<<std::endl;
            registers[std::stoi(memResults.first)] = std::stoi(memResults.second);
        }
         registers[0]=0;
    }
   
};
