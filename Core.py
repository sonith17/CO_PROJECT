import numpy as np
import pandas as pd

def twos_complement_binary_to_int(binary_str):
    if binary_str[0] == '1':
        positive_value = int(binary_str, 2)
        num_bits = len(binary_str)
        return positive_value - (1 << num_bits)
    else:
        return int(binary_str, 2)


class Core:
    pc = 0
    registers = np.zeros(32, dtype=np.int32)
    instructionExecuted = 0

    @classmethod
    def run(cls,memory,instructionLatencies,end_pc):
        a=0

    def IF(cls,pc_given,memory):
        instruction =""

        for i in range(4):
            instruction = bin(memory[pc_given+i])[2:].zfill(8) +instruction
        
        print("if ",instruction)
        return instruction

    def ID(cls,instruction):
        decoded =[]
        df = pd.read_csv('Instruction.csv')
        df = df[df['Opcode']==int(instruction[-7:])]
        if(df.iloc[0]['Type']=='R-Type'):
            df2 = df[(df['Func3']==int(instruction[-15:-12])) & (df['Func7']==int(instruction[:7]))].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-12:-7],2))
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(int(instruction[-25:-20],2))
        elif(df.iloc[0]['Type']=='I-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-12:-7],2))
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(twos_complement_binary_to_int(instruction[:12]))
        elif(df.iloc[0]['Type']=='S-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-25:-20],2))
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(twos_complement_binary_to_int(instruction[:7]+instruction[-12:-7]))
        elif(df.iloc[0]['Type']=='SB-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(int(instruction[-25:-20],2))
            decoded.append(twos_complement_binary_to_int(instruction[0]+instruction[-8]+instruction[1:7]+instruction[-12:-8]+'0'))
        elif(df.iloc[0]['Type']=='UJ-Type'):
            df2 = df.iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-12:-7],2))
            decoded.append(twos_complement_binary_to_int(instruction[0]+instruction[-20:-12]+instruction[11]+instruction[1:11]+'0'))
        return decoded
    
    def EXE(cls,decoded,currPc):
        executed =[decoded[0]]
        if decoded[0]=='add':
            val = cls.registers[decoded[2]]+cls.registers[decoded[3]]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='sub':
            val = cls.registers[decoded[2]]-cls.registers[decoded[3]]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='or':
            val = cls.registers[decoded[2]]|cls.registers[decoded[3]]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='and':
            val = cls.registers[decoded[2]]&cls.registers[decoded[3]]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='xor':
            val = cls.registers[decoded[2]]^cls.registers[decoded[3]]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='slt':
            val = 1 if cls.registers[decoded[2]] < cls.registers[decoded[3]] else 0
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='srl':
            val = cls.registers[decoded[2]] >> cls.registers[decoded[3]]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='sll':
            val = cls.registers[decoded[2]] << cls.registers[decoded[3]]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        
        elif decoded[0]=='addi':
            val = cls.registers[decoded[2]]+decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='ori':
            val = cls.registers[decoded[2]]|decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='andi':
            val = cls.registers[decoded[2]]&decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='xori':
            val = cls.registers[decoded[2]]^decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='slti':
            val = 1 if cls.registers[decoded[2]] < decoded[3] else 0
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='srli':
            val = cls.registers[decoded[2]] >> decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='slli':
            val = cls.registers[decoded[2]] << decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        
        elif decoded[0]=='lw':
            val = cls.registers[decoded[2]]+decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='sw':
            val = cls.registers[decoded[2]]+decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        
        elif decoded[0]=='beq':
            currPc = currPc+decoded[3] if cls.registers[decoded[1]]==cls.registers[decoded[2]] else currPc+4
            return executed,currPc
        elif decoded[0]=='bne':
            currPc = currPc+decoded[3] if cls.registers[decoded[1]]!=cls.registers[decoded[2]] else currPc+4
            return executed,currPc
        elif decoded[0]=='blt':
            currPc = currPc+decoded[3] if cls.registers[decoded[1]]< cls.registers[decoded[2]] else currPc+4
            return executed,currPc
        elif decoded[0]=='bge':
            currPc = currPc+decoded[3] if cls.registers[decoded[1]]>=cls.registers[decoded[2]] else currPc+4
            return executed,currPc
        
        elif decoded[0]=='jal':
            val = currPc+decoded[2]
            executed.append(decoded[1])
            executed.append(currPc+4)
            return executed,val
        elif decoded[0]=='jalr':
            val = currPc+4
            executed.append(decoded[1])
            executed.append(val)
            return executed,cls.registers[decoded[2]]+decoded[3]
        
    def MEM(cls,executed,memory):
        memorised=[]
        if executed[0]=='lw':
            val = ""
            for i in range(4):
               val = bin(memory[executed[2]+i])[2:].zfill(8) +val
            val = twos_complement_binary_to_int(val)
            memorised.append(executed[1])
            memorised.append(val)
            return memorised
        elif executed[0]=='sw':
            store = [(cls.registers[executed[1]] >> (i * 8)) & 0xFF for i in range(4)]
            for i in range(4):
                memory[executed[2]+i]=store[i]
            return []
        else:
            executed.pop(0)
            return executed
    
    def WB(cls,memorised):
        if memorised:
            cls.registers[memorised[0]]=memorised[1]
        return


            




    def test(cls,pc,memory):
        for i in range(0,pc+1,4):
            print(cls.ID(cls.IF(i,memory)))
        return False




