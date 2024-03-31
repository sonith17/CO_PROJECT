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
    stalls = 0
    pipeline = []
    pipelineLatency = {}
    IF_input =0
    ID_input =[]
    EXE_input =[]
    MEM_input =[]
    WB_input =[]
    IF_output =''
    ID_output =[]
    EXE_output =[]
    MEM_output =[]
    WB_output =[]
    registerInUse =[]
    toBeUsedRegisters =[]
    stalled_at_IF = False
    stalled_at_ID = False
    stalled_at_EXE = False
    stalled_at_MEM = False
    stalled_at_WB = False
    branchTaken = False
    harzad = False


    @classmethod
    def run(cls,memory,instructionLatencies,end_pc):
        if(cls.pipeline or cls.pc<=end_pc):
            if cls.pc <= end_pc and not cls.stalled_at_IF:
                cls.pipeline.append([cls.pc,1,1])
                cls.pipelineLatency[cls.pc]={1,1,1,1,1,1} #will be updated 
                cls.IF_input=cls.pc

        for i in cls.pipeline:
            if i[1]==1:
                cls.IF_output=cls.IF(cls.IF_input,memory)
                if i[2] > 0:
                    i[2] -= 1
                if i[2]==0:
                    cls.ID_input=cls.IF_output
                    cls.stalled_at_IF = False
                else:
                    cls.stalled_at_IF = True
            elif i[1]==2:
                cls.ID_output=cls.ID(cls.ID_input)
                cls.pipelineLatency[i[0]]=instructionLatencies[cls.ID_output[0]]
                if i[2] > 0:
                    i[2] -= 1
                for wr,src1,src2 in cls.registerInUse:
                    if ( wr in cls.ID_output[2:] and wr!= -1):
                        cls.harzad = True
                        break
                    else:
                        cls.harzad=False
                if i[2]==0 and not cls.harzad:
                    cls.EXE_input=cls.ID_output
                    cls.stalled_at_ID = False
                else:
                    cls.stalled_at_IF = True
                    cls.stalled_at_ID = True
            elif i[1]==3:
                cls.EXE_output,nextPc=cls.EXE(cls.EXE_input,i[0])
                if nextPc-i[0]!=4:
                    cls.branchTaken=True
                    cls.pc = nextPc
                if i[2] > 0:
                    i[2] -= 1
                if i[2]==0:
                    cls.MEM_input = cls.EXE_output
                    cls.stalled_at_EXE = False
                else:
                    cls.stalled_at_EXE = True
                    cls.stalled_at_IF = True
                    cls.stalled_at_ID = True
            elif i[1] == 4:
                cls.MEM_output=cls.MEM(cls.MEM_input,memory)
                if i[2] > 0:
                    i[2] -= 1
                if i[2]==0:
                    cls.WB_input=cls.MEM_output
                    cls.stalled_at_MEM = False
                else:
                    cls.stalled_at_MEM = True
                    cls.stalled_at_EXE = True
                    cls.stalled_at_ID = True
                    cls.stalled_at_IF = True
            elif i[1] == 5:
                cls.WB(cls.WB_input)
                if i[2] > 0:
                    i[2] -= 1
                if i[2]==0:
                    cls.stalled_at_WB = False
                else:
                    cls.stalled_at_WB = True
                    cls.stalled_at_MEM = True
                    cls.stalled_at_EXE = True
                    cls.stalled_at_ID = True
                    cls.stalled_at_IF = True

        while(cls.pipeline[0][0]== -1):
            cls.pipeline.pop(0)
            cls.stall+=1
        if(cls.pipeline[0][1]==5 and cls.pipeline[0][1]==0):
            cls.pipeline.pop(0)
            cls.registerInUse.pop(0)
            cls.instructionExecuted+=1


        #need to update inuseregiter with,handle hazard,stalls due to var latencies.updatelatencies,promote to next stage
        if(cls.harzad):
            index = 0
            while index < len(cls.pipeline):
                if cls.pipeline[index][1]==2:
                    cls.insert(index,[-1,-1,-1])
                    index+=1
                index+=1

        if cls.branchTaken:  #assuming branch statements latency is one
            cls.pipeline = [row for row in cls.pipeline if row[1] not in [1, 2]] 
            cls.append([-1.-1,-1])
            cls.append([-1.-1,-1])
            cls.branchTaken = False
            return
                

                
                





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
            cls.toBeUsedRegisters=[int(instruction[-12:-7],2),int(instruction[-20:-15],2),int(instruction[-25:-20],2)]
        elif(df.iloc[0]['Type']=='I-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-12:-7],2))
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(twos_complement_binary_to_int(instruction[:12]))
            cls.toBeUsedRegisters=[int(instruction[-12:-7],2),int(instruction[-20:-15],2),-1]
        elif(df.iloc[0]['Type']=='S-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-25:-20],2))
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(twos_complement_binary_to_int(instruction[:7]+instruction[-12:-7]))
            cls.toBeUsedRegisters=[-1,int(instruction[-20:-15],2),int(instruction[-25:-20],2)]
        elif(df.iloc[0]['Type']=='SB-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(int(instruction[-25:-20],2))
            decoded.append(twos_complement_binary_to_int(instruction[0]+instruction[-8]+instruction[1:7]+instruction[-12:-8]+'0'))
            cls.toBeUsedRegisters=[-1,int(instruction[-20:-15],2),int(instruction[-25:-20],2)]
        elif(df.iloc[0]['Type']=='UJ-Type'):
            df2 = df.iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-12:-7],2))
            decoded.append(twos_complement_binary_to_int(instruction[0]+instruction[-20:-12]+instruction[11]+instruction[1:11]+'0'))
            cls.toBeUsedRegisters=[int(instruction[-12:-7],2),-1,-1]
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




