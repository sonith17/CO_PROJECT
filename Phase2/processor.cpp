#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "./core.cpp"


class processor{
    public:
    int clock =0;
    int clock_core1 = 0;
    int clock_core2 = 0;
    Core cores[2] = {Core(),Core()};
    int8_t memory1[2048] = {0};
    int8_t memory2[2048] = {0};
    int t=1,t1=1;
    
    void run()
    {   
        while(t1|t!=0)
        {
            t=cores[0].RunPipeline(memory1);
            t1=cores[1].RunPipeline(memory2);
            clock++;
            if(t == 0 && clock_core1 == 0)
            {
                clock_core1 = clock;
            }
            if(t1 == 0 && clock_core2 == 0)
            {
                clock_core2 = clock;
            }
        }
    }
    void printMemory()
    {
        int count = 0;
        std::cout << "First 2048 bytes" << std::endl;
        std::cout << "------------------------------------------------------------------------------------------------------"<<std::endl;
        for(int i = 0; i < 2048; i++)
        {
            std::cout << "|" << (int)memory1[i] << " ";
            count++;
            if((i+1) % 32 == 0)
            {
                std::cout << "|" << std::endl;
            }
        }
        std::cout << "------------------------------------------------------------------------------------------------------" <<std::endl;
        std::cout << "Second 2048 bytes" << std::endl;
        std::cout << "------------------------------------------------------------------------------------------------------"<<std::endl;
        for(int i = 0; i < 2048; i++)
        {
            std::cout << "|" << (int)memory2[i] << " ";
            count++;
            if((i+1) % 32 == 0)
            {
                std::cout << "|" << std::endl;
            }
        }
        std::cout << "------------------------------------------------------------------------------------------------------" <<std::endl;
    }
};