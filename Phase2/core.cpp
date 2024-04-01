#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "instruction.hpp"
#include <stdint.h>
#include "EXE.cpp"
#include <utility>
#include "Token.hpp"
#include "id.hpp"


class Core{
    public:
    int registers[32]={0};
    int instructions = 0;
    int programCounter = 1;
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
    bool isRAWHarzad = false;;
    bool dataForward;
    std::vector<std::pair<int,std::pair<int,int>>> inUseRegisters;
    int tempregisters[32];
    bool stall = false;

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
    
    int RunPipeline(int8_t memory[])
    {
        // std::cout << "The Pipeline's 1's line" << std::endl;
        if(programCounter>insType.size() && pipeline.empty()) 
        {
            return 0;
        }
        // std::cout << "The instructions are less than total and Pipeline not empty" << std::endl;
        if(programCounter<=insType.size()&& !isRAWHarzad && !stall)
        {
            pipeline.push_back({{programCounter,1},1});
            // std::cout << "The instruction is less than total and is there is no Data Hazard" << std::endl;
        }
        // std::cout << "Start of the every Pipeline: " << std::endl;
        // std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
        // for(auto x: pipeline)
        // {
        //     std::cout << "PC: " << x.first.first <<" Stage: " << x.first.second << "Times: " << x.second << std::endl;
        // }
        // std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
        // std::cout << "Present information in Temperory registers" << std::endl;

        // std::cout << "INSTYP: " << this->instyp << std::endl;
        // std::cout << "DECODED:" << std::endl;
        // for(auto x: decoded)
        // {
        //     std::cout << x << std::endl;
        // }
        // std::cout << "Executed: " << std::endl;
        // for(auto x: executed)
        // {
        //     std::cout << x << std::endl;
        // }
        // std::cout << "Memorized: " << std::endl;
        // std::cout << "A" << memorized.first << "B" << memorized.second<< std::endl;

        // std::cout << "WriteBack" << std::endl;
        // for(auto x : pipeline)
        // {
        //     std::cout<< "A " << x.first.first<<" B "<<x.first.second<<" C "<<x.second<<std::endl;
        // }

        // std::cout << "Completed in display temperory registers" << std::endl;

        // std::cout << "Displaying inUsedRegisters" << std::endl;
        // for(auto x : inUseRegisters)
        // {
        //     std::cout<<x.first<<" pipierc1 "<<x.second.first<<" "<<x.second.second<<std::endl;
        // }
        // std::cout << "Completed Displaying inUsedRegisters" << std::endl;
        
        int nxtPC;
        stall = false;
        std::string s;
       if(programCounter<=insType.size())
       {
         s = insType[programCounter];
        //  std::cout << "Program Counter " << programCounter << " is less than isType " << insType.size() << std::endl;
       }
        // std::cout << "Inserting the times of each instruction into the pipeline" << std::endl;
        if(s=="Rtype")
        {
            std::vector<int> latency = {rtype[programCounter].IF_t,rtype[programCounter].ID_t,rtype[programCounter].EX_t,rtype[programCounter].MEM_t,rtype[programCounter].WB_t};
            Times.push_back(latency);
            // std::cout << "Inserted RType Times into the pipeline" << std::endl;
        }
        if(s=="Itype")
        {
            std::vector<int> latency = {itype[programCounter].IF_t,itype[programCounter].ID_t,itype[programCounter].EX_t,itype[programCounter].MEM_t,itype[programCounter].WB_t};
            Times.push_back(latency);
            // std::cout << "Inserted IType Times into the pipeline" << std::endl;
        }
        if(s=="Stype")
        {
            std::vector<int> latency = {stype[programCounter].IF_t,stype[programCounter].ID_t,stype[programCounter].EX_t,stype[programCounter].MEM_t,stype[programCounter].WB_t};
            Times.push_back(latency);
            // std::cout << "Inserted SType Times into the pipeline" << std::endl;
        }
        if(s=="SBtype")
        {
            std::vector<int> latency = {sbtype[programCounter].IF_t,sbtype[programCounter].ID_t,sbtype[programCounter].EX_t,sbtype[programCounter].MEM_t,sbtype[programCounter].WB_t};
            Times.push_back(latency);
            // std::cout << "Inserted SBType Times into the pipeline" << std::endl;
        }
        if(s=="Utype")
        {
            // std::cout<<"i16 "<<std::endl;
            std::vector<int> latency = {utype[programCounter].IF_t,utype[programCounter].ID_t,utype[programCounter].EX_t,utype[programCounter].MEM_t,utype[programCounter].WB_t};
            Times.push_back(latency);
            // std::cout << "Inserted UType Times into the pipeline" << std::endl;
        }
        if(s=="UJ1type")
        {
            std::vector<int> latency = {ujtype[programCounter].IF_t,ujtype[programCounter].ID_t,ujtype[programCounter].EX_t,ujtype[programCounter].MEM_t,ujtype[programCounter].WB_t};
            Times.push_back(latency);
            // std::cout << "Inserted UJ1Type Times into the pipeline" << std::endl;
        }
        // std::cout << "Inserting Appropriate Time is completed." << std::endl;
        // std::cout << "The execution of the Pipeline starts for Pipeline.size() " << pipeline.size() << std::endl; 
        for(int i =0;i<this->pipeline.size();i++)
        {
            // std::cout << "1st Line in Pipeline, pc: " << programCounter <<  std::endl;
            if(pipeline[i].first.second ==1 && !isRAWHarzad &&!stall)
            {
                instyp = instructionFetch(pipeline[i].first.first);
                // std::cout<<"Just Now:Instruction Fetch is Completed: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2" << instyp << std::endl;
                if(!stall && !isRAWHarzad)
                {
                    // std::cout << "There is no Stall, and No Raw Hazards , so decreasinig the times " << std::endl; 
                    pipeline[i].second--;
                }
            }
            if(pipeline[i].first.second ==2 && !stall)
            {
                decoded = instructionDecode(pipeline[i].first.first,instyp);
                // std::cout << "Just Now: Instruction Decode has compeleted: "<<std::endl;
                // for(auto x: decoded)
                // {
                //     std::cout << x << " ";
                // }
                // std::cout << std::endl;
                if(!stall && !isRAWHarzad)
                {
                    pipeline[i].second--;
                    // std::cout << "There is no Stall or Hazard, so decreased the times" << std::endl; 
                }
            }
            if(pipeline[i].first.second ==3)
            {
                if(pipeline[i].second == 1)
                {
                    int currPC = pipeline[i].first.first;
                    executed = Execute(pipeline[i].first.first,decoded);
                    // std::cout << "Just Now, Execute has completed: " << std::endl;
                    // for(auto x: executed)
                    // {
                    //     std::cout << x << " ";
                    // }
                    // std::cout << std::endl;
                    nxtPC = pipeline[i].first.first;
                    // std::cout << "Present PC: " << currPC << "Next PC: " << nxtPC << std::endl;
                    if(nxtPC-currPC!=1)
                    {
                        // std::cout << "Branch Taken is turned true" << std::endl;
                        BranchTaken = true;
                    }
                    pipeline[i].first.first = currPC;
                }
                pipeline[i].second--;
                // std::cout << "Decrease the times by 1, after this stage, no conditions check: because it should go till WB()" << std::endl;
                if(pipeline[i].second>0)
                {
                    // std::cout << "Check the Stage of the current instruction and is above 0" << std::endl;
                    // std::cout << "Stall is turned true" << std::endl;
                    stall = true;
                }
                else
                {
                    // std::cout << "The stall is made false" << std::endl;
                    stall = false;
                }
            }
            if(pipeline[i].first.second ==4)
            {
                memorized = MEM(memory,executed);
                // std::cout << "Just Now, MEM is completed " << std::endl;
                // std:: cout << "Dest: " << memorized.first << "Value: " << memorized.second << std::endl;
                pipeline[i].second--;
                // std::cout << "The times of the present instruction has decreased" << std::endl;
            }
            if(pipeline[i].first.second ==5 && pipeline[i].second==1)
            {
                WriteBack(memorized);
                // std::cout << "Just now WriteBack is completed " << std::endl;
                pipeline[i].second--;
                // std::cout << "The times of the present instruction has decreased" << std::endl;
                instructions++;
            }
        }
        // std::cout << "Started to make changes in the pipeline " << std::endl;
        // std::cout << "Before Making Changes: " << std::endl;
        // std::cout << "--------------------------------------------------------------------------------------" << std::endl;
        // for(auto x: pipeline)
        // {
        //     std::cout << "PC: " << x.first.first <<" Stage: " << x.first.second << "Times: " << x.second << std::endl;
        // }
        // std::cout << "---------------------------------------------------------------------------------------" << std::endl;
        if(pipeline[0].first.second==5 && pipeline[0].second<=0)
        {
            // std::cout << "An instruction at the top is at the stage == 5, and times is neagtive and 0" << std::endl;
            pipeline.erase(pipeline.begin());
            // std::cout << "Erased the top instruction & Times in the pipeline" << std::endl;
            Times.erase(Times.begin());
        }
        // std::cout << "If there are pc = -1(stall) in it, we are removing all of it, at the same stage: " << std::endl;
        // std::cout << "Before Stall count: " << NumOfStalls << std::endl;
        while(pipeline[0].first.first==-1)
        {
            // std::cout << "Erased the top instruction & Times in the pipeline" << std::endl;
            pipeline.erase(pipeline.begin());
            Times.erase(Times.begin());
            NumOfStalls++; 
        }
        // std::cout << "After Stall count: " << NumOfStalls << std::endl;
        // std::cout << "Going into another for loop for Pipeline.size() : " << pipeline.size() << " times" << std::endl;
        int pipeSize = this->pipeline.size();
        for(int i =0;i<pipeSize;i++)
        {
            // std::cout<<"Entered the for loop: (Pipeline.size: )"<<pipeline.size()<<std::endl;
            if(pipeline[i].second==0 && (pipeline[i].first.second==3||pipeline[i].first.second==4))
            {
                // std::cout << "Times == 0 & Stage == 3|4" << std::endl;
                pipeline[i].first.second++;
                // std::cout<<"Stage increased by one"<<std::endl;
                pipeline[i].second = Times[i][pipeline[i].first.second-1];
                // std::cout << "Input form Times for the Latency of the instructions " << std::endl;
            }
            else if(pipeline[i].second==0 && (pipeline[i].first.second==1||pipeline[i].first.second==2) && stall)
            {
            //    std::cout << "Times == 0 & Stage: 1|2 & Stall == true" << std::endl;
            }
            else if(pipeline[i].second==0 && (pipeline[i].first.second==1||pipeline[i].first.second==2) && !stall && !isRAWHarzad)
            {
                // std::cout << "Times == 0 & Stage: 1|2 & No Stall & No Hazard" << std::endl;
                pipeline[i].first.second++;
                // std::cout<<"Stage increased by one; Times.size(): " << Times.size()<<std::endl;
                pipeline[i].second = Times[i][pipeline[i].first.second-1];
                // std::cout << "Input appropriate latency" << std::endl;
            }
            else if(isRAWHarzad && pipeline[i].first.second==2)
            {
                // std::cout<<"There is a Raw Hazard & Stage == 2"<<std::endl;
                int k1 =0;
                int n1 = pipeline.size();
                auto it = pipeline.begin();
                for(int q = 0;q<n1;q++)
                {
                    if(pipeline[q].first.second==2)
                    {
                        break;
                    }
                    k1++;
                    it++;
                }
                // std::cout << "Found appropriate position: " << std::endl;
                pipeline.insert(it,{{-1,-1},-1});
                Times.insert(Times.begin()+k1,{1,1,1,1,1});
                // std::cout << "Added stall into the pipeline, with proper times, break from loop" << std::endl;
                break;
            }
        }
        if(stall)
        {
            // std::cout << "Stall is true: increased the count " << std::endl;
            NumOfStalls++;
        }
        else
        {
            // std::cout << "No Stall" << std::endl;
            if(BranchTaken)
            {
                // std::cout << "Branch Taken is true" << std::endl;
                if(isRAWHarzad)
                {
                    // std::cout << "Hazard is true" << std::endl;
                    if(pipeline.back().first.second ==1)
                    {
                        // std::cout << "Last instruction back: pop out if it Stage = 1" << std::endl;
                        pipeline.pop_back();
                        Times.pop_back();
                    }
                    if(pipeline.back().first.second ==2)
                    {
                        // std::cout << "Last instruction back: pop out if it Stage = 2" << std::endl;
                        pipeline.pop_back();
                        Times.pop_back();
                    }
                    // std::cout << "We have flushed out the last instructions" << std::endl;
                    isRAWHarzad = false;
                    // std::cout << "Raw Hazard is made false" << std::endl;
                }
                else
                {
                    // std::cout << "Hazard is false "<< std::endl;
                    if(pipeline.back().first.second ==2)
                    {
                        // std::cout << "Stage == 2 : Removed" <<std::endl;
                        pipeline.pop_back();
                        Times.pop_back();
                    }
                    if(pipeline.back().first.second ==3)
                    {
                        // std::cout << "Stage == 3 : Removed" <<std::endl;
                        pipeline.pop_back();
                        Times.pop_back();
                    }
                    if(!inUseRegisters.empty())
                    {
                        inUseRegisters.pop_back();
                        // std::cout << "InUsed Register is poped back" <<std::endl;
                    }
                }
                // std::cout << "Pushed two Stalls" << std::endl;
                pipeline.push_back({{-1,-1},-1});
                pipeline.push_back({{-1,-1},-1});
                Times.push_back({1,1,1,1,1});
                Times.push_back({1,1,1,1,1});
                // std::cout << "Setting pc: " << programCounter <<" to nextPC: " << nxtPC << std::endl; 
                programCounter = nxtPC;
                //pipeline.push_back({{programCounter,1},1});
                // std::cout << "BranchTaken is made false" << std::endl;
                BranchTaken = false;
                //NumOfStalls+=2;
            }
            else
            {
                // std::cout << "BranchTaken is false " << std::endl;
                if(!isRAWHarzad && programCounter<=insType.size() )
                {
                    programCounter++;
                    // std::cout << "No Hazard, PC <= total" << std::endl;
                }// pipeline.push_back({{programCounter,1},1});
            }
        }
        // std::cout<<"Stall Count: "<<NumOfStalls<<std::endl;
        // std::cout << "After Making Changes: " << std::endl;
        // std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
        // for(auto x: pipeline)
        // {
        //     std::cout << "PC: " << x.first.first <<" Stage: " << x.first.second << "Times: " << x.second << std::endl;
        // }
        // std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
        // printRegisters();
        // std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
        // for(int i=0;i<32;i++)
        // {
        //     std::cout<< "|" << tempregisters[i]<<" ";
        // }
        // std::cout << "|";
        // std::cout<<std::endl;
        // std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
        return 1;
    }

    // void loadWord(int address, std::vector<Token> LineTokens, int8_t memory[])
    // {
    //     int value = (int)memory[address]+(256*(int)memory[address+1])+(65536*(int)memory[address+2])+(16777216*(int)memory[address+3]);
    //     registers[std::stoi(LineTokens[1].Name.substr(1,LineTokens[1].Name.length()-1))] = value;
    // }

    // void storeWord(int address, std::vector<Token> LineTokens, int8_t memory[])
    // {
    //     uint8_t *;
    //     po = (uint8_t*)&registers[std::stoi(LineTokens[1].Name.substr(1,LineTokens[1].Name.length()-1))];
    //     for(int i =0;i<4;i++)
    //     {
    //         memory[address+i] = *(po+i);
    //     }
    // }
    
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
        std::string instructType;
        if(pc<=insType.size())  instructType = insType[pc];
        // std::cout<<instructType<<"*********"<<pc<<std::endl;
        return instructType;
    }
    std::vector<std::string> instructionDecode(int pc , std::string instructType)
    {
        bool isLW = false;
        std::vector<std::string> result;
        std::pair<int,std::pair<int,int>>toBeUsedRegisters;
        // std::cout<<instructType<<"------789---------"<<pc<<std::endl;
        if(instructType =="Rtype")
        {
            result.push_back((rtype[pc].opcode));
            result.push_back(std::to_string(rtype[pc].dest));
            result.push_back(std::to_string(rtype[pc].src1));
            result.push_back(std::to_string(rtype[pc].src2));
            toBeUsedRegisters = {rtype[pc].dest,{rtype[pc].src1,rtype[pc].src2}};
        }
        else if(instructType=="Itype")
        {
            result.push_back((itype[pc].opcode));
            result.push_back(std::to_string(itype[pc].dest));
            result.push_back(std::to_string(itype[pc].src1));
            result.push_back(std::to_string(itype[pc].immed));
            toBeUsedRegisters = {itype[pc].dest,{itype[pc].src1,-1}};
            if(result[0]=="lw")
            {
                // std::cout<< " ISLW "<<std::endl;
                isLW = true;
            }
        }
         else if(instructType=="Stype")
        {
            result.push_back((stype[pc].opcode));
            result.push_back(std::to_string(stype[pc].src1));
            result.push_back(std::to_string(stype[pc].immed));
            result.push_back(std::to_string(stype[pc].dest));
            toBeUsedRegisters = {-1,{stype[pc].src1,stype[pc].dest}};
        }
         else if(instructType=="SBtype")
        {
            // std::cout << "*********************************************" << std::endl;
            // std::map<int, SBType>::iterator it = sbtype.begin();
            // while(it != sbtype.end())
            // {
            //     std::cout << "key: " << it->first << "value: " << it->second.opcode << " " << it->second.src1 << " " << it->second.src2 << std::endl;
            //     it++;
            // }
            // std::cout << "*********************************************" << pc << std::endl;
            result.push_back((sbtype[pc].opcode));
            result.push_back(std::to_string(sbtype[pc].src1));
            result.push_back(std::to_string(sbtype[pc].src2));
            result.push_back((sbtype[pc].label));
            toBeUsedRegisters = {-1,{sbtype[pc].src1,sbtype[pc].src2}};
        }
        else if(instructType=="Utype")
        {
            result.push_back((utype[pc].opcode));
            result.push_back(std::to_string(utype[pc].dest));
            result.push_back(std::to_string(utype[pc].immed));
            toBeUsedRegisters = {utype[pc].dest,{-1,-1}};
        }
        else if(instructType=="UJ1type")
        {
            result.push_back((ujtype[pc].opcode));
            result.push_back(std::to_string(ujtype[pc].dest));
            result.push_back(std::to_string(ujtype[pc].immed));
        }
        for(std::pair<int,std::pair<int,int>>x : inUseRegisters)
        {
            if(x.first== toBeUsedRegisters.second.first && x.first!= -1)
            {
                isRAWHarzad = true;
                break;
            }
            if(x.first== toBeUsedRegisters.second.second && x.first!= -1)
            {
                isRAWHarzad = true;
                break;
            }
        }
        if(!isRAWHarzad && !this->dataForward)
        {
            inUseRegisters.push_back(toBeUsedRegisters);
        }
        if(!isRAWHarzad && this->dataForward && isLW)
        {
            inUseRegisters.push_back(toBeUsedRegisters);
        }
        return result;
    }

    std::vector<std::string> Execute(int &pc, std::vector<std::string> IDResults){
        std::vector<std::string> exResults;
        // std::cout<<"i27 "<<IDResults.size()<<std::endl;
        if(IDResults.at(0) == "add")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] + registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] + tempregisters[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "sub")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] - registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] - tempregisters[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "and")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] & registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] & tempregisters[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "or")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] | registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] | tempregisters[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "xor")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] ^ registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] ^ tempregisters[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "sll")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] << registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] << tempregisters[std::stoi(IDResults.at(3))];
            }
        }
        else if(IDResults.at(0) == "srl")
        {
            exResults.push_back(IDResults.at(0)); //opcode
            exResults.push_back(IDResults.at(1)); //destination
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] >> registers[std::stoi(IDResults.at(3))])); // value
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] >> tempregisters[std::stoi(IDResults.at(3))];
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
                    tempregisters[std::stoi(IDResults.at(1))] = 1;
                }

            }
            else{
                exResults.push_back(std::to_string(0)); // value
                if(this->dataForward)
                {
                    tempregisters[std::stoi(IDResults.at(1))] = 0;
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
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "andi")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] & std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] & std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "ori")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] | std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] | std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "xori")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] ^ std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] ^ std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "slli")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] << std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] << std::stoi(IDResults.at(3));
            }
        }
        else if(IDResults.at(0) == "srli")
        {
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] >> std::stoi(IDResults.at(3))));
            if(this->dataForward)
            {
                tempregisters[std::stoi(IDResults.at(1))] = tempregisters[std::stoi(IDResults.at(2))] >> std::stoi(IDResults.at(3));
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
                    tempregisters[std::stoi(IDResults.at(1))] = 1;
                }
            }
            else
            {
                exResults.push_back("0");
                if(this->dataForward)
                {
                    tempregisters[std::stoi(IDResults.at(1))] = 0;
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
                tempregisters[std::stoi(IDResults.at(1))] = pc+1;
            }
            pc = registers[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3));
            return exResults; 
        }
        else if(IDResults.at(0) == "lw")
        {
            if(!dataForward)
            {
                exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3))));
            }
            else
            {
                exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(tempregisters[std::stoi(IDResults.at(2))] + std::stoi(IDResults.at(3))));

            }
            
        }
        else if(IDResults.at(0) == "sw")
        {
            if(!dataForward)
            {
                 exResults.push_back(IDResults.at(0));  // opcode
            exResults.push_back(std::to_string(registers[std::stoi(IDResults.at(3))] + std::stoi(IDResults.at(2)))); // destination
            exResults.push_back((IDResults.at(1)));  // value
            }
            else
            {
            exResults.push_back(IDResults.at(0));  // opcode
            exResults.push_back(std::to_string(tempregisters[std::stoi(IDResults.at(3))] + std::stoi(IDResults.at(2)))); // destination
            exResults.push_back((IDResults.at(1)));  // value
            }
        }
        else if(IDResults.at(0) == "bne")
        {
            if(this->dataForward)
            {
                if(tempregisters[stoi(IDResults.at(1))] != tempregisters[stoi(IDResults.at(2))])
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
            return exResults;
            
        }
        else if(IDResults.at(0) == "beq")
        {
            if(this->dataForward)
            {
                if(tempregisters[stoi(IDResults.at(1))] == tempregisters[stoi(IDResults.at(2))])
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
            if(this->dataForward)
            {
                if(tempregisters[stoi(IDResults.at(1))] >= tempregisters[stoi(IDResults.at(2))])
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
            if(this->dataForward)
            {
                if(tempregisters[stoi(IDResults.at(1))] < tempregisters[stoi(IDResults.at(2))])
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
            // std::cout<<"i24 "<<exResults.size()<<std::endl;
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(IDResults.at(2));
        }
        else if(IDResults.at(0) == "jal")
        {
            // std::cout<<"bupdated --------------------------------"<<pc<<std::endl;
            exResults.push_back(IDResults.at(0));
            exResults.push_back(IDResults.at(1));
            exResults.push_back(std::to_string(pc+1));
            pc = std::stoi(IDResults.at(2));
            // std::cout<<"updated --------------------------------"<<pc<<std::endl;
            return exResults;
        }
        pc++;
        return exResults;
    }


    std::pair<std::string, std::string> MEM(int8_t memory[],std::vector<std::string> exResults)
    {
        std::pair<std::string, std::string> memResults;
        // std::cout<<exResults[0]<<"-------------------------"<<exResults[1]<<" "<<exResults[2]<<std::endl;
        if(exResults.at(0)=="lw")
        {
            int address = std::stoi(exResults.at(2));
            int value = (int)memory[address]+(256*(int)memory[address+1])+(65536*(int)memory[address+2])+(16777216*(int)memory[address+3]);
            memResults.first = exResults.at(1);
            memResults.second = std::to_string(value);
            if(this->dataForward)
            {
                tempregisters[std::stoi(memResults.first)]=value;
            }
            if(this->dataForward && !inUseRegisters.empty()) inUseRegisters.erase(inUseRegisters.begin());
        }
        else if(exResults.at(0) == "sw")
        {
            // std::cout << "-------------------------------**************************-------------------" << std::endl;
            uint8_t *po;
            if(this->dataForward)
            {
                po = (uint8_t*)&tempregisters[std::stoi(exResults.at(2))];
            }
            else
            {
                po = (uint8_t*)&registers[std::stoi(exResults.at(2))];
            }
            int address = std::stoi(exResults.at(1));
            for(int i =0;i<4;i++)
            {
                memory[address+i] = *(po+i);
                // std::cout << "Address: " << address+i << "Value: " << (int)*(po+i) << std::endl;  
            }
            memResults.first = "-1";
            memResults.second = "-1";
            //  std::cout<<" I came1 "<<std::endl;
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
        if(!dataForward)
        {
            if(memResults.first != "-1" )
            {
                // std::cout<<"i26 "<<memResults.first<<" "<<memResults.second<<std::endl;
                registers[std::stoi(memResults.first)] = std::stoi(memResults.second);
            }
            registers[0]=0;
            if(!inUseRegisters.empty()) inUseRegisters.erase(inUseRegisters.begin());
            isRAWHarzad = false;
        }
        else
        {
            if(memResults.first != "-1" )
            {
                // std::cout<<"i26 "<<memResults.first<<" "<<memResults.second<<std::endl;
                registers[std::stoi(memResults.first)] =tempregisters[std::stoi(memResults.first)];
            }
            registers[0]=0;
            isRAWHarzad = false;
        }
        
    }

    bool PredictBranch(int pc)
    {
        return false;
    }
   
};