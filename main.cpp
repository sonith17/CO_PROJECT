#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include "./processor.cpp"


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

        Processor.cores[i].Program = Program;
    }
    Processor.run();
    // Processor.cores[0].printDataLabels();
    // Processor.cores[1].printDataLabels();
    std::cout << "Register Contents: " << std::endl;
    Processor.cores[0].printRegisters();
    Processor.cores[1].printRegisters();
    std::cout << "Memory Contents:" << std::endl;
    Processor.printMemory();
}