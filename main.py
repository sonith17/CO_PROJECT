from Parser import Parser
from Processor import Processor
import re

file1 = 'Program1.s'
file2 = 'Program2.s'

instructionsProgram1 = []
instructionsProgram2 = []

processor = Processor()
with open(file1,'r') as file:
    for line in file:
        if(line.strip()!=''):
            line = line.split('#',1)[0]
            line = line.strip()
            instructionsProgram1.append(line)
print(instructionsProgram1)
print("--------------------------------")
p = Parser()

p.parse(memory=(processor.memory1),instruction=instructionsProgram1)

print(processor.memory1[1023:])

with open(file2,'r') as file:
    for line in file:
        if(line.strip()!=''):
            instructionsProgram1.append(re.split(r'[ ,](?![^#]*#)', line))

#p.parse(processor.memory2,instructionsProgram2)

# c =0
# for x in processor.memory1:
#     print(str(x)+'{'+str(c)+'}',end="|")
#     c +=1

