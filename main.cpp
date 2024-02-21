#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include "./processor.cpp"

using namespace std;
using namespace std::filesystem;

int main(int argc, char* argv[])
{
    processor Processor;
    vector<string> FilePaths;
    for(int i=1;i<argc;i++)
    {
        FilePaths.push_back(argv[i]);
    }
    for(int i =0;i<2;i++)
    {
        path path = FilePaths[i]; 
        if (!filesystem::exists(path)) {
            cerr << "File not found." << endl;
            return 1; 
        }

        ifstream inputFile(path);
        if (!inputFile.is_open()) {
            cerr << "Unable to open the file." << endl;
            return 1; 
        }

        string line;
        vector<string> Program;
        while (std::getline(inputFile, line)) {
            Program.push_back(line);
            //cout<<line<<" "<<i<<endl;
        }

        Processor.cores[i].Program = Program;
    }
    cout<<Processor.memory1[12]<<endl;
    Processor.memory1[13]=67;
    Processor.run();
    Processor.cores[0].Print();
    Processor.cores[1].Print();
    cout<<Processor.memory1[12]<<endl;
    Processor.print();



}