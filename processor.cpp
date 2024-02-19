#include <iostream>
#include <string>
#include <vector>
#include "./core.cpp"

using namespace std;

class processor{
    public:
    int clock;
    Core cores[2] = {Core(),Core()};
    int memory[4096] = {0};
    void run()
    {
        cores[0].getLabels();
        cores[1].getLabels();
        //cout<<cores[0].Program.size() <<"  JJJ  "<<clock << cores[1].Program.size()<<endl;
        int t =1,t1=1;
        while(t1|t!=0)
        {
            // cout<<cores[0].Program.size() <<"  JJJ  "<<clock << cores[1].Program.size()<<endl;
            // cout<<"clock is "<<clock<<endl;

                //cout<<"clock is1 "<<clock<<endl;
                t=cores[0].executeLine(memory);
                t1=cores[1].executeLine(memory);
                //cout<<"clock is2 "<<clock<<endl;
                
            
            clock++;
        }
    }
};