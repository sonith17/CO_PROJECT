#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <map>
#include <algorithm>
#include "./processor.cpp"
#include "./parser.cpp"

// Make x0 hard wire to zero.

using namespace std::filesystem;

int main(int argc, char* argv[])
{
    processor Processor;
    std::vector<std::string> FilePaths;

    std::map<std::string, std::vector<int>> timeMap;
    std::vector<std::string> ins={"add","sub","or","and","xor","slt","srl","sll","addi","subi","ori","andi","xori","slti","srli","slli","jalr","lw",
                                "sw","beq","bne","blt","bge","auipc","jal"};

    std::cout << "Do you want to Manually set the Latency for instructions(YES/NO): " << std::endl;
    std::string user_input;
    std::cin >> user_input;
    transform(user_input.begin(), user_input.end(), user_input.begin(), ::toupper);
    if(user_input == "YES")
    {
        for(std:: string str: ins)
        {
            std::cout << "Enter latency for " << str << " :" ;
            int input;
            std::cin >> input;
            timeMap[str] = {1,1,input,1,1};
        }
    }
    else
    {
        for(std:: string str: ins)
        {
            timeMap[str] = {1,1,1,1,1};
        }
    }
    std::cout << "DataForwarding(Enable/Disable): " << std::endl;
    std::cin >> user_input;
    transform(user_input.begin(), user_input.end(), user_input.begin(), ::toupper);
    if(user_input == "ENABLE")
    {
        Processor.cores[0].dataForward = true;
        Processor.cores[1].dataForward = true;
    }
    else
    {
        Processor.cores[0].dataForward = false;
        Processor.cores[1].dataForward = false;
    }


    for(int i=1;i<argc;i++)
    {
        FilePaths.push_back(argv[i]);
    }
    for(int i =0;i<2;i++)
    {
        path path = FilePaths[i]; 
        if (!std::filesystem::exists(path)) {
            std::cerr << "File not found." << std::endl;
            return 1; 
        }

        std::ifstream inputFile(path);
        if (!inputFile.is_open()) {
            std::cerr << "Unable to open the file." << std::endl;
            return 1; 
        }

        std::string line;
        std::vector<std::string> Program;
        while (std::getline(inputFile, line)) {
            Program.push_back(line);
        }

        parser p;
        if(i==0){
        p.parse(Processor.memory1,Program,
        Processor.cores[i].insType,
        Processor.cores[i].rtype,
        Processor.cores[i].itype,
        Processor.cores[i].stype,
        Processor.cores[i].sbtype,
        Processor.cores[i].utype,
        Processor.cores[i].ujtype,
        Processor.cores[i].dataLabels,
        Processor.cores[i].labels,
        timeMap);
        }
        else
        {
        p.parse(Processor.memory2,Program,
        Processor.cores[i].insType,
        Processor.cores[i].rtype,
        Processor.cores[i].itype,
        Processor.cores[i].stype,
        Processor.cores[i].sbtype,
        Processor.cores[i].utype,
        Processor.cores[i].ujtype,
        Processor.cores[i].dataLabels,
        Processor.cores[i].labels,
        timeMap);
        }
    }
    Processor.run();
    // Processor.cores[0].printDataLabels();
    // Processor.cores[1].printDataLabels();
    // Processor.cores[0].printLabels();
    // Processor.cores[1].printLabels();
    std::cout << "Register Contents: " << std::endl;
    Processor.cores[0].printRegisters();
    Processor.cores[1].printRegisters();
    std::cout << "Memory Contents:" << std::endl;
    Processor.printMemory();

    std::cout << "Instructions per Cycles(Core1): " << ((double)(Processor.cores[0].instructions)) / (double)(Processor.clock_core1-1) << std::endl;
    std::cout << "Instructions , Clocks " << ((double)(Processor.cores[0].instructions)) <<" "<< (double)(Processor.clock_core1-1) << std::endl;
    std::cout << "Instructions per Cycles(Core2): " << ((double)(Processor.cores[1].instructions)) / (double)(Processor.clock_core2-1) << std::endl;
    std::cout << "Instructions , Clocks " << ((double)(Processor.cores[1].instructions)) <<" "<< (double)(Processor.clock_core2-1) << std::endl;
}