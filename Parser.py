import pandas as pd
import re

def get12_bit(num):
    if(num>=0):
        return bin(num)[2:].zfill(12)
    else:
        s=""
        num = -num
        tf = True
        for i in range(12):
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

    @classmethod
    def parse(cls,memory,instruction):
        df = pd.read_csv('Instruction.csv')
        print(df)
        insStoreAddress = 0
        pc = 0
        dataLabels = {}
        labels = {}
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
        print(instruction)
        while len(instruction)!=0 :
            ins = re.split(r',\s*|\s+', instruction[0])
            instruction.pop(0) #getting instruction for parse
            if len(ins)==1 and ins[0].endswith(':'): #check for labels
                labels[ins[0][:-1]] = pc
                continue
            elif len(ins)>1 and ins[0].endswith(':'):
                labels[ins[0][:-1]] = pc
                ins.pop(0)
            df2 = df[df['Instruction']==ins[0]]  #nxt 3 get details for that instruction
            print(df2) 
            df2 = df2.iloc[0]
            if df2['Type']=='R-Type':
                src1 = str(bin(int(ins[2][1:]) & 31))[2:].zfill(5)
                src2 = str(bin(int(ins[3][1:]) & 31))[2:].zfill(5)
                dest = str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                instr = str(int(df2['Func7'])).zfill(7)+src2+src1+ str(int(df2['Func3'])).zfill(3)+dest+str(int(df2['Opcode'])).zfill(7)
                hexrep = hex(int(instr,2))
                print(hexrep)
                instrVal = int(instr,2)
                store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                for i in range(4):
                    memory[insStoreAddress+i]=store[i]
                insStoreAddress += 4
                pc = insStoreAddress -4
            
            elif (df2['Type']=='I-Type') and (ins[0]!='lw'):
                src1 = str(bin(int(ins[2][1:]) & 31))[2:].zfill(5)
                immed = get12_bit(int(ins[3]))
                print("imee ",immed)
                dest = str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                instr = immed+src1+ str(int(df2['Func3'])).zfill(3)+dest+str(int(df2['Opcode'])).zfill(7)
                print("instr ",instr)
                hexrep = hex(int(instr,2))
                print(hexrep)
                instrVal = int(instr,2)
                store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                for i in range(4):
                    memory[insStoreAddress+i]=store[i]
                insStoreAddress += 4
                pc = insStoreAddress -4

            elif df2['Type']=='S-Type':
                src1 = str(bin(int(ins[1][1:]) & 31))[2:].zfill(5)
                immed,src2 = ins[2].split('(')
                src2 = str(bin(int(src2[1:-1]) & 31))[2:].zfill(5)
                immed =get12_bit(int(immed))
                instr = immed[:7] +src1+src2+ str(int(df2['Func3'])).zfill(3)+immed[7:] +str(int(df2['Opcode'])).zfill(7)
                print("instr(sw) ",instr)
                hexrep = hex(int(instr,2))
                print(hexrep)
                instrVal = int(instr,2)
                store = [(instrVal >> (i * 8)) & 0xFF for i in range(4)]
                for i in range(4):
                    memory[insStoreAddress+i]=store[i]
                insStoreAddress += 4
                pc = insStoreAddress -4
                print('pc is ',pc)
