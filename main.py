from Parser import Parser
from Processor import Processor
import re

filename1 = input("Enter the name of file1: ")
filename2 = input("Enter the name of file2: ")

while(True):
    
    if((filename1 == "Program1.s") or (filename1 == "Program2.s") or (filename1 == "Program3.s")):
        if((filename2 == "Program1.s") or (filename2 == "Program2.s") or (filename2 == "Program3.s")):
            break
        else:
            print("The name of file2 is not found")
            filename2 = input("Enter the name of file2: ")
    else:
        print("The name of file1 is not found")
        filename1 = input("Enter the name of file1: ")

file1 = filename1
file2 = filename2

user_dataForward = True if (input("DataForwarding(Y/N): ").upper() == "Y") else False

dataForward = user_dataForward

instructionsProgram1 = []
instructionsProgram2 = []

instruct = ["add","sub","or","and","xor","slt","srl","sll","addi","subi","ori","andi","xori","slti","srli","slli","jalr","lw","sw","beq","bne","blt","bge","auipc","jal"]

latencies={}

print("All the latencies of the instructions are of 1 cycle")
if(input("Do you want to change the latencies of the instructions(Y/N):").upper() == "Y"):
    for x in instruct:
        print(f"Latency for {x} instruction (Enter 5 positive numbers): ", end = " ")
        latencies[x] = {int(input()),int(input()),int(input()),int(input()),int(input())}
else:
    for x in instruct:
        latencies[x]=[1,1,1,1,1]


with open("processor_state.txt", "w") as file:
    file.close()

user_cacheSize = int(input("Enter the cacheSize: "))
user_blockSize = int(input("Enter the blockSize: "))
user_associativity = int(input("Enter the associativity: "))
user_accesslatencyofCache = int(input("Enter the access latency of the cache: "))

processor = Processor(user_cacheSize, user_blockSize, user_associativity, user_accesslatencyofCache)
with open(file1,'r') as file:
    for line in file:
        if(line.strip()!=''):
            line = line.split('#',1)[0]
            line = line.strip()
            instructionsProgram1.append(line)

p1 = Parser()

pc1 = p1.parse(memory=(processor.memory1),instruction=instructionsProgram1)


with open(file2,'r') as file:
    for line in file:
        if(line.strip()!=''):
            line = line.split('#',1)[0]
            line = line.strip()
            instructionsProgram2.append(line)

p2 = Parser()

pc2 = p2.parse(memory=(processor.memory2),instruction=instructionsProgram2)

processor.run(latencies,end_pc1=pc1,end_pc2=pc2,dataForward=dataForward)


for x in range(1024,2043,4):
    print(processor.memory1[x]," ",processor.memory1[x+1]," ",processor.memory1[x+2]," ",processor.memory1[x+3],"\n")

print(f"Clocks taken for Core1: {processor.clock1}")
print(f"Number of instructions executed: {processor.Core1.instructionExecuted}")
print(f"Number of Cache access: {processor.Core1.cacheAccess}")
print(f"Number of cache misses: {processor.Core1.cacheMiss}")
print(f"Miss Rate: {processor.Core1.cacheMiss/processor.Core1.cacheAccess}")
print(f"Number of Stalls : {processor.Core1.stalls}")
print(f"IPC: {processor.Core1.instructionExecuted/processor.clock1}")

for x in range(1024,2043,4):
    print(processor.memory2[x]," ",processor.memory2[x+1]," ",processor.memory2[x+2]," ",processor.memory2[x+3],"\n")

print(f"Clocks taken for Core2: {processor.clock2}")
print(f"Number of instructions executed: {processor.Core2.instructionExecuted}")
print(f"Number of Cache access: {processor.Core2.cacheAccess}")
print(f"Number of cache misses: {processor.Core2.cacheMiss}")
print(f"Miss Rate: {processor.Core2.cacheMiss/processor.Core2.cacheAccess}")
print(f"Number of Stalls : {processor.Core2.stalls}")
print(f"IPC: {processor.Core2.instructionExecuted/processor.clock2}")
