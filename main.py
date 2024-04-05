from Parser import Parser
from Processor import Processor
import re

file1 = 'Program1.s'
file2 = 'Program2.s'

dataForward = False

instructionsProgram1 = []
instructionsProgram2 = []

instruct = ["add","sub","or","and","xor","slt","srl","sll","addi","subi","ori","andi","xori","slti","srli","slli","jalr","lw","sw","beq","bne","blt","bge","auipc","jal"]

latencies={}

for x in instruct:
    latencies[x]=[1,1,1,1,1]

with open("processor_state.txt", "w") as file:
    file.close()

processor = Processor(512,8,8)
with open(file1,'r') as file:
    for line in file:
        if(line.strip()!=''):
            line = line.split('#',1)[0]
            line = line.strip()
            instructionsProgram1.append(line)
print(instructionsProgram1)
print("--------------------------------")
p = Parser()

pc1 = p.parse(memory=(processor.memory1),instruction=instructionsProgram1)

print(processor.memory1[1024:])

for x in range(0,pc1+1,4):
    print(processor.memory1[x]," ",processor.memory1[x+1]," ",processor.memory1[x+2]," ",processor.memory1[x+3],"\n")


print("-------------------------------------------------------------------------------------------------------")


# x1 = int(input())
# while(x1!=-1):
#     processor.run1(latencies,end_pc1=pc1,end_pc2=pc1)
#     x1 = int(input())
processor.run(latencies,end_pc1=pc1,end_pc2=pc1,dataForward=dataForward)


for x in range(1023,2043,4):
    print(processor.memory1[x]," ",processor.memory1[x+1]," ",processor.memory1[x+2]," ",processor.memory1[x+3],"\n")

print(processor.clock1)
print(processor.Core1.instructionExecuted)
print(processor.Core1.cacheAccess)
print(processor.Core1.cacheMiss)


# with open(file2,'r') as file:
#     for line in file:
#         if(line.strip()!=''):
#             instructionsProgram1.append(re.split(r'[ ,](?![^#]*#)', line))

#p.parse(processor.memory2,instructionsProgram2)

# c =0
# for x in processor.memory1:
#     print(str(x)+'{'+str(c)+'}',end="|")
#     c +=1

