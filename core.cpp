#include <iostream>
#include <string>
#include <vector>
#include "./parser.cpp"

using namespace std;

class core{
    public:
    int registers[32]={0};
    int programCounter =0;
    vector<string> Program;
    void executeLine(int memory[])
    {
        if(programCounter>= Program.size())
        {
            return;
        }
        else
        {
            
            programCounter = parse(Program[programCounter],registers,memory,programCounter);
        }
    }

};