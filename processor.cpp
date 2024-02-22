#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "./core.cpp"

using namespace std;

class processor{
    public:
    int clock;
    Core cores[2] = {Core(),Core()};
    int8_t memory1[2048] = {0};
    int8_t memory2[2048] = {0};
    
    void run()
    {
        cores[0].getData(memory1);
        cores[1].getData(memory2);
        cores[0].getLabels();
        cores[1].getLabels();
        //cout<<cores[0].Program.size() <<"  JJJ  "<<clock << cores[1].Program.size()<<endl;
        int t =1,t1=1;
        while(t1|t!=0)
        {
            // cout<<cores[0].Program.size() <<"  JJJ  "<<clock << cores[1].Program.size()<<endl;
            // cout<<"clock is "<<clock<<endl;

                //cout<<"clock is1 "<<clock<<endl;
                t=cores[0].executeLine(memory1);
                t1=cores[1].executeLine(memory2);
                //cout<<"clock is2 "<<clock<<endl;
                
            
            clock++;
        }
    }
    void print()
    {
        for(int i =256;i<400;i++)
        {
            cout<<(int)memory1[i]<<" ";
        }
        cout<<endl;
    }
};