#include <iostream>
#include <map>
#include <vector>
#include "instruction.hpp"
#include "ID.cpp"
 class parser
 {
    std::map <std::string,std::vector<std::string>> instrMap;
    parser()
    {
        instrMap["Rtype"] = {"add","sub","or","and","xor","slt","srl","sll"};
        instrMap["Itype"] = {"addi","subi","ori","andi","xori","slti","srli","slli","jalr"};
        instrMap["Stype"] = {"sw"};
        instrMap["SBtype"] = {"beq","bne","blt","bge"};
        instrMap["Utype"] = {"auipc"};
        instrMap["UJtype"] = {"jal"};
    }
    void parse(std::vector<std::string> Program,std::map<int,std::string> &insType,std::map<int,RType> &rtype,std::map<int,IType> &itype,std::map<int,SType> &stype,std::map<int,SBType> &sbtype,std::map<int,UType> &utype,std::map<int,UJType> &ujtype)
    {
        for(int i =0;i<Program.size();i++)
        {
            std::vector<Token> lineTokens = Tokenizer(Program.at(i),i+1);
        }
    }
 };