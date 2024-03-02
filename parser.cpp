#include <iostream>
#include <map>
#include <vector>
#include "instruction.hpp"
#include "id.hpp"
class parser
{
    std::map<std::string, std::string> instrMap;
    public :
    parser()
    {
        instrMap["add"] = "Rtype";
        instrMap["sub"] = "Rtype";
        instrMap["or"] = "Rtype";
        instrMap["and"] = "Rtype";
        instrMap["xor"] = "Rtype";
        instrMap["slt"] = "Rtype";
        instrMap["srl"] = "Rtype";
        instrMap["sll"] = "Rtype";
        instrMap["addi"] = "Itype";
        instrMap["subi"] = "Itype";
        instrMap["ori"] = "Itype";
        instrMap["andi"] = "Itype";
        instrMap["xori"] = "Itype";
        instrMap["lw"] = "Itype";
        instrMap["slti"] = "Itype";
        instrMap["srli"] = "Itype";
        instrMap["slli"] = "Itype";
        instrMap["jalr"] = "Itype";
        instrMap["sw"] = "Stype";
        instrMap["beq"] = "SBtype";
        instrMap["bne"] = "SBtype";
        instrMap["blt"] = "SBtype";
        instrMap["bge"] = "SBtype";
        instrMap["auipc"] = "Utype";
        instrMap["jal"] = "UJ1type";
        // instrMap["Rtype"] = {"add","sub","or","and","xor","slt","srl","sll"};
        // instrMap["Itype"] = {"addi","subi","ori","andi","xori","slti","srli","slli","jalr","lw"};
        // instrMap["Stype"] = {"sw"};
        // instrMap["SBtype"] = {"beq","bne","blt","bge"};
        // instrMap["Utype"] = {"auipc"};
        // instrMap["UJtype"] = {"jal"};
    }
    void parse(int8_t memory[], std::vector<std::string> &Program, std::map<int, std::string> &insType, std::map<int, RType> &rtype, std::map<int, IType> &itype, std::map<int, SType> &stype, std::map<int, SBType> &sbtype, std::map<int, UType> &utype, std::map<int, UJ1Type> &ujtype, std::map<std::string, DataToken> &dataLabels, std::map<std::string, int> &labels, std::map<std::string, std::vector<int>> timeMap)
    {
        std::map<int, std::string> jLabels;
        this->getData(memory, Program, dataLabels);
        std::vector<std::vector<Token>> ProgramTokens = this->tokenization(Program);
        int offset = 0;
        for (int i = 1; i <= ProgramTokens.size(); i++)
        {
        //std::cout<<i<<" klj"<<" "<<ProgramTokens[i-1].size()<<ProgramTokens[i-1].at(0).Name<<std::endl;
            std::vector<Token> line = ProgramTokens[i - 1];
            if (line[0].Name[line[0].Name.length() - 1] == ':' && line.size() == 1)
            {
                std::string label = line[0].Name.substr(0, line[0].Name.length() - 1);
                labels.insert({label, i + offset});
                offset--;
                continue;
            }
            else if (line[0].Name[line[0].Name.length() - 1] == ':' && line.size() > 1)
            {
                std::string label = line[0].Name.substr(0, line[0].Name.length() - 1);
                labels.insert({label, i + offset});
                line.erase(line.begin());
            }
            if (instrMap[line[0].Name] == "Rtype")
            {
                RType instruction(line[0].Name, timeMap[line[0].Name][0],
                                  timeMap[line[0].Name][1],
                                  timeMap[line[0].Name][2],
                                  timeMap[line[0].Name][3],
                                  timeMap[line[0].Name][4],
                                  std::stoi(line[2].Name.substr(1, line[2].Name.length() - 1)),
                                  std::stoi(line[3].Name.substr(1, line[3].Name.length() - 1)),
                                  std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)));

                rtype.insert({i + offset, instruction});
                insType.insert({i + offset, "Rtype"});
            }
            else if (line[0].Name == "lw" && line.size() == 3)
            {
                UType instruction("auipc", timeMap[line[0].Name][0],
                                  timeMap[line[0].Name][1],
                                  timeMap[line[0].Name][2],
                                  timeMap[line[0].Name][3],
                                  timeMap[line[0].Name][4],
                                  std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)),
                                  0);
                utype.insert({i + offset, instruction});
                insType.insert({i + offset, "Utype"});
                
                
                IType instruction1(line[0].Name, timeMap[line[0].Name][0],
                                  timeMap[line[0].Name][1],
                                  timeMap[line[0].Name][2],
                                  timeMap[line[0].Name][3],
                                  timeMap[line[0].Name][4],
                                  std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)),
                                  labels[line[2].Name],
                                  std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)));

                itype.insert({i + offset+1, instruction1});
                insType.insert({i + offset+1, "Itype"});
            }

            else if (instrMap[line[0].Name] == "Itype")
            {
            //    std::cout<<"hjk "<<std::endl;
                if(line[0].Name=="lw")
                {
                    IType instruction(line[0].Name, timeMap[line[0].Name][0],
                                  timeMap[line[0].Name][1],
                                  timeMap[line[0].Name][2],
                                  timeMap[line[0].Name][3],
                                  timeMap[line[0].Name][4],
                                  std::stoi(line[3].Name.substr(1, line[3].Name.length() - 1)),
                                  std::stoi(line[2].Name),
                                  std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)));

                itype.insert({i + offset, instruction});
                insType.insert({i + offset, "Itype"});
                continue;
                }
                IType instruction(line[0].Name, timeMap[line[0].Name][0],
                                  timeMap[line[0].Name][1],
                                  timeMap[line[0].Name][2],
                                  timeMap[line[0].Name][3],
                                  timeMap[line[0].Name][4],
                                  std::stoi(line[2].Name.substr(1, line[2].Name.length() - 1)),
                                  std::stoi(line[3].Name),
                                  std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)));

                itype.insert({i + offset, instruction});
                insType.insert({i + offset, "Itype"});
            }
            else if (instrMap[line[0].Name] == "Stype")
            {
                SType instruction(line[0].Name, timeMap[line[0].Name][0],
                                  timeMap[line[0].Name][1],
                                  timeMap[line[0].Name][2],
                                  timeMap[line[0].Name][3],
                                  timeMap[line[0].Name][4],
                                  std::stoi(line[3].Name.substr(1, line[3].Name.length() - 1)),
                                  std::stoi(line[2].Name),
                                  std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)));

                stype.insert({i + offset, instruction});
                insType.insert({i + offset, "Stype"});
            }
            else if (instrMap[line[0].Name] == "SBtype")
            {
                SBType instruction(line[0].Name, timeMap[line[0].Name][0],
                                   timeMap[line[0].Name][1],
                                   timeMap[line[0].Name][2],
                                   timeMap[line[0].Name][3],
                                   timeMap[line[0].Name][4],
                                   std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)),
                                   std::stoi(line[2].Name.substr(1, line[2].Name.length() - 1)),
                                   line[3].Name);

                sbtype.insert({i + offset, instruction});
                insType.insert({i + offset, "SBtype"});
            }
            else if (instrMap[line[0].Name] == "Utype")
            {
                UType instruction(line[0].Name, timeMap[line[0].Name][0],
                                  timeMap[line[0].Name][1],
                                  timeMap[line[0].Name][2],
                                  timeMap[line[0].Name][3],
                                  timeMap[line[0].Name][4],
                                  std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)),
                                  std::stoi(line[2].Name));

                utype.insert({i + offset, instruction});
                insType.insert({i + offset, "Utype"});
            }
            else if (instrMap[line[0].Name] == "UJ1type")
            {
                UJ1Type instruction(line[0].Name, timeMap[line[0].Name][0],
                                   timeMap[line[0].Name][1],
                                   timeMap[line[0].Name][2],
                                   timeMap[line[0].Name][3],
                                   timeMap[line[0].Name][4],
                                   std::stoi(line[1].Name.substr(1, line[1].Name.length() - 1)),
                                   -1);
                jLabels.insert({i + offset, line[1].Name});
                ujtype.insert({i + offset, instruction});
                insType.insert({i + offset, "UJ1type"});
            }
            else if (line[0].Name == "j")
            {
                //std::cout<<"s is "<<line[0].Name<<" "<<line[1].Name<<std::endl;
                UJ1Type instruction("jal", timeMap["jal"][0],
                                   timeMap["jal"][1],
                                   timeMap["jal"][2],
                                   timeMap["jal"][3],
                                   timeMap["jal"][4],
                                   0,
                                   -1);
                                  // std::cout<<"s1 is "<<line.size()<<std::endl;
                jLabels.insert({i + offset, line[1].Name});
                ujtype.insert({i + offset, instruction});
                insType.insert({i + offset, "UJ1type"});
            }
        }
        for (auto it = jLabels.begin(); it != jLabels.end(); it++)
        {
            int immed = labels[it->second];
            ujtype[it->first].immed = immed;
        }
    }
    std::vector<std::vector<Token>> tokenization(std::vector<std::string> Program)
    {
        int lineNumber = 1;
        std::vector<std::vector<Token>> ProgramTokens;
        for (int i = 1; i <= Program.size(); i++)
        {
            std::vector<Token> line = Tokenizer(Program[i - 1], lineNumber);
            if (line.size() != 0)
            {
                ProgramTokens.push_back(line);
                lineNumber++;
            }
        }
        return ProgramTokens;
    }
    void getData(int8_t memory[], std::vector<std::string> &Program, std::map<std::string, DataToken> &dataLabels)
    {
        if (isData(Program.at(0)))
        {
            int i = 1, memPointer = 256;
            while (i < Program.size() && !isText(Program.at(i)))
            {
                storeData(memory, Program.at(i), dataLabels, memPointer);
                i++;
            }
            i++;
            Program.erase(Program.begin());
        }
        if (isText(Program.at(0)))
        {
            Program.erase(Program.begin());
        }
    }
};

// int main()
// {
//     std::cout << "HI";
// }
