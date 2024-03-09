#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <map>
#include "./processor.cpp"
#include "./parser.cpp"

// Make x0 hard wire to zero.

using namespace std::filesystem;

int main(int argc, char* argv[])
{
    processor Processor;
    std::vector<std::string> FilePaths;
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
        std::map<std::string, std::vector<int>> timeMap;
        std::vector<std::string> ins={"add","sub","or","and","xor","slt","srl","sll","addi","subi","ori","andi","xori","slti","srli","slli","jalr","lw",
                                 "sw","beq","bne","blt","bge","auipc","jal"};
        std::cout << "Latency from user needs to be taken" << std::endl;
        for(std:: string str: ins)
        {
            timeMap[str] = {1,1,2,1,1};
        }
        std::cout << "Latency from the user has been taken" << std::endl;
        parser p;
        std::cout << "The Program has started it's parsing" << std::endl;
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
        std::cout<<"The Program has completed the parsing the instructions"<<std::endl;
    }
    // int x;
    // std::cin>>x;
    // std::cout<<"Enter x\n";
    // while(x!=-1)
    // {
    //     Processor.run();
    //     std::cout<<"Enter x\n";
    //     std::cin>>x;
    // }
    Processor.run();
    Processor.cores[0].printDataLabels();
    Processor.cores[1].printDataLabels();
    Processor.cores[0].printLabels();
    Processor.cores[1].printLabels();
    std::cout << "Register Contents: " << std::endl;
    Processor.cores[0].printRegisters();
    Processor.cores[1].printRegisters();
    std::cout << "Memory Contents:" << std::endl;
    Processor.printMemory();
    std::cout<<Processor.clock<<std::endl;
}