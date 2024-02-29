#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include<string>

class instruction
{
    public:
    std::string opcode;
    int IF_t;
    int ID_t;
    int EX_t;
    int MEM_t;
    int WB_t;
    instruction(std::string opcode,int IF_t,int ID_t,int EX_t,int MEM_t,int WB_t)
    {
        this->opcode = opcode;
        this->IF_t = IF_t;
        this->ID_t = ID_t;
        this->EX_t = EX_t;
        this->MEM_t = MEM_t;
        this->WB_t = WB_t;
    }


};

class RType : public instruction
{
    public:
    int src1;
    int src2;
    int dest;
    RType(std::string opcode,int IF_t,int ID_t,int EX_t,int MEM_t,int WB_t,int src1,int src2,int dest) : instruction(opcode,IF_t,ID_t,EX_t,MEM_t,WB_t)
    {
        this->src1 = src1;
        this->src2 = src2;
        this->dest = dest;
    }
};

class IType : public instruction
{
    public:
    int src1;
    int immed;
    int dest;
    IType(std::string opcode,int IF_t,int ID_t,int EX_t,int MEM_t,int WB_t,int src1,int immed,int dest) : instruction(opcode,IF_t,ID_t,EX_t,MEM_t,WB_t)
    {
        this->src1 = src1;
        this->immed = immed;
        this->dest = dest;
    }
};

class SType : public instruction
{
    public:
    int src1;
    int immed;
    int dest;
    SType(std::string opcode,int IF_t,int ID_t,int EX_t,int MEM_t,int WB_t,int src1,int immed,int dest) : instruction(opcode,IF_t,ID_t,EX_t,MEM_t,WB_t)
    {
        this->src1 = src1;
        this->immed = immed;
        this->dest = dest;
    }
};

class SBType :  public instruction
{
    public:
    int src1;
    int src2;
    std::string label;
    SBType(std::string opcode,int IF_t,int ID_t,int EX_t,int MEM_t,int WB_t,int src1,int src2,std::string label) : instruction(opcode,IF_t,ID_t,EX_t,MEM_t,WB_t)
    {
        this->src1 = src1;
        this->src2 = src2;
        this->label = label;
    }
};

class UType :  public instruction
{
    public:
    int immed;
    int dest;
    UType(std::string opcode,int IF_t,int ID_t,int EX_t,int MEM_t,int WB_t,int immed,int dest) : instruction(opcode,IF_t,ID_t,EX_t,MEM_t,WB_t)
    {
        this->immed = immed;
        this->dest = dest;
    }
};

class UJType :  public instruction
{
    public:
    int immed;
    int dest;
    UJType(std::string opcode,int IF_t,int ID_t,int EX_t,int MEM_t,int WB_t,int immed,int dest) : instruction(opcode,IF_t,ID_t,EX_t,MEM_t,WB_t)
    {
        this->immed = immed;
        this->dest = dest;
    }
};




#endif