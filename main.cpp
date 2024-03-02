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
    for(int i =0;i<1;i++)
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
        for(std:: string str: ins)
        {
            timeMap[str] = {1,1,1,1,1};
        }
        parser p;
        p.parse(Processor.memory1,Program,
        Processor.cores[0].insType,
        Processor.cores[0].rtype,
        Processor.cores[0].itype,
        Processor.cores[0].stype,
        Processor.cores[0].sbtype,
        Processor.cores[0].utype,
        Processor.cores[0].ujtype,
        Processor.cores[0].dataLabels,
        Processor.cores[0].labels,
        timeMap);
    }
    // Processor.run();
    // Processor.cores[0].printDataLabels();
    // Processor.cores[1].printDataLabels();
    // std::cout << "Register Contents: " << std::endl;
    // Processor.cores[0].printRegisters();
    // Processor.cores[1].printRegisters();
    std::cout << "Memory Contents:" << std::endl;
    Processor.printMemory();
}