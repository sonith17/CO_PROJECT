from Parser import Parser
from Processor import Processor
import re

file1 = 'Program1.s'
file2 = 'Program2.s'

dataForward = True

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
p1 = Parser()

pc1 = p1.parse(memory=(processor.memory1),instruction=instructionsProgram1)

print(processor.memory1[1024:])

for x in range(0,pc1+1,4):
    print(processor.memory1[x]," ",processor.memory1[x+1]," ",processor.memory1[x+2]," ",processor.memory1[x+3],"\n")


print("-------------------------------------------------------------------------------------------------------")

with open(file2,'r') as file:
    for line in file:
        if(line.strip()!=''):
            line = line.split('#',1)[0]
            line = line.strip()
            instructionsProgram2.append(line)
print(instructionsProgram2)
print("--------------------------------")
p2 = Parser()

pc2 = p2.parse(memory=(processor.memory2),instruction=instructionsProgram2)

print(processor.memory2[1024:])

for x in range(0,pc2+1,4):
    print(processor.memory2[x]," ",processor.memory2[x+1]," ",processor.memory2[x+2]," ",processor.memory2[x+3],"\n")


print("-------------------------------------------------------------------------------------------------------")


# x1 = int(input())
# while(x1!=-1):
#     processor.run1(latencies,end_pc1=pc1,end_pc2=pc1)
#     x1 = int(input())
processor.run(latencies,end_pc1=pc1,end_pc2=pc2,dataForward=dataForward)


for x in range(1023,2043,4):
    print(processor.memory1[x]," ",processor.memory1[x+1]," ",processor.memory1[x+2]," ",processor.memory1[x+3],"\n")

print(processor.clock1)
print(processor.Core1.instructionExecuted)
print(processor.Core1.cacheAccess)
print(processor.Core1.cacheMiss)

for x in range(1023,2043,4):
    print(processor.memory2[x]," ",processor.memory2[x+1]," ",processor.memory2[x+2]," ",processor.memory2[x+3],"\n")

print(processor.clock2)
print(processor.Core2.instructionExecuted)
print(processor.Core2.cacheAccess)
print(processor.Core2.cacheMiss)



# with open(file2,'r') as file:
#     for line in file:
#         if(line.strip()!=''):
#             instructionsProgram1.append(re.split(r'[ ,](?![^#]*#)', line))

#p.parse(processor.memory2,instructionsProgram2)

# c =0
# for x in processor.memory1:
#     print(str(x)+'{'+str(c)+'}',end="|")
#     c +=1

