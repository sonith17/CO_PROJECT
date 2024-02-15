#include <iostream>
#include <string>
#include <vector>
#include "./core.cpp"

using namespace std;

class processor{
    public:
    int clock;
    core cores[2] = {core(),core()};
    int memory[4096] = {0};
    void run()
    {
        while(clock < cores[0].Program.size() || clock < cores[1].Program.size())
        {
            for(int i =0;i<2;i++)
            {
                cores[i].executeLine(memory);
            }
            clock++;
        }
    }
};