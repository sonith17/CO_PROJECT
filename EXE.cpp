#include<iostream>
#include<string>
#include<vector>
#include <map>
#include <unordered_map>
#include "Token.hpp"
#include "id.hpp"
#include <unistd.h>

class EXE
{
private:
    std::map<std::string,int> Register;
public:
    EXE();
    int  execute(std::vector<Token> tokens, int registers[], int &pc,std::map<std::string, int > &labels,std::map<std::string,DataToken>& dataLabels);
};

EXE::EXE()
{
    std::string x = "x";
    for(int i = 0;i<32;i++)
    {
        Register.insert({x+std::to_string(i),i});
    }
}

int EXE:: execute(std::vector<Token> tokens, int registers[], int &pc,std::map<std::string, int > &labels,std::map<std::string,DataToken>& dataLabels)
{
    if(tokens[0].Name=="add")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]+registers[Register[sourc2.Name]];
        pc+=1;
    }
    else if(tokens[0].Name=="sub")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]-registers[Register[sourc2.Name]];
        pc+=1;
    }
    else if(tokens[0].Name=="addi")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token immediate = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]+stoi(immediate.Name);
        pc+=1;
    }
    if(tokens[0].Name=="and")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]&registers[Register[sourc2.Name]];
        pc+=1;
    }
    if(tokens[0].Name=="or")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]|registers[Register[sourc2.Name]];
        pc+=1;
    }
    if(tokens[0].Name=="xor")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]^registers[Register[sourc2.Name]];
        pc+=1;
    }
    else if(tokens[0].Name=="andi")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token immediate = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]&stoi(immediate.Name);
        pc+=1;
    }
     else if(tokens[0].Name=="ori")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token immediate = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]|stoi(immediate.Name);
        pc+=1;
    }
     else if(tokens[0].Name=="xori")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token immediate = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]^stoi(immediate.Name);
        pc+=1;
    }
     else if(tokens[0].Name=="slli")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token immediate = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]<<stoi(immediate.Name);
        pc+=1;
    }
     else if(tokens[0].Name=="srli")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token immediate = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]>>stoi(immediate.Name);
        pc+=1;
    }
    if(tokens[0].Name=="sll")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]<<registers[Register[sourc2.Name]];
        pc+=1;
    }
    if(tokens[0].Name=="srl")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        registers[Register[destination.Name]]= registers[Register[sourc1.Name]]>>registers[Register[sourc2.Name]];
        pc+=1;
    }
    if(tokens[0].Name=="slt")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token sourc2 = tokens[3];
        if(registers[Register[sourc1.Name]]<registers[Register[sourc2.Name]]){
            registers[Register[destination.Name]]= 1;
        }
        else{
             registers[Register[destination.Name]]= 0;
        }
        pc+=1;
    }
    if(tokens[0].Name=="slti")
    {
        Token destination = tokens[1];
        Token sourc1 = tokens[2];
        Token immediate = tokens[3];
        if(registers[Register[sourc1.Name]]<stoi(immediate.Name)){
            registers[Register[destination.Name]]= 1;
        }
        else{
             registers[Register[destination.Name]]= 0;
        }
        pc+=1;
    }
    else if(tokens[0].Name[tokens[0].Name.length()-1]==':' && tokens.size()==1)
    {
        labels.insert({tokens[0].Name.substr(0,tokens[0].Name.length()-1),pc+1});
        pc+=1;
    }
    else if(tokens[0].Name=="bne")
    {
        Token sourc1 = tokens[1];
        Token sourc2 = tokens[2];
        Token destination = tokens[3];
        if(registers[Register[sourc1.Name]]!=registers[Register[sourc2.Name]])
        {
            pc = labels[destination.Name];
        }
        else
        {
            pc+=1;
        }
    }
    else if(tokens[0].Name=="beq")
    {
        Token sourc1 = tokens[1];
        Token sourc2 = tokens[2];
        Token destination = tokens[3];
        if(registers[Register[sourc1.Name]]==registers[Register[sourc2.Name]])
        {
            pc = labels[destination.Name];
        }
        else
        {
            pc+=1;
        }
    }
    else if(tokens[0].Name=="blt")
    {
        Token sourc1 = tokens[1];
        Token sourc2 = tokens[2];
        Token destination = tokens[3];
        if(registers[Register[sourc1.Name]]<registers[Register[sourc2.Name]])
        {
            pc = labels[destination.Name];
        }
        else
        {
            pc+=1;
        }
    }
    else if(tokens[0].Name=="bge")
    {
        Token sourc1 = tokens[1];
        Token sourc2 = tokens[2];
        Token destination = tokens[3];
        if(registers[Register[sourc1.Name]]>=registers[Register[sourc2.Name]])
        {
            pc = labels[destination.Name];
        }
        else
        {
            pc+=1;
        }
    }
    else if(tokens[0].Name=="sw")
    {
        Token sourc1 = tokens[3];
        Token immediate = tokens[2];
        Token destination = tokens[1];
        pc+=1;
        return stoi(immediate.Name)+registers[Register[sourc1.Name]];
        
    }
    else if(tokens[0].Name=="lw" && tokens.size()==4)
    {
        Token destination = tokens[1];
        Token immediate = tokens[2];
        Token sourc1 = tokens[3];
        pc+=1;
        return stoi(immediate.Name)+registers[Register[sourc1.Name]];
        
    }
    else if(tokens[0].Name=="lw" && tokens.size()==3)
    {
        Token destination = tokens[1];
        Token label = tokens[2];
        pc+=1;
        return dataLabels[label.Name].address;
    }
    else if(tokens[0].Name=="j")
    {
        Token destination = tokens[1];
        pc = labels[destination.Name];
    }
    // else if(tokens[0].Name=="jal")
    // {
    //     Token destination = tokens[1];
    //     pc = labels[destination.Name];
    // }
    return -1;
}

