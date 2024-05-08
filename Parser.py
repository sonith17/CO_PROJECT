import pandas as pd
import re

def get12_bit(num,bits=12):
    if(num>=0):
        return bin(num)[2:].zfill(bits)
    else:
        s=""
        num = -num
        tf = True
        for i in range(bits):
            if(num&1==0 and tf ):
                s ='0'+s
            elif(num&1==1 and tf):
                s ='1'+s
                tf = False
            elif(num&1==0 and (not tf)):
                s ='1'+s
            elif(num&1==1 and (not tf)):
                s ='0'+s
            num = num>>1
        return s



class Parser:
    labels = {}


    def prerun(self,instructions):
        offset =0
        i = 0
        instruction = []
        while i!=len(instructions):
            ins = re.split(r',\s*|\s+', instructions[i])
            # print('prerun ',ins)
            if len(ins)==1 and ins[0].endswith(':'): #check for labels
                self.labels[ins[0][:-1]] = i+offset
                i +=1
                offset -=1

            elif len(ins)>1 and ins[0].endswith(':'):
                self.labels[ins[0][:-1]] = i+offset
                ins.pop(0)
                instruction.append(ins)
                i+=1
                if ins[0]=='lw' and len(ins)==3:
                    offset+=1
            else:
                instruction.append(ins)
                i+=1
            if ins[0]=='lw' and ins[2][0:1].isalpha():
                    # print('hit is',ins)
                    offset+=1
        
        # print(self.labels)
        #print(instruction)
        return instruction


    def parse(self,memory,instruction):
        df = pd.read_csv('Instruction.csv')
        # print(df)
        insStoreAddress = 0
        pc = 0
        dataLabels = {}
        memPointer = 1024
        if(instruction[0]=='.data'):
            while True:
                instruction.pop(0)
                tokens =  re.split(r',\s*|\s+', instruction[0])
                #print("tokens",tokens)
                if tokens[0][-1]==':' :
                    dataLabels[tokens[0][:-1]] = memPointer
                    tokens.pop(0)
                tokens.pop(0)
                for num in tokens:
                    number =0
                    if num.startswith('0x'):
                        number = int(num[2:],16)
                    else:
                        number = int(num)
                    store = [(number >> (i * 8)) & 0xFF for i in range(4)]
                    for i in range(4):
                        memory[memPointer+i]=store[i]
                    memPointer += 4
                if instruction[1]=='.text':
                    break
        instruction.pop(0)
        instruction.pop(0)
        # print(instruction)
        instruction = self.prerun(instruction)
        while len(instruction)!=0 :
            ins = instruction[0]
            instruction.pop(0) #getting instruction for parse
            df2 = df[df['Instruction']==ins[0]]  #nxt 3 get details for that instruction
            # print(df2) 
            df2 = df2.iloc[0]
            if df2['Type']=='R-Type':
                src1 = str(bin(int(ins[2][1:]) & 31))[2:].zfill(5)
                src2 = str(bin(int(ins[3][1:]) & 31))[2:].zfill(5)
                dest = str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                instr = str(int(df2['Func7'])).zfill(7)+src2+src1+ str(int(df2['Func3'])).zfill(3)+dest+str(int(df2['Opcode'])).zfill(7)
                hexrep = hex(int(instr,2))
                # print(hexrep)
                instrVal = int(instr,2)
                store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                for i in range(4):
                    memory[insStoreAddress+i]=store[i]
                insStoreAddress += 4
                pc = insStoreAddress 
            
            elif (df2['Type']=='I-Type') and (ins[0]!='lw'):
                src1 = str(bin(int(ins[2][1:]) & 31))[2:].zfill(5)
                immed = get12_bit(int(ins[3]))
                # print("imee ",immed)
                dest = str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                instr = immed+src1+ str(int(df2['Func3'])).zfill(3)+dest+str(int(df2['Opcode'])).zfill(7)
                # print("instr ",instr)
                hexrep = hex(int(instr,2))
                # print(hexrep)
                instrVal = int(instr,2)
                store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                for i in range(4):
                    memory[insStoreAddress+i]=store[i]
                insStoreAddress += 4
                pc = insStoreAddress 



            elif (df2['Type']=='I-Type') and (ins[0]=='lw') and (not ins[2][0:1].isalpha()):
                dest = str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                immed,src1 = ins[2].split('(')
                src1 = str(bin(int(src1[1:-1]) & 31))[2:].zfill(5)
                immed = get12_bit(int(immed))
                # //print('lw prop',dest,immed,src1)
                instr = immed+src1+ str(int(df2['Func3'])).zfill(3)+dest+str(int(df2['Opcode'])).zfill(7)
                # print("instr ",instr)
                hexrep = hex(int(instr,2))
                # print(hexrep)
                instrVal = int(instr,2)
                store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                for i in range(4):
                    memory[insStoreAddress+i]=store[i]
                insStoreAddress += 4
                pc = insStoreAddress 



            elif df2['Type']=='S-Type':
                src1 = str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                immed,src2 = ins[2].split('(')
                src2 = str(bin(int(src2[1:-1]) & 31))[2:].zfill(5)
                immed =get12_bit(int(immed))
                instr = immed[:7] +src1+src2+ str(int(df2['Func3'])).zfill(3)+immed[7:] +str(int(df2['Opcode'])).zfill(7)
                # print("instr(sw) ",instr)
                hexrep = hex(int(instr,2))
                # print(hexrep)
                instrVal = int(instr,2)
                store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                for i in range(4):
                    memory[insStoreAddress+i]=store[i]
                insStoreAddress += 4
                pc = insStoreAddress 
                
            elif df2['Type']=='SB-Type':
                src1 = str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                src2 = str(bin(int(ins[2][1:]) & 31))[2:].zfill(5)
                immed = get12_bit(self.labels[ins[3]]*4 - pc,bits=13)
                # print(len(immed))
                instr = immed[0]+immed[2:8]+src2+src1+str(int(df2['Func3'])).zfill(3)+immed[8:12]+immed[1]+str(int(df2['Opcode'])).zfill(7)
                # print("instr(sb) ",instr,pc)
                hexrep = hex(int(instr,2))
                # print(hexrep)
                instrVal = int(instr,2)
                store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                for i in range(4):
                    memory[insStoreAddress+i]=store[i]
                insStoreAddress += 4
                pc = insStoreAddress 

            elif df2['Type']=='UJ-Type':
                if ins[0]=='jal':
                    dest =  str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                    immed = get12_bit(self.labels[ins[2]]*4 - pc,bits=21)
                    instr = immed[0]+immed[10:20]+immed[9]+immed[1:9]+dest+str(int(df2['Opcode'])).zfill(7)
                    # print("instr(sb) ",instr,pc)
                    hexrep = hex(int(instr,2))
                    # print(hexrep)
                    instrVal = int(instr,2)
                    store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                    for i in range(4):
                        memory[insStoreAddress+i]=store[i]
                    insStoreAddress += 4
                    pc = insStoreAddress
                if ins[0]=='j':
                    dest =  '00000'
                    immed = get12_bit(self.labels[ins[1]]*4 - pc,bits=21)
                    instr = immed[0]+immed[10:20]+immed[9]+immed[1:9]+dest+str(int(df2['Opcode'])).zfill(7)
                    # print("instr(sb) ",instr,pc)
                    hexrep = hex(int(instr,2))
                    # print(hexrep)
                    instrVal = int(instr,2)
                    store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                    for i in range(4):
                        memory[insStoreAddress+i]=store[i]
                    insStoreAddress += 4
                    pc = insStoreAddress
            
        return pc-4
                


